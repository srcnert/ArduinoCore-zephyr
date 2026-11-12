/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/String/test_toFloat.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

TEST_GROUP(String_toFloat){};

TEST(String_toFloat, toFloat_01) {
	arduino::String str;
	float const val = str.toFloat();
	DOUBLES_EQUAL(0.0f, val, 0.0);
}

TEST(String_toFloat, toFloat_02) {
	arduino::String str("abc");
	float const val = str.toFloat();
	DOUBLES_EQUAL(0.0f, val, 0.0);
}

TEST(String_toFloat, toFloat_03) {
	arduino::String str("-1.2345");
	float const val = str.toFloat();
	DOUBLES_EQUAL(-1.2345f, val, 0.0);
}
