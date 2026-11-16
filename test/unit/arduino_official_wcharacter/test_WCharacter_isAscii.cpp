/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/WCharacter/test_isAscii.cpp
 * with an array and a for loop instead of std::vector and std::for_each
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

char const VALID_ASCII_VECT[] = {' ', 'a', 'b', 'q', '\n', '\r'};

TEST_GROUP(WCharacter_isAscii){};

TEST(WCharacter_isAscii, isAscii_01) {
	for (char const c : VALID_ASCII_VECT) {
		CHECK(arduino::isAscii(c));
	}
}

TEST(WCharacter_isAscii, isAscii_02) {
	CHECK_FALSE(arduino::isAscii(0xf7));
}

TEST(WCharacter_isAscii, isAscii_03) {
	CHECK_FALSE(arduino::isAscii((unsigned char)0xf7));
}

TEST(WCharacter_isAscii, isAscii_04) {
	CHECK_FALSE(arduino::isAscii(0x3030));
}
