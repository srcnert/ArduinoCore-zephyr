/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/String/test_operators.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

TEST_GROUP(String_operators){};

TEST(String_operators, operator_plus_01) {
	arduino::String str1("Hello ");
	arduino::String str2("Arduino");
	arduino::String str("Hello Arduino");
	CHECK(str == str1 + str2);
}

TEST(String_operators, operator_plus_02) {
	arduino::String str1("Hello ");
	arduino::String str("Hello Arduino");
	CHECK(str == str1 + "Arduino");
}

TEST(String_operators, operator_plus_03) {
	arduino::String str1("Hello");
	char ch = '!';
	arduino::String str("Hello!");
	CHECK(str == str1 + ch);
}

TEST(String_operators, operator_plus_04) {
	arduino::String str1("Hello ");
	unsigned char ch = 'A';
	arduino::String str("Hello 65"); /* ASCII['A'] = 65 */
	CHECK(str == str1 + ch);
}

TEST(String_operators, operator_plus_05) {
	arduino::String str1("Hello ");
	arduino::String str("Hello 1");
	CHECK(str == str1 + 1);
}

TEST(String_operators, operator_plus_06) {
	arduino::String str1("Hello ");
	unsigned int const num = 1;
	arduino::String str("Hello 1");
	CHECK(str == str1 + num);
}

TEST(String_operators, operator_plus_07) {
	arduino::String str1("Hello ");
	long const num = -1;
	arduino::String str("Hello -1");
	CHECK(str == str1 + num);
}

TEST(String_operators, operator_plus_08) {
	arduino::String str1("Hello ");
	unsigned long const num = 1;
	arduino::String str("Hello 1");
	CHECK(str == str1 + num);
}

TEST(String_operators, operator_plus_09) {
	arduino::String str1("Hello ");
	float const num = 1.234f;
	arduino::String str("Hello 1.23");
	CHECK(str == str1 + num);
}

TEST(String_operators, operator_plus_10) {
	arduino::String str1("Hello ");
	double const num = 5.678;
	arduino::String str("Hello 5.68");
	CHECK(str == str1 + num);
}

TEST(String_operators, operator_plus_11) {
	arduino::String str1("Hello ");
	arduino::String str("Hello Arduino");
	CHECK(str == str1 + F("Arduino"));
}

TEST(String_operators, operator_plus_12) {
	arduino::String str1("Hello ");
	arduino::String str = (str1 + "Arduino");
	STRCMP_EQUAL("Hello Arduino", str.c_str());
}

TEST(String_operators, operator_plus_13) {
	arduino::String str1("Hello");
	char *buffer2 = NULL;
	arduino::String str2(buffer2);
	str1 = str2;
	CHECK(str1 == str2);
}

TEST(String_operators, operator_plus_14_changed) {
	char *buffer = NULL;
	arduino::String str("Hello");
	str = buffer;
	CHECK(str != "Hello");
}

TEST(String_operators, operator_plus_14_invalid) {
	char *buffer = NULL;
	arduino::String str("Hello");
	str = buffer;
	CHECK_FALSE(str);
}

TEST(String_operators, operator_plus_15) {
	char *buffer1 = NULL;
	arduino::String str1(buffer1);
	arduino::String str2("Hello");
	str1 = str2;
	CHECK(str1 == str2);
}

TEST(String_operators, operator_plus_16) {
	arduino::String str("Hello");
	arduino::String str1("Arduino");
	str1 = static_cast<arduino::String &&>(str);
	STRCMP_EQUAL("Hello", str1.c_str());
}

TEST(String_operators, operator_plus_17) {
	arduino::String str("Hello");
	char const ch = '!';
	arduino::String str1("Arduino");
	str1 = static_cast<arduino::StringSumHelper &&>(str + ch);
	STRCMP_EQUAL("Hello!", str1.c_str());
}
