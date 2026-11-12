/*
 * Copyright (c) 2026 RAKwireless Technology Limited
 * Author: sercan.erat@rakwireless.com
 * SPDX-License-Identifier: Apache-2.0
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>
#include <avr/dtostrf.h>

TEST_GROUP(Conversion){};

TEST(Conversion, integers) {
	char buf[34];

	STRCMP_EQUAL("-42", itoa(-42, buf, 10));
	STRCMP_EQUAL("1e240", ltoa(123456L, buf, 16));
	STRCMP_EQUAL("11111111", utoa(255u, buf, 2));
}

TEST(Conversion, floating_point) {
	char buf[16];

	STRCMP_EQUAL("   3.14", dtostrf(3.14159, 7, 2, buf));
}
