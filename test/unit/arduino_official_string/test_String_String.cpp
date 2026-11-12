/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/String/test_String.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>
#include <float.h>

// picolibc's printf prints at most 17 significant digits and fills the rest with zeros, where
// upstream expects glibc's exact digits; C allows both.
static arduino::String picolibc_digits(const char *digits, unsigned int zeros) {
	arduino::String s(digits);

	for (unsigned int i = 0; i < zeros; i++) {
		s += '0';
	}
	s += ".00";
	return s;
}

TEST_GROUP(String_String){};

TEST(String_String, Ctor_01) {
	char const CSTR[] = "Hello Arduino String Class";
	arduino::String str(CSTR);
	STRCMP_EQUAL(CSTR, str.c_str());
}

TEST(String_String, Ctor_02) {
	arduino::String str1("Hello Arduino String class"), str2(str1);
	CHECK(str1 == str2);
}

TEST(String_String, Ctor_03) {
	arduino::String str1(F("Hello"));
	STRCMP_EQUAL("Hello", str1.c_str());
}

TEST(String_String, Ctor_04) {
	char const ch = 'A';
	arduino::String str(ch);
	STRCMP_EQUAL("A", str.c_str());
}

TEST(String_String, Ctor_05) {
	unsigned char const val = 1;
	arduino::String str(val);
	STRCMP_EQUAL("1", str.c_str());
}

TEST(String_String, Ctor_06) {
	int const val = -1;
	arduino::String str(val);
	STRCMP_EQUAL("-1", str.c_str());
}

TEST(String_String, Ctor_07) {
	unsigned int const val = 1;
	arduino::String str(val);
	STRCMP_EQUAL("1", str.c_str());
}

TEST(String_String, Ctor_08) {
	long const val = -1;
	arduino::String str(val);
	STRCMP_EQUAL("-1", str.c_str());
}

TEST(String_String, Ctor_09) {
	unsigned long const val = 1;
	arduino::String str(val);
	STRCMP_EQUAL("1", str.c_str());
}

TEST(String_String, Ctor_10_float) {
	arduino::String str(1.234f);
	STRCMP_EQUAL("1.23", str.c_str());
}

TEST(String_String, Ctor_10_flt_max) {
	arduino::String str(FLT_MAX);
	STRCMP_EQUAL(picolibc_digits("34028234663852886", 22).c_str(), str.c_str());
}

TEST(String_String, Ctor_10_neg_flt_max) {
	arduino::String str(-FLT_MAX);
	STRCMP_EQUAL(picolibc_digits("-34028234663852886", 22).c_str(), str.c_str());
}

TEST(String_String, Ctor_11_double) {
	arduino::String str(5.678);
	STRCMP_EQUAL("5.68", str.c_str());
}

TEST(String_String, Ctor_11_dbl_max) {
	arduino::String str(DBL_MAX);
	STRCMP_EQUAL(picolibc_digits("17976931348623157", 292).c_str(), str.c_str());
}

TEST(String_String, Ctor_11_neg_dbl_max) {
	arduino::String str(-DBL_MAX);
	STRCMP_EQUAL(picolibc_digits("-17976931348623157", 292).c_str(), str.c_str());
}

TEST(String_String, Ctor_12_invalid_buffer) {
	char *buffer = NULL;
	arduino::String str1(F(buffer));
	CHECK_FALSE(str1);
}

TEST(String_String, Ctor_13) {
	arduino::String str("Hello");
	char const ch = '!';
	arduino::String str1(static_cast<arduino::StringSumHelper &&>(str + ch));
	STRCMP_EQUAL("Hello!", str1.c_str());
}

TEST(String_String, Ctor_14) {
	arduino::String str("Hello");
	arduino::String str1(static_cast<arduino::String &&>(str));
	STRCMP_EQUAL("Hello", str1.c_str());
}

TEST(String_String, Ctor_15) {
	arduino::String str("Hello");
	arduino::String str1("Arduino");
	str1 = static_cast<arduino::String &&>(str);
	STRCMP_EQUAL("Hello", str1.c_str());
}

TEST(String_String, Ctor_16) {
	arduino::String str("Hello");
	arduino::String str1("Arduino");
	str = static_cast<arduino::String &&>(str1);
	STRCMP_EQUAL("Arduino", str.c_str());
}
