/*
 * Copyright (c) 2026 RAKwireless Technology Limited
 * Author: sercan.erat@rakwireless.com
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * This example shows random function, it reads WB_A0 as random seed.
 */

#include "Arduino.h"

#if defined(WISBLOCK_BASE_19007)
uint8_t inputPin = WB_A0;
#else
uint8_t inputPin = 0xFF;
#endif

void setup() {
	// initialize serial communication at 115200 bits per second
	Serial.begin(115200);
	delay(2000);

	Serial.println("RAKwireless Arduino Random Example");
	Serial.println("------------------------------------------------------");

	// initializes the pseudo-random number generator
	randomSeed(analogRead(inputPin));
}

void loop() {
	Serial.print("Random number(0 ~ 999) : ");
	uint32_t l = random(1000); // generate a random number between 0 and 999
	Serial.println(l);
	delay(1000);
}
