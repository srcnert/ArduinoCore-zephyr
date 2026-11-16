/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2
 * test/src/String/test_characterAccessFunc.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

TEST_GROUP(String_characterAccessFunc){};

TEST(String_characterAccessFunc, charAt_01) {
	arduino::String str1("Hello");
	LONGS_EQUAL('l', str1.charAt(2));
}

TEST(String_characterAccessFunc, setCharAt_02) {
	arduino::String str1("Hello");
	str1.setCharAt(1, 'a');
	STRCMP_EQUAL("Hallo", str1.c_str());
}

TEST(String_characterAccessFunc, getBytes_02_no_bufsize) {
	arduino::String str("Hello");
	unsigned char buf[2];
	str.getBytes(buf, 0, 0);
}

TEST(String_characterAccessFunc, getBytes_02_index_past_end) {
	arduino::String str("Hello");
	unsigned char buf[2];
	str.getBytes(buf, 5, 6);
}

TEST(String_characterAccessFunc, getBytes_02_valid) {
	arduino::String str("Hello");
	unsigned char buf[3];
	str.getBytes(buf, 5, 3);
	LONGS_EQUAL('l', buf[0]);
	LONGS_EQUAL('o', buf[1]);
	LONGS_EQUAL('\0', buf[2]);
}

TEST(String_characterAccessFunc, subscript_03) {
	arduino::String str("Hello");
	str[0] = 'M';
	STRCMP_EQUAL("Mello", str.c_str());
}

TEST(String_characterAccessFunc, subscript_04_invalid_buffer) {
	char *buffer = NULL;
	arduino::String str(buffer);
	str[0] = 'M';
	LONGS_EQUAL(0, str[0]);
}
