/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/String/test_concat.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

TEST_GROUP(String_concat){};

TEST(String_concat, concat_01) {
	arduino::String str1("Hello "), str2("Arduino!");
	CHECK(str1.concat(str2));
	STRCMP_EQUAL("Hello Arduino!", str1.c_str());
}

TEST(String_concat, concat_02) {
	arduino::String str("Hello ");
	CHECK(str.concat("Arduino!"));
	STRCMP_EQUAL("Hello Arduino!", str.c_str());
}

TEST(String_concat, concat_03) {
	arduino::String str("Hello ");
	char const c = 'A';
	CHECK(str.concat(c));
	STRCMP_EQUAL("Hello A", str.c_str());
}

TEST(String_concat, concat_04) {
	arduino::String str("Hello ");
	unsigned char const c = 'A';
	CHECK(str.concat(c));
	STRCMP_EQUAL("Hello 65", str.c_str()); /* ASCII['A'] = 65 */
}

TEST(String_concat, concat_05) {
	arduino::String str("Hello ");
	int const num = -1;
	CHECK(str.concat(num));
	STRCMP_EQUAL("Hello -1", str.c_str());
}

TEST(String_concat, concat_06) {
	arduino::String str("Hello ");
	unsigned int const num = 1;
	CHECK(str.concat(num));
	STRCMP_EQUAL("Hello 1", str.c_str());
}

TEST(String_concat, concat_07) {
	arduino::String str("Hello ");
	long const num = -1;
	CHECK(str.concat(num));
	STRCMP_EQUAL("Hello -1", str.c_str());
}

TEST(String_concat, concat_08) {
	arduino::String str("Hello ");
	unsigned long const num = 1;
	CHECK(str.concat(num));
	STRCMP_EQUAL("Hello 1", str.c_str());
}

TEST(String_concat, concat_09) {
	arduino::String str("Hello ");
	float const num = 1.234f;
	CHECK(str.concat(num));
	STRCMP_EQUAL("Hello 1.23", str.c_str());
}

TEST(String_concat, concat_10) {
	arduino::String str("Hello ");
	double const num = 5.678;
	CHECK(str.concat(num));
	STRCMP_EQUAL("Hello 5.68", str.c_str());
}

TEST(String_concat, concat_11) {
	arduino::String str1("Hello");
	CHECK(str1.concat(F(" Arduino")));
	STRCMP_EQUAL("Hello Arduino", str1.c_str());
}
