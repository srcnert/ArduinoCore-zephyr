/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/String/test_toDouble.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

TEST_GROUP(String_toDouble){};

TEST(String_toDouble, toDouble_01) {
	arduino::String str;
	double const val = str.toDouble();
	DOUBLES_EQUAL(0.0, val, 0.0);
}

TEST(String_toDouble, toDouble_02) {
	arduino::String str("abc");
	double const val = str.toDouble();
	DOUBLES_EQUAL(0.0, val, 0.0);
}

TEST(String_toDouble, toDouble_03) {
	arduino::String str("-1.2345");
	double const val = str.toDouble();
	DOUBLES_EQUAL(-1.2345, val, 0.0);
}

TEST(String_toDouble, toDouble_04) {
	char *buffer = NULL;
	arduino::String str(buffer);
	DOUBLES_EQUAL(0.0, str.toDouble(), 0.0);
}
