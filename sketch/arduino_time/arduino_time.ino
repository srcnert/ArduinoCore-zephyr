/*
 * Copyright (c) 2026 RAKwireless Technology Limited
 * Author: sercan.erat@rakwireless.com
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * This example shows time function, including millis, micros, delay, delayMicroseconds.
 */

#include "Arduino.h"

long delayTime = 1000; // variable for setting the delay time

void setup() {
	// initialize serial communication at 115200 bits per second
	Serial.begin(115200);
	delay(2000);

	Serial.println("RAKwireless Arduino Time Example");
	Serial.println("------------------------------------------------------");
}

void loop() {
	unsigned long msBefore, usBefore;
	unsigned long msAfter, usAfter;

	usBefore = micros();
	msBefore = millis();

	Serial.println("Now Time:");
	Serial.print("millis(): ");
	Serial.println(msBefore); // show the time with millis
	Serial.print("micros(): ");
	Serial.println(usBefore); // show the time with micros

	Serial.printf("After Delay %ld milliseconds\n", delayTime);

	msBefore = millis();
	usBefore = micros();
	delay(delayTime); // delay time (milliseconds)
	usAfter = micros();
	msAfter = millis();

	Serial.print("millis(): ");
	Serial.println(msAfter); // show the time with millis
	Serial.print("micros(): ");
	Serial.println(usAfter); // show the time with micros

	Serial.printf("measured: %lu ms / %lu us\n", msAfter - msBefore, usAfter - usBefore);

	Serial.printf("After Delay %ld microseconds\n", delayTime);

	msBefore = millis();
	usBefore = micros();
	delayMicroseconds(delayTime); // delay time (Microseconds)
	usAfter = micros();
	msAfter = millis();

	Serial.print("millis(): ");
	Serial.println(msAfter); // show the time with millis
	Serial.print("micros(): ");
	Serial.println(usAfter); // show the time with micros

	Serial.printf("measured: %lu ms / %lu us\n", msAfter - msBefore, usAfter - usBefore);

	Serial.println("");

	delayTime += 1000; // delay time add 1000

	delay(5000);
}
