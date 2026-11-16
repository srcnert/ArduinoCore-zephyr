/*
 * Copyright (c) 2026 RAKwireless Technology Limited
 * Author: sercan.erat@rakwireless.com
 * SPDX-License-Identifier: Apache-2.0
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>
#include <math.h>

TEST_GROUP(ArduinoMath){};

TEST(ArduinoMath, numbers) {
	LONGS_EQUAL(3, min(3, 5));
	LONGS_EQUAL(5, max(3, 5));
	LONGS_EQUAL(7, abs(-7));
	LONGS_EQUAL(10, constrain(15, 0, 10));
	LONGS_EQUAL(500, map(50, 0, 100, 0, 1000));
	LONGS_EQUAL(144, sq(12));
}

TEST(ArduinoMath, angles) {
	DOUBLES_EQUAL(M_PI, radians(180.0), 1e-12);
	DOUBLES_EQUAL(90.0, degrees(M_PI / 2), 1e-9);
}

TEST(ArduinoMath, bits_and_bytes) {
	unsigned long bits = 0;

	bitSet(bits, 4);
	bitWrite(bits, 0, 1);
	bitClear(bits, 4);
	LONGS_EQUAL(1, bits);
	LONGS_EQUAL(8, bit(3));
	LONGS_EQUAL(1, bitRead(0x10, 4));
	LONGS_EQUAL(0x34, lowByte(0x1234));
	LONGS_EQUAL(0x12, highByte(0x1234));
	LONGS_EQUAL(0x1234, word(0x12, 0x34));
}

TEST(ArduinoMath, random_seed) {
	randomSeed(1234);
	long first = random(1000);

	randomSeed(1234);
	LONGS_EQUAL(first, random(1000));
}

TEST(ArduinoMath, random_range) {
	for (int i = 0; i < 100; i++) {
		long r = random(5, 10);

		CHECK_COMPARE(r, >=, 5);
		CHECK_COMPARE(r, <, 10);
	}
}
