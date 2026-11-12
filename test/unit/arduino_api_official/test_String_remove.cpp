/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/String/test_remove.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

TEST_GROUP(String_remove){};

TEST(String_remove, remove_01) {
	arduino::String str;
	str.remove(0);
	LONGS_EQUAL(0, str.length());
}

TEST(String_remove, remove_02) {
	arduino::String str("Hello Arduino!");
	str.remove(100);
	STRCMP_EQUAL("Hello Arduino!", str.c_str());
}

TEST(String_remove, remove_03) {
	arduino::String str("Hello Arduino!");
	str.remove(5);
	STRCMP_EQUAL("Hello", str.c_str());
}

TEST(String_remove, remove_04) {
	arduino::String str;
	str.remove(0, 10);
	LONGS_EQUAL(0, str.length());
}

TEST(String_remove, remove_05) {
	arduino::String str("Hello Arduino!");
	str.remove(100, 10);
	STRCMP_EQUAL("Hello Arduino!", str.c_str());
}

TEST(String_remove, remove_06) {
	arduino::String str("Hello Arduino!");
	str.remove(5, 100);
	STRCMP_EQUAL("Hello", str.c_str());
}

TEST(String_remove, remove_07) {
	arduino::String str("Hello Arduino!");
	str.remove(5, 1);
	STRCMP_EQUAL("HelloArduino!", str.c_str());
}
