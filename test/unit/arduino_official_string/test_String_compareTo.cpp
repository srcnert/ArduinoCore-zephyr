/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/String/test_compareTo.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

TEST_GROUP(String_compareTo){};

TEST(String_compareTo, compareTo_01_equal) {
	arduino::String str1("Hello"), str2("Hello");
	LONGS_EQUAL(0, str1.compareTo(str2));
}

TEST(String_compareTo, compareTo_01_str2_empty) {
	arduino::String str1("Hello"), str2;
	CHECK(str1.compareTo(str2) > 0);
}

TEST(String_compareTo, compareTo_01_str1_empty) {
	arduino::String str1, str2("Hello");
	CHECK(str1.compareTo(str2) < 0);
}

TEST(String_compareTo, compareTo_02_equal) {
	arduino::String str("Hello");
	LONGS_EQUAL(0, str.compareTo("Hello"));
}

TEST(String_compareTo, compareTo_02_passed_empty) {
	arduino::String str("Hello");
	CHECK(str.compareTo("") > 0);
}

TEST(String_compareTo, compareTo_02_both_empty) {
	arduino::String str;
	LONGS_EQUAL(0, str.compareTo(""));
}

TEST(String_compareTo, compareTo_03_valid_buffer) {
	char *buffer = NULL;
	arduino::String str("Hello");
	CHECK(str.compareTo(buffer) != 0);
}

TEST(String_compareTo, compareTo_03_no_buffer) {
	char *buffer1 = NULL;
	char *buffer2 = NULL;
	arduino::String str(buffer1);
	LONGS_EQUAL(0, str.compareTo(buffer2));
}

TEST(String_compareTo, compareTo_04_valid_buffer) {
	char *buffer = NULL;
	arduino::String str1("Hello");
	arduino::String str2(buffer);
	CHECK(str1.compareTo(str2) != 0);
}

TEST(String_compareTo, compareTo_04_no_buffer) {
	char *buffer1 = NULL;
	char *buffer2 = NULL;
	arduino::String str1(buffer1);
	arduino::String str2(buffer2);
	LONGS_EQUAL(0, str1.compareTo(str2));
}
