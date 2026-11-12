/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/String/test_indexOf.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

TEST_GROUP(String_indexOf){};

TEST(String_indexOf, indexOf_01_empty) {
	arduino::String str;
	LONGS_EQUAL(-1, str.indexOf('a'));
}

TEST(String_indexOf, indexOf_01_not_found) {
	arduino::String str("Hello");
	LONGS_EQUAL(-1, str.indexOf('a'));
}

TEST(String_indexOf, indexOf_01_found) {
	arduino::String str("Hello");
	LONGS_EQUAL(2, str.indexOf('l'));
}

TEST(String_indexOf, indexOf_02_empty) {
	arduino::String str;
	LONGS_EQUAL(-1, str.indexOf('a', 5));
}

TEST(String_indexOf, indexOf_02_not_found) {
	arduino::String str("Hallo");
	LONGS_EQUAL(-1, str.indexOf('a', 3));
}

TEST(String_indexOf, indexOf_02_found) {
	arduino::String str("Hello");
	LONGS_EQUAL(3, str.indexOf('l', 3));
}

TEST(String_indexOf, indexOf_03_empty) {
	arduino::String const search_str("Arduino");
	arduino::String str;
	LONGS_EQUAL(-1, str.indexOf(search_str));
}

TEST(String_indexOf, indexOf_03_not_found) {
	arduino::String const search_str("Arduino");
	arduino::String str("Hallo");
	LONGS_EQUAL(-1, str.indexOf(search_str));
}

TEST(String_indexOf, indexOf_03_found) {
	arduino::String const search_str("Arduino");
	arduino::String str("Hello Arduino!");
	LONGS_EQUAL(6, str.indexOf(search_str));
}

TEST(String_indexOf, indexOf_04_empty) {
	arduino::String const search_str("Arduino");
	arduino::String str;
	LONGS_EQUAL(-1, str.indexOf(search_str, 3));
}

TEST(String_indexOf, indexOf_04_not_found) {
	arduino::String const search_str("Arduino");
	arduino::String str("Hallo");
	LONGS_EQUAL(-1, str.indexOf(search_str, 3));
}

TEST(String_indexOf, indexOf_04_from_before_match) {
	arduino::String const search_str("Arduino");
	arduino::String str("Hello Arduino!");
	LONGS_EQUAL(6, str.indexOf(search_str, 3));
}

TEST(String_indexOf, indexOf_04_from_after_match) {
	arduino::String const search_str("Arduino");
	arduino::String str("Hello Arduino!");
	LONGS_EQUAL(-1, str.indexOf(search_str, 8));
}
