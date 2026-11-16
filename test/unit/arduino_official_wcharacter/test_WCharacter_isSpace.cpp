/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/WCharacter/test_isSpace.cpp
 * with an array and a for loop instead of std::vector and std::for_each
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

char const VALID_SPACE_VECT[] = {' ', '\t', '\n', '\v', '\f', '\r'};

TEST_GROUP(WCharacter_isSpace){};

TEST(WCharacter_isSpace, isSpace_01) {
	for (char const c : VALID_SPACE_VECT) {
		CHECK(arduino::isSpace(c));
	}
}

TEST(WCharacter_isSpace, isSpace_02) {
	CHECK_FALSE(arduino::isSpace('{'));
}
