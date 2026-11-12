/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/String/test_length.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>
#include <string.h>

TEST_GROUP(String_length){};

TEST(String_length, length_01) {
	arduino::String str;
	LONGS_EQUAL(0, str.length());
}

TEST(String_length, length_02) {
	arduino::String str("Testing String::length");
	LONGS_EQUAL(strlen("Testing String::length"), str.length());
}
