/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/WCharacter/test_isControl.cpp
 * with an array and a for loop instead of std::vector and std::for_each
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

char const VALID_CONTROL_VECT[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
								   '\t', '\f', '\v', '\n', '\r', 0x0E, 0x0F, 0x10, 0x11,
								   0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A,
								   0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x7F};

TEST_GROUP(WCharacter_isControl){};

TEST(WCharacter_isControl, isControl_01) {
	for (char const c : VALID_CONTROL_VECT) {
		CHECK(arduino::isControl(c));
	}
}

TEST(WCharacter_isControl, isControl_02) {
	CHECK_FALSE(arduino::isControl('z'));
}
