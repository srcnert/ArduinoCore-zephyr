/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/String/test_lastIndexOf.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

TEST_GROUP(String_lastIndexOf){};

TEST(String_lastIndexOf, lastIndexOf_01_empty) {
	arduino::String str;
	LONGS_EQUAL(-1, str.lastIndexOf('a'));
}

TEST(String_lastIndexOf, lastIndexOf_01_not_found) {
	arduino::String str("Hello");
	LONGS_EQUAL(-1, str.lastIndexOf('a'));
}

TEST(String_lastIndexOf, lastIndexOf_01_found) {
	arduino::String str("Hellolol");
	LONGS_EQUAL(7, str.lastIndexOf('l'));
}

TEST(String_lastIndexOf, lastIndexOf_02_empty) {
	arduino::String str;
	LONGS_EQUAL(-1, str.lastIndexOf('a', 5));
}

TEST(String_lastIndexOf, lastIndexOf_02_not_found) {
	arduino::String str("Hallo");
	LONGS_EQUAL(-1, str.lastIndexOf('o', 3));
}

TEST(String_lastIndexOf, lastIndexOf_02_found) {
	arduino::String str("Hellolol");
	LONGS_EQUAL(3, str.lastIndexOf('l', 3));
}

TEST(String_lastIndexOf, lastIndexOf_03_empty) {
	arduino::String const search_str("Arduino");
	arduino::String str;
	LONGS_EQUAL(-1, str.lastIndexOf(search_str));
}

TEST(String_lastIndexOf, lastIndexOf_03_not_found) {
	arduino::String const search_str("Arduino");
	arduino::String str("Hallo");
	LONGS_EQUAL(-1, str.lastIndexOf(search_str));
}

TEST(String_lastIndexOf, lastIndexOf_03_found) {
	arduino::String const search_str("Arduino");
	arduino::String str("Hello Arduino, Arduino!");
	LONGS_EQUAL(15, str.lastIndexOf(search_str));
}

TEST(String_lastIndexOf, lastIndexOf_04_empty) {
	arduino::String const search_str("Arduino");
	arduino::String str;
	LONGS_EQUAL(-1, str.lastIndexOf(search_str, 3));
}

TEST(String_lastIndexOf, lastIndexOf_04_not_found) {
	arduino::String const search_str("Arduino");
	arduino::String str("Hallo");
	LONGS_EQUAL(-1, str.lastIndexOf(search_str, 3));
}

TEST(String_lastIndexOf, lastIndexOf_04_from_after_match) {
	arduino::String const search_str("Arduino");
	arduino::String str("Hello Arduino, Arduino!");
	LONGS_EQUAL(15, str.lastIndexOf(search_str, 17));
}

TEST(String_lastIndexOf, lastIndexOf_04_from_before_match) {
	arduino::String const search_str("Arduino");
	arduino::String str("Hello Arduino, Arduino!");
	LONGS_EQUAL(-1, str.lastIndexOf(search_str, 3));
}
