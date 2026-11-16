/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/String/test_trim.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

TEST_GROUP(String_trim){};

TEST(String_trim, trim_01) {
	arduino::String str("  hello");
	str.trim();
	STRCMP_EQUAL("hello", str.c_str());
}

TEST(String_trim, trim_02) {
	arduino::String str("hello  ");
	str.trim();
	STRCMP_EQUAL("hello", str.c_str());
}

TEST(String_trim, trim_03) {
	arduino::String str("  hello  ");
	str.trim();
	STRCMP_EQUAL("hello", str.c_str());
}

TEST(String_trim, trim_04) {
	arduino::String str("Hello Arduino!");
	str.trim();
	STRCMP_EQUAL("Hello Arduino!", str.c_str());
}
