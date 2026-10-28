/*
 * Copyright (c) 2026 RAKwireless Technology Limited
 * Author: sercan.erat@rakwireless.com
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * Needs the loader built with the rzi snippet (-S rzi).
 */

#include <Arduino.h>

#if !defined(CONFIG_RZI_LORAWAN) || !defined(CONFIG_RZI_LLEXT_EXPORTS)

void setup() {
	Serial.begin(115200);
	delay(2000);
	Serial.println("Build the loader with the rzi snippet: extra/build.sh <board> -S rzi");
}

void loop() {
}

#else

#include <zephyr/kernel.h>

#include <rzi/err.h>
#include <rzi/init.h>
#include <rzi/lorawan/lorawan.h>

#define LORAWAN_REGION     RZI_LORAWAN_REGION_EU_868
#define LORAWAN_CLASS      RZI_LORAWAN_CLASS_A
#define LORAWAN_ADR        true
#define LORAWAN_DATA_RATE  RZI_LORAWAN_DR_5
#define RETRY_DELAY_MS     5000
#define UPLINK_INTERVAL_MS 60000
#define UPLINK_PORT        101
#define BLINK_PERIOD_MS    500

/* Fill in your own OTAA credentials from your LoRaWAN network server */
static const uint8_t dev_eui[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const uint8_t join_eui[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const uint8_t app_key[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
									0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static const uint8_t gen_app_key[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
										0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static bool ready;

/*
 * on_event() runs on the RZI dispatcher thread: it only records the
 * events, and setup()/loop() print them from the sketch thread.
 */
static atomic_t stack_ready;
static atomic_t join_done;
static atomic_t joined;
static atomic_t join_error;

/* TX event state */
static struct rzi_lorawan_tx_result tx_event;
static atomic_t tx_pending;

/* Downlink event */
static struct {
	uint8_t port;
	size_t size;
	int16_t rssi_dbm;
	int8_t snr_quarter_db;
	uint8_t data[242];
} dl_event;

static atomic_t dl_pending;

/* Error event */
static atomic_t error_event;
static atomic_t error_pending;

/* Blinks the LED on the system workqueue until the join completes */
static struct k_work_delayable blink_work;

static void blink_handler(struct k_work *work) {
	static bool led_on;

	ARG_UNUSED(work);
	led_on = !led_on;
	digitalWrite(LED_BUILTIN, led_on ? HIGH : LOW);
	k_work_schedule(&blink_work, K_MSEC(BLINK_PERIOD_MS));
}

static void on_event(const struct rzi_lorawan_event *event, void *user_data) {
	ARG_UNUSED(user_data);

	switch (event->type) {
	case RZI_LORAWAN_EVENT_READY:
		atomic_set(&stack_ready, 1);
		break;
	case RZI_LORAWAN_EVENT_STATE_CHANGED:
		if (event->state == RZI_LORAWAN_STATE_READY) {
			atomic_set(&stack_ready, 1);
		}
		break;
	case RZI_LORAWAN_EVENT_JOINED:
		atomic_set(&joined, 1);
		atomic_set(&join_done, 1);
		break;
	case RZI_LORAWAN_EVENT_JOIN_FAILED:
		atomic_set(&join_error, event->error);
		atomic_set(&joined, 0);
		atomic_set(&join_done, 1);
		break;
	case RZI_LORAWAN_EVENT_TX_DONE:
		if (!atomic_get(&tx_pending)) {
			tx_event = event->tx;
			atomic_set(&tx_pending, 1);
		}
		break;
	case RZI_LORAWAN_EVENT_DOWNLINK:
		if (!atomic_get(&dl_pending)) {
			dl_event.port = event->downlink.port;
			dl_event.size = MIN(event->downlink.size, sizeof(dl_event.data));
			dl_event.rssi_dbm = event->downlink.rssi_dbm;
			dl_event.snr_quarter_db = event->downlink.snr_quarter_db;
			if (dl_event.size) {
				memcpy(dl_event.data, event->downlink.data, dl_event.size);
			}
			atomic_set(&dl_pending, 1);
		}
		break;
	case RZI_LORAWAN_EVENT_ERROR:
		atomic_set(&error_event, event->error);
		atomic_set(&error_pending, 1);
		break;
	default:
		break;
	}
}

static const struct rzi_lorawan_callbacks callbacks = {
	.on_event = on_event,
	.user_data = NULL,
};

static const char *tx_status_str(enum rzi_lorawan_tx_status status) {
	switch (status) {
	case RZI_LORAWAN_TX_SENT:
		return "sent";
	case RZI_LORAWAN_TX_ACKED:
		return "acked";
	case RZI_LORAWAN_TX_NOT_SENT:
		return "not sent";
	default:
		return "unknown";
	}
}

static const char *eui_str(const uint8_t eui[8]) {
	static const char digits[] = "0123456789ABCDEF";
	static char str[2 * 8 + 1];

	for (size_t i = 0; i < 8; i++) {
		str[2 * i] = digits[eui[i] >> 4];
		str[2 * i + 1] = digits[eui[i] & 0x0F];
	}
	str[2 * 8] = '\0';
	return str;
}

static bool check(const char *what, int rc, const char *value = NULL) {
	Serial.print(what);
	if (rc != 0) {
		Serial.print(" failed: ");
		Serial.println(rzi_err_str(rc));
		return false;
	}
	Serial.print(" OK");
	if (value) {
		Serial.print(": ");
		Serial.print(value);
	}
	Serial.println();
	return true;
}

static void print_pending_events(void) {
	if (atomic_get(&tx_pending)) {
		Serial.print("Uplink ");
		Serial.print(tx_status_str(tx_event.status));
		if (tx_event.error != 0) {
			Serial.print(": ");
			Serial.print(rzi_err_str(tx_event.error));
		}
		Serial.println();
		atomic_set(&tx_pending, 0);
	}

	if (atomic_get(&dl_pending)) {
		Serial.print("Downlink: port ");
		Serial.print(dl_event.port);
		Serial.print(", ");
		Serial.print((unsigned int)dl_event.size);
		Serial.print(" bytes, RSSI ");
		Serial.print(dl_event.rssi_dbm);
		Serial.print(" dBm, SNR ");
		Serial.print(dl_event.snr_quarter_db / 4.0f, 2);
		Serial.println(" dB");
		for (size_t i = 0; i < dl_event.size; i++) {
			if (dl_event.data[i] < 0x10) {
				Serial.print('0');
			}
			Serial.print(dl_event.data[i], HEX);
			Serial.print(' ');
		}
		if (dl_event.size) {
			Serial.println();
		}
		atomic_set(&dl_pending, 0);
	}

	if (atomic_cas(&error_pending, 1, 0)) {
		Serial.print("Error: ");
		Serial.println(rzi_err_str((int)atomic_get(&error_event)));
	}
}

static bool request_join(void) {
	atomic_set(&joined, 0);
	atomic_set(&join_done, 0);

	int rc = rzi_lorawan_join(1, 0, RZI_LORAWAN_JOIN_INTERVAL_DEFAULT, 0);
	if (!check("Join request", rc)) {
		return false;
	}

	Serial.println("Joining");
	while (!atomic_get(&join_done)) {
		print_pending_events();
		delay(100);
	}
	if (!atomic_get(&joined)) {
		Serial.print("Join failed: ");
		Serial.println(rzi_err_str((int)atomic_get(&join_error)));
		return false;
	}
	return true;
}

void setup() {
	pinMode(LED_BUILTIN, OUTPUT);
	k_work_init_delayable(&blink_work, blink_handler);

	Serial.begin(115200);
	delay(2000);

	Serial.println("RAKwireless Arduino RZI LoRaWAN Periodical Uplink Example");
	Serial.println("---------------------------------------------------------");

	if (!check("RZI init", rzi_init()) ||
		!check("Callback registration", rzi_lorawan_register_callbacks(&callbacks)) ||
		!check("DevEUI", rzi_lorawan_set_dev_eui(dev_eui, sizeof(dev_eui)), eui_str(dev_eui)) ||
		!check("JoinEUI", rzi_lorawan_set_app_eui(join_eui, sizeof(join_eui)), eui_str(join_eui)) ||
		!check("AppKey", rzi_lorawan_set_app_key(app_key, sizeof(app_key))) ||
		!check("GenAppKey", rzi_lorawan_set_gen_app_key(gen_app_key, sizeof(gen_app_key))) ||
		!check("Activation", rzi_lorawan_set_activation(RZI_LORAWAN_ACTIVATION_OTAA), "OTAA") ||
		!check("LoRaWAN init", rzi_lorawan_init(LORAWAN_REGION), STRINGIFY(LORAWAN_REGION))) {
		return;
	}

	Serial.print("Waiting for the LoRaWAN stack");
	while (!atomic_get(&stack_ready)) {
		Serial.print('.');
		delay(100);
	}
	Serial.println();
	Serial.println("LoRaWAN stack OK");

	k_work_schedule(&blink_work, K_NO_WAIT);
	while (!request_join()) {
		Serial.print("Retrying join in ");
		Serial.print(RETRY_DELAY_MS / 1000);
		Serial.println(" s");
		(void)rzi_lorawan_leave();
		delay(RETRY_DELAY_MS);
	}
	k_work_cancel_delayable(&blink_work);
	digitalWrite(LED_BUILTIN, LOW);
	Serial.println("Joined");

	/*
	 * Class, ADR and a fixed data rate apply only after the join: USP
	 * rejects set_class() until the modem is joined. set_data_rate()
	 * pins one rate and turns ADR off, so only one of them is used.
	 */
	check("Class", rzi_lorawan_set_class(LORAWAN_CLASS), STRINGIFY(LORAWAN_CLASS));
	if (LORAWAN_ADR) {
		check("ADR", rzi_lorawan_set_adr(true), "enabled");
	} else {
		check("Data rate", rzi_lorawan_set_data_rate(LORAWAN_DATA_RATE),
			  STRINGIFY(LORAWAN_DATA_RATE));
	}

	ready = true;
}

void loop() {
	static uint32_t uplink_counter;

	if (!ready) {
		delay(1000);
		return;
	}

	/* Highest byte first, so a server reads the counter as a normal integer */
	uint8_t payload[4] = {
		(uint8_t)(uplink_counter >> 24),
		(uint8_t)(uplink_counter >> 16),
		(uint8_t)(uplink_counter >> 8),
		(uint8_t)uplink_counter,
	};

	int rc = rzi_lorawan_send(UPLINK_PORT, payload, sizeof(payload), RZI_LORAWAN_MSG_UNCONFIRMED);
	if (rc == 0) {
		Serial.print("Uplink ");
		Serial.print(uplink_counter);
		Serial.print(" on port ");
		Serial.println(UPLINK_PORT);
		uplink_counter++;
	} else if (rc != -RZI_ERR_BUSY) {
		check("Uplink", rc);
	}

	/* Wait for the next uplink, printing callback events as they arrive */
	for (uint32_t waited = 0; waited < UPLINK_INTERVAL_MS; waited += 100) {
		print_pending_events();
		delay(100);
	}
}
#endif
