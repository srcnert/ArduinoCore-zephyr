/*
 * Copyright (c) 2026 RAKwireless Technology Limited
 * Author: sercan.erat@rakwireless.com
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * BLE peripheral example for the RAK4631.
 *
 * To build this example, you need to install the ArduinoBLE library:
 * arduino-cli lib install ArduinoBLE
 */

#include <ArduinoBLE.h>

#define DEVICE_NAME "RAK4631"

BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214");
BLEByteCharacteristic ledCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

static void onConnected(BLEDevice central) {
	digitalWrite(LED_CONN, HIGH);
	Serial.print("Connected to central: ");
	Serial.println(central.address());
}

static void onDisconnected(BLEDevice central) {
	digitalWrite(LED_CONN, LOW);
	Serial.print("Disconnected from central: ");
	Serial.println(central.address());
}

static void onLedWritten(BLEDevice central, BLECharacteristic characteristic) {
	bool on = ledCharacteristic.value() != 0;

	digitalWrite(LED_BUILTIN, on ? HIGH : LOW);
	Serial.println(on ? "LED on" : "LED off");
}

void setup() {
	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(LED_CONN, OUTPUT);

	Serial.begin(115200);
	unsigned long start = millis();
	while (!Serial && millis() - start < 5000) {
		delay(100);
	}

	Serial.println("RAKwireless BLE Peripheral Example");
	Serial.println("------------------------------------------------------");

	if (!BLE.begin()) {
		Serial.println("Starting BLE failed!");
		while (true) {
			delay(1000);
		}
	}

	Serial.print("Local address: ");
	Serial.println(BLE.address());

	BLE.setLocalName(DEVICE_NAME);
	BLE.setDeviceName(DEVICE_NAME);
	BLE.setAdvertisedService(ledService);

	ledService.addCharacteristic(ledCharacteristic);
	BLE.addService(ledService);
	ledCharacteristic.writeValue(0);

	BLE.setEventHandler(BLEConnected, onConnected);
	BLE.setEventHandler(BLEDisconnected, onDisconnected);
	ledCharacteristic.setEventHandler(BLEWritten, onLedWritten);

	BLE.advertise();
	Serial.println("Advertising as \"" DEVICE_NAME "\"...");
}

void loop() {
	BLE.poll();
}
