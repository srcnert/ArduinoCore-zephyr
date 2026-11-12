/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/String/test_substring.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

TEST_GROUP(String_substring){};

TEST(String_substring, substring_01_left_past_end) {
	arduino::String str("Hello");
	str.substring(7, 9);
}

TEST(String_substring, substring_01_right_past_end) {
	arduino::String str1("Hello");
	arduino::String str2("ello");
	CHECK(str2 == str1.substring(1, 9));
}

TEST(String_substring, substring_01_left_above_right) {
	arduino::String str1("Hello");
	arduino::String str2("ello");
	CHECK(str2 == str1.substring(9, 1));
}
