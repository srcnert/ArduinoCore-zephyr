/*
 * Copyright (c) 2026 RAKwireless Technology Limited
 * Author: sercan.erat@rakwireless.com
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * LoRaWAN class A test sketch for the RAK4631 (nrf52840dk_nrf52840 variant).
 *
 */

#include <Arduino.h>
#include <zephyr/kernel.h>
#include <zephyr/lorawan/lorawan.h>

#define BLINK_PERIOD_MS          500

#define UPLINK_PERIOD_MS         30000
#define LORAWAN_PORT             2
#define JOIN_RETRY_MS            10000
#define JOIN_RETRY_RESTRICTED_MS 30000
/* Send a confirmed uplink (and a link check request) every Nth message */
#define CONFIRMED_EVERY          5

static uint8_t dev_eui[8] = {0xD8, 0x79, 0xE1, 0x9D, 0x36, 0xF2, 0x43, 0x8A};
static uint8_t join_eui[8] = {0x1D, 0x25, 0xB0, 0xB5, 0x26, 0xCB, 0x58, 0xDE};
static uint8_t app_key[16] = {0x2D, 0xCF, 0xEB, 0x73, 0xB8, 0x63, 0x68, 0x7C,
							  0x5B, 0xC2, 0x7E, 0xFB, 0x11, 0x49, 0x94, 0x1A};

static bool joined = false;

/*
 * Blink task: self-rescheduling work item on the system workqueue, so the
 * LED keeps blinking while setup()/loop() block on join/uplink. A steady
 * blink confirms the sketch is loaded and the scheduler is running.
 */
static struct k_work_delayable blink_work;

static void blink_handler(struct k_work *work) {
	static bool led_on;

	led_on = !led_on;
	digitalWrite(LED_BUILTIN, led_on ? HIGH : LOW);
	k_work_schedule(&blink_work, K_MSEC(BLINK_PERIOD_MS));
}

static void downlink_cb(uint8_t port, uint8_t flags, int16_t rssi, int8_t snr, uint8_t len,
						const uint8_t *payload) {
	Serial.println();
	Serial.print("Downlink: port ");
	Serial.print(port);
	Serial.print(", flags 0x");
	Serial.print(flags, HEX);
	Serial.print(", RSSI ");
	Serial.print(rssi);
	Serial.print(" dBm, SNR ");
	Serial.print(snr);
	Serial.print(" dB, ");
	Serial.print(len);
	Serial.println(" bytes");
	for (uint8_t i = 0; i < len; i++) {
		Serial.print(payload[i], HEX);
		Serial.print(' ');
	}
	if (len) {
		Serial.println();
	}
}

static void dr_changed_cb(enum lorawan_datarate dr) {
	Serial.println();
	Serial.print("ADR: datarate changed to DR");
	Serial.println((int)dr);
}

static void link_check_ans_cb(uint8_t margin, uint8_t gw_count) {
	Serial.println();
	Serial.print("Link check: margin ");
	Serial.print(margin);
	Serial.print(" dB, ");
	Serial.print(gw_count);
	Serial.println(" gateway(s)");
}

static uint8_t battery_level_cb(void) {
	return 255;
}

static struct lorawan_downlink_cb dl_cb = {
	.port = LW_RECV_PORT_ANY,
	.cb = downlink_cb,
};

/* "[  123.4 s] " uptime prefix for every log line */
static void print_uptime(void) {
	int64_t ms = k_uptime_get();

	Serial.print('[');
	Serial.print((uint32_t)(ms / 1000));
	Serial.print('.');
	Serial.print((uint32_t)((ms % 1000) / 100));
	Serial.print(" s] ");
}

