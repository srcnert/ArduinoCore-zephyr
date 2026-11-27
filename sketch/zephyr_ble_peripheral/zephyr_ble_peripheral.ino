/*
 * Copyright (c) 2026 RAKwireless Technology Limited
 * Author: sercan.erat@rakwireless.com
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * BLE peripheral example for the RAK4631, using the Zephyr Bluetooth host
 * API instead of ArduinoBLE (see sketch/ble_peripheral for that version).
 */

#include <Arduino.h>

#if !defined(CONFIG_RAK_ZEPHYR_BLE)

void setup() {
	Serial.begin(115200);
	delay(2000);
	Serial.println("Build the loader with the zephyr-ble snippet: extra/build.sh -S zephyr-ble");
}

void loop() {
}

#else

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/gatt.h>

#define DEVICE_NAME "RAK4631"

#define ADV_INTERVAL BT_GAP_MS_TO_ADV_INTERVAL(100)

#define LED_SERVICE_UUID BT_UUID_128_ENCODE(0x19B10000, 0xE8F2, 0x537E, 0x4F6C, 0xD104768A1214)
#define LED_CHAR_UUID    BT_UUID_128_ENCODE(0x19B10001, 0xE8F2, 0x537E, 0x4F6C, 0xD104768A1214)

static const struct bt_uuid_128 led_service_uuid = BT_UUID_INIT_128(LED_SERVICE_UUID);
static const struct bt_uuid_128 led_char_uuid = BT_UUID_INIT_128(LED_CHAR_UUID);

static uint8_t led_value;
static atomic_t led_written;

static bt_addr_le_t peer_addr;
static atomic_t connected_pending;
static atomic_t disconnected_pending;
static atomic_t recycled_pending;

static ssize_t read_led(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf,
						uint16_t len, uint16_t offset) {
	return bt_gatt_attr_read(conn, attr, buf, len, offset, &led_value, sizeof(led_value));
}

static ssize_t write_led(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf,
						 uint16_t len, uint16_t offset, uint8_t flags) {
	if (offset != 0) {
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
	}
	if (len != sizeof(led_value)) {
		return BT_GATT_ERR(BT_ATT_ERR_INVALID_ATTRIBUTE_LEN);
	}

	led_value = *(const uint8_t *)buf;
	atomic_set(&led_written, 1);
	return len;
}

static struct bt_gatt_attr led_attrs[] = {
	BT_GATT_PRIMARY_SERVICE(&led_service_uuid),
	BT_GATT_CHARACTERISTIC(&led_char_uuid.uuid, BT_GATT_CHRC_READ | BT_GATT_CHRC_WRITE,
						   BT_GATT_PERM_READ | BT_GATT_PERM_WRITE, read_led, write_led, NULL),
};

static struct bt_gatt_service led_service = BT_GATT_SERVICE(led_attrs);

static const struct bt_le_adv_param adv_param =
	BT_LE_ADV_PARAM_INIT(BT_LE_ADV_OPT_CONN, ADV_INTERVAL, ADV_INTERVAL, NULL);

static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA_BYTES(BT_DATA_UUID128_ALL, LED_SERVICE_UUID),
};

static const struct bt_data sd[] = {
	BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, sizeof(DEVICE_NAME) - 1),
};

static void on_connected(struct bt_conn *conn, uint8_t err) {
	if (err) {
		return;
	}

	bt_addr_le_copy(&peer_addr, bt_conn_get_dst(conn));
	atomic_set(&connected_pending, 1);
}

static void on_disconnected(struct bt_conn *conn, uint8_t reason) {
	bt_addr_le_copy(&peer_addr, bt_conn_get_dst(conn));
	atomic_set(&disconnected_pending, 1);
}

/* The connection object is free again: advertising can be restarted. */
static void on_recycled(void) {
	atomic_set(&recycled_pending, 1);
}

static struct bt_conn_cb conn_callbacks = {
	.connected = on_connected,
	.disconnected = on_disconnected,
	.recycled = on_recycled,
};

static void print_addr(const bt_addr_le_t *addr) {
	for (int i = 5; i >= 0; i--) {
		if (addr->a.val[i] < 0x10) {
			Serial.print('0');
		}
		Serial.print(addr->a.val[i], HEX);
		if (i) {
			Serial.print(':');
		}
	}
	Serial.println(addr->type == BT_ADDR_LE_PUBLIC ? " (public)" : " (random)");
}

static void start_advertising(void) {
	int err = bt_le_adv_start(&adv_param, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
	if (err) {
		Serial.print("Advertising failed to start: ");
		Serial.println(err);
		return;
	}

	Serial.println("Advertising as \"" DEVICE_NAME "\"...");
}

void setup() {
	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(LED_CONN, OUTPUT);

	Serial.begin(115200);
	unsigned long start = millis();
	while (!Serial && millis() - start < 5000) {
		delay(100);
	}

	Serial.println("RAKwireless Zephyr BLE Peripheral Example");
	Serial.println("------------------------------------------------------");

	int err = bt_enable(NULL);
	if (err) {
		Serial.print("Starting BLE failed: ");
		Serial.println(err);
		while (true) {
			delay(1000);
		}
	}

	bt_addr_le_t addr;
	size_t count = 1;
	bt_id_get(&addr, &count);
	Serial.print("Local address: ");
	print_addr(&addr);

	bt_set_name(DEVICE_NAME);
	bt_conn_cb_register(&conn_callbacks);

	err = bt_gatt_service_register(&led_service);
	if (err) {
		Serial.print("GATT service registration failed: ");
		Serial.println(err);
	}

	start_advertising();
}

void loop() {
	if (atomic_cas(&connected_pending, 1, 0)) {
		digitalWrite(LED_CONN, HIGH);
		Serial.print("Connected to central: ");
		print_addr(&peer_addr);
	}

	if (atomic_cas(&disconnected_pending, 1, 0)) {
		digitalWrite(LED_CONN, LOW);
		Serial.print("Disconnected from central: ");
		print_addr(&peer_addr);
	}

	if (atomic_cas(&recycled_pending, 1, 0)) {
		start_advertising();
	}

	if (atomic_cas(&led_written, 1, 0)) {
		bool on = led_value != 0;

		digitalWrite(LED_BUILTIN, on ? HIGH : LOW);
		Serial.println(on ? "LED on" : "LED off");
	}

	delay(10);
}
#endif
