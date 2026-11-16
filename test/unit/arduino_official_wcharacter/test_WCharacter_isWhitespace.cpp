/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2
 * test/src/WCharacter/test_isWhitespace.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

TEST_GROUP(WCharacter_isWhitespace){};

TEST(WCharacter_isWhitespace, isWhitespace_01) {
	CHECK(arduino::isWhitespace(' '));
}

TEST(WCharacter_isWhitespace, isWhitespace_02) {
	CHECK(arduino::isWhitespace('\t'));
}

TEST(WCharacter_isWhitespace, isWhitespace_03) {
	CHECK_FALSE(arduino::isWhitespace('\r'));
}