void setup() {
	pinMode(LED_BUILTIN, OUTPUT);
	k_work_init_delayable(&blink_work, blink_handler);
	k_work_schedule(&blink_work, K_MSEC(BLINK_PERIOD_MS));

	Serial.begin(115200);
	delay(1000);

	Serial.println("=== RAK4631 LoRaWAN class A test ===");

	int ret = lorawan_set_region(LORAWAN_REGION_EU868);
	if (ret) {
		Serial.print("Failed to set LoRaWAN region: ");
		Serial.println(ret);
		return;
	}

	ret = lorawan_start();
	if (ret) {
		Serial.print("Failed to start LoRaWAN stack: ");
		Serial.println(ret);
		return;
	}
	Serial.println("LoRaWAN stack started (SX1262 OK)");

	lorawan_register_downlink_callback(&dl_cb);
	lorawan_register_dr_changed_callback(dr_changed_cb);
	lorawan_register_battery_level_callback(battery_level_cb);
	lorawan_register_link_check_ans_callback(link_check_ans_cb);
	lorawan_enable_adr(true);
	lorawan_set_conf_msg_tries(3);

	struct lorawan_join_config join_cfg = {};
	join_cfg.mode = LORAWAN_ACT_OTAA;
	join_cfg.dev_eui = dev_eui;
	join_cfg.otaa.join_eui = join_eui;
	join_cfg.otaa.app_key = app_key;
	join_cfg.otaa.nwk_key = app_key;

	int64_t restricted_since = -1;
	uint32_t restricted_polls = 0;

	for (int attempt = 1; !joined; attempt++) {
		print_uptime();
		Serial.print("Joining (attempt ");
		Serial.print(attempt);
		Serial.println(")...");

		int64_t t0 = k_uptime_get();
		ret = lorawan_join(&join_cfg);
		int64_t took = k_uptime_get() - t0;

		if (ret != -ECONNREFUSED && restricted_since >= 0) {
			print_uptime();
			Serial.print("Restriction lifted after ");
			Serial.print((uint32_t)((t0 - restricted_since) / 1000));
			Serial.print(" s (");
			Serial.print(restricted_polls);
			Serial.println(" refused polls)");
			restricted_since = -1;
			restricted_polls = 0;
		}

		if (ret == 0) {
			joined = true;
			print_uptime();
			Serial.println("Joined!");
		} else if (ret == -ECONNREFUSED) {
			if (restricted_since < 0) {
				restricted_since = t0;
			}
			restricted_polls++;
			print_uptime();
			Serial.print("Duty-cycle restricted for ");
			Serial.print((uint32_t)((t0 - restricted_since) / 1000));
			Serial.print(" s (call took ");
			Serial.print((uint32_t)took);
			Serial.print(" ms), retrying in ");
			Serial.print(JOIN_RETRY_RESTRICTED_MS / 1000);
			Serial.println("s");
			delay(JOIN_RETRY_RESTRICTED_MS);
		} else {
			print_uptime();
			Serial.print("Failed to join: ");
			Serial.print(ret);
			Serial.print(" (call took ");
			Serial.print((uint32_t)took);
			Serial.print(" ms), retrying in ");
			Serial.print(JOIN_RETRY_MS / 1000);
			Serial.println("s");
			delay(JOIN_RETRY_MS);
		}
	}

	/* Ask the network for the current time; answer arrives with a later uplink */
	ret = lorawan_request_device_time(false);
	if (ret) {
		Serial.print("Failed to request device time: ");
		Serial.println(ret);
	}
}

void loop() {
	static uint32_t counter = 0;

	if (!joined) {
		return;
	}

	uint8_t max_payload, max_payload_next;
	lorawan_get_payload_sizes(&max_payload, &max_payload_next);

	bool confirmed = (counter % CONFIRMED_EVERY) == (CONFIRMED_EVERY - 1);
	if (confirmed) {
		lorawan_request_link_check(false);
	}

	uint8_t payload[4] = {
		(uint8_t)(counter >> 24),
		(uint8_t)(counter >> 16),
		(uint8_t)(counter >> 8),
		(uint8_t)counter,
	};

	Serial.print("Uplink #");
	Serial.print(counter);
	Serial.print(confirmed ? " (confirmed" : " (unconfirmed");
	Serial.print(", max payload ");
	Serial.print(max_payload);
	Serial.print(" B)... ");

	int ret = lorawan_send(LORAWAN_PORT, payload, sizeof(payload),
						   confirmed ? LORAWAN_MSG_CONFIRMED : LORAWAN_MSG_UNCONFIRMED);
	if (ret == 0) {
		Serial.println("sent");
	} else if (ret == -EAGAIN) {
		Serial.println("duty cycle limit, retrying next period");
	} else {
		Serial.print("Failed to send uplink: ");
		Serial.println(ret);
	}

	uint32_t gps_time = 0;
	if (lorawan_device_time_get(&gps_time) == 0) {
		Serial.print("Network GPS time: ");
		Serial.println(gps_time);
	}

	counter++;
	delay(UPLINK_PERIOD_MS);
}
