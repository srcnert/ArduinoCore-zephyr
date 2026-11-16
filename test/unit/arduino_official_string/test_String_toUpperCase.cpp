/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/String/test_toUpperCase.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

TEST_GROUP(String_toUpperCase){};

TEST(String_toUpperCase, toUpperCase_01) {
	arduino::String str("hello arduino");
	str.toUpperCase();
	STRCMP_EQUAL("HELLO ARDUINO", str.c_str());
}
