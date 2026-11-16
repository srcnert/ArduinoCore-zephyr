/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/WCharacter/test_isLowerCase.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

TEST_GROUP(WCharacter_isLowerCase){};

TEST(WCharacter_isLowerCase, isLowerCase_01) {
	CHECK_FALSE(arduino::isLowerCase('A'));
}

TEST(WCharacter_isLowerCase, isLowerCase_02) {
	CHECK(arduino::isLowerCase('a'));
}

TEST(WCharacter_isLowerCase, isLowerCase_03) {
	CHECK_FALSE(arduino::isLowerCase('0'));
}
