/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/WCharacter/test_toAscii.cpp
 * with an array and a for loop instead of std::vector and std::for_each
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

char const VALID_ASCII_VECT[] = {' ', 'a', 'b', 'q', '\n', '\r'};

TEST_GROUP(WCharacter_toAscii){};

TEST(WCharacter_toAscii, toAscii_01) {
	for (char const c : VALID_ASCII_VECT) {
		LONGS_EQUAL(c, arduino::toAscii(c));
	}
}

TEST(WCharacter_toAscii, toAscii_02) {
	LONGS_EQUAL(0x77, arduino::toAscii(0xf7));
}

TEST(WCharacter_toAscii, toAscii_03) {
	LONGS_EQUAL(0x77, arduino::toAscii((unsigned char)0xf7));
}

TEST(WCharacter_toAscii, toAscii_04) {
	LONGS_EQUAL(0x30, arduino::toAscii(0x3030));
}
