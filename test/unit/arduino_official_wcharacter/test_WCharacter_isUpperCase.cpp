/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/WCharacter/test_isUpperCase.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

TEST_GROUP(WCharacter_isUpperCase){};

TEST(WCharacter_isUpperCase, isUpperCase_01) {
	CHECK(arduino::isUpperCase('A'));
}

TEST(WCharacter_isUpperCase, isUpperCase_02) {
	CHECK_FALSE(arduino::isUpperCase('a'));
}

TEST(WCharacter_isUpperCase, isUpperCase_03) {
	CHECK_FALSE(arduino::isUpperCase('0'));
}
