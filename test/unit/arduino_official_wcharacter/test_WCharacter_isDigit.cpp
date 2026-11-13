/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/WCharacter/test_isDigit.cpp
 * with an array and a for loop instead of std::vector and std::for_each
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

char const VALID_DIGIT_VECT[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

TEST_GROUP(WCharacter_isDigit){};

TEST(WCharacter_isDigit, isDigit_01) {
	for (char const c : VALID_DIGIT_VECT) {
		CHECK(arduino::isDigit(c));
	}
}

TEST(WCharacter_isDigit, isDigit_02) {
	CHECK_FALSE(arduino::isDigit('z'));
}
