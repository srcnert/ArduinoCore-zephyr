/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/String/test_replace.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

TEST_GROUP(String_replace){};

TEST(String_replace, replace_01) {
	arduino::String str;
	str.replace('a', 'b');
	LONGS_EQUAL(0, str.length());
}

TEST(String_replace, replace_02) {
	arduino::String str("Hello Arduino!");
	str.replace('Z', '0');
	STRCMP_EQUAL("Hello Arduino!", str.c_str());
}

TEST(String_replace, replace_03) {
	arduino::String str("Hello Arduino!");
	str.replace('o', '0');
	str.replace('e', '3');
	str.replace('i', '1');
	STRCMP_EQUAL("H3ll0 Ardu1n0!", str.c_str());
}

TEST(String_replace, replace_04) {
	arduino::String str("Hello Arduino!");
	str.replace(arduino::String("Zulu"), arduino::String("11"));
	STRCMP_EQUAL("Hello Arduino!", str.c_str());
}

TEST(String_replace, replace_05) {
	arduino::String str("Hello Arduino!");
	str.replace(arduino::String("ll"), arduino::String("11"));
	STRCMP_EQUAL("He11o Arduino!", str.c_str());
}

TEST(String_replace, replace_06) {
	arduino::String str("Hello Arduino!");
	str.replace(arduino::String("llo"), arduino::String("11"));
	STRCMP_EQUAL("He11 Arduino!", str.c_str());
}

TEST(String_replace, replace_07) {
	arduino::String str("Hello Arduino!");
	str.replace(arduino::String("ll"), arduino::String("111"));
	STRCMP_EQUAL("He111o Arduino!", str.c_str());
}

TEST(String_replace, replace_08) {
	arduino::String str("Hello Arduino! Hello, Hello, Hello");
	str.replace(arduino::String("ll"), arduino::String("lll"));
	STRCMP_EQUAL("Helllo Arduino! Helllo, Helllo, Helllo", str.c_str());
}

TEST(String_replace, replace_09) {
	arduino::String str("Hello Arduino! Hello, Hello, Hello");
	str.replace(arduino::String("ll"), arduino::String("11"));
	STRCMP_EQUAL("He11o Arduino! He11o, He11o, He11o", str.c_str());
}

TEST(String_replace, replace_10) {
	arduino::String str("Helllo Arduino! Helllo, Helllo, Helllo");
	str.replace(arduino::String("lll"), arduino::String("ll"));
	STRCMP_EQUAL("Hello Arduino! Hello, Hello, Hello", str.c_str());
}
