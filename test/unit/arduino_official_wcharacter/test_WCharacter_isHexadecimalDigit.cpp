/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2
 * test/src/WCharacter/test_isHexadecimalDigit.cpp
 * with an array and a for loop instead of std::vector and std::for_each
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

char const VALID_HEXADECIMAL_NUMBER_VECT[] = {'0', '1', '2', '3', '4', '5', '6', '7',
											  '8', '9', 'a', 'b', 'c', 'd', 'e', 'f',
											  'A', 'B', 'C', 'D', 'E', 'F'};

TEST_GROUP(WCharacter_isHexadecimalDigit){};

TEST(WCharacter_isHexadecimalDigit, isHexadecimalDigit_01) {
	for (char const c : VALID_HEXADECIMAL_NUMBER_VECT) {
		CHECK(arduino::isHexadecimalDigit(c));
	}
}

TEST(WCharacter_isHexadecimalDigit, isHexadecimalDigit_02) {
	CHECK_FALSE(arduino::isHexadecimalDigit('z'));
}
