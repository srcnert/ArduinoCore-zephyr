/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/String/test_toInt.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

TEST_GROUP(String_toInt){};

TEST(String_toInt, toInt_01) {
	arduino::String str;
	int const val = str.toInt();
	LONGS_EQUAL(0, val);
}

TEST(String_toInt, toInt_02) {
	arduino::String str("abc");
	int const val = str.toInt();
	LONGS_EQUAL(0, val);
}

TEST(String_toInt, toInt_03) {
	arduino::String str("-1");
	int const val = str.toInt();
	LONGS_EQUAL(-1, val);
}

TEST(String_toInt, toInt_04) {
	char *buffer = NULL;
	arduino::String str(buffer);
	LONGS_EQUAL(0, str.toInt());
}
