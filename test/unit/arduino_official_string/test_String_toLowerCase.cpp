/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/String/test_toLowerCase.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

TEST_GROUP(String_toLowerCase){};

TEST(String_toLowerCase, toLowerCase_01) {
	arduino::String str("HELLO ARDUINO");
	str.toLowerCase();
	STRCMP_EQUAL("hello arduino", str.c_str());
}
