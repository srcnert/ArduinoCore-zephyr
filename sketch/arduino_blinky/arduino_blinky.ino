/*
 * Copyright (c) 2026 RAKwireless Technology Limited
 * Author: sercan.erat@rakwireless.com
 * SPDX-License-Identifier: Apache-2.0
 */

void setup() {
	// initialize serial communication at 115200 bits per second
	Serial.begin(115200);
	delay(2000);

	pinMode(LED_BUILTIN, OUTPUT);

	Serial.println("RAKwireless Arduino Blinky Example");
	Serial.println("-------------------------------------------");
}

void loop() {
	digitalWrite(LED_BUILTIN, HIGH);
	Serial.println("LED ON");
	delay(1000);

	digitalWrite(LED_BUILTIN, LOW);
	Serial.println("LED OFF");
	delay(1000);
}
