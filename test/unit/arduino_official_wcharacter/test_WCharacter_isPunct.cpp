/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/WCharacter/test_isPunct.cpp
 * with an array and a for loop instead of std::vector and std::for_each
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

char const VALID_PUNCT_VECT[] = {'!', '"',  '#', '$', '%', '&', '\'', '(', ')', '*', '+',
								 ',', '-',  '.', '/', ':', ';', '<',  '=', '>', '?', '@',
								 '[', '\\', ']', '^', '_', '`', '{',  '|', '}', '~'};

TEST_GROUP(WCharacter_isPunct){};

TEST(WCharacter_isPunct, isPunct_01) {
	for (char const c : VALID_PUNCT_VECT) {
		CHECK(arduino::isPunct(c));
	}
}

TEST(WCharacter_isPunct, isPunct_02) {
	CHECK_FALSE(arduino::isPunct('a'));
}
