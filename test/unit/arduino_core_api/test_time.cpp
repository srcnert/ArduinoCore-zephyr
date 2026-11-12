/*
 * Copyright (c) 2026 RAKwireless Technology Limited
 * Author: sercan.erat@rakwireless.com
 * SPDX-License-Identifier: Apache-2.0
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

TEST_GROUP(Time){};

TEST(Time, delay_ms) {
	unsigned long start = millis();

	delay(20);
	unsigned long elapsed = millis() - start;

	CHECK_COMPARE(elapsed, >=, 20);
	CHECK_COMPARE(elapsed, <=, 30);
}

TEST(Time, delay_us) {
	unsigned long start = micros();

	delayMicroseconds(500);
	unsigned long elapsed = micros() - start;

	// micros() can count in 30.5 us steps (32.768 kHz system timer)
	CHECK_COMPARE(elapsed, >=, 500 - 31);
	CHECK_COMPARE(elapsed, <=, 1500);
}
