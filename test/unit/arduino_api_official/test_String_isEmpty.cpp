/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/String/test_isEmpty.cpp
 * Upstream's test/CMakeLists.txt does not build this file.
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

TEST_GROUP(String_isEmpty){};

TEST(String_isEmpty, isEmpty_01) {
	arduino::String str;
	CHECK(str.isEmpty());
}

TEST(String_isEmpty, isEmpty_02) {
	arduino::String str("Testing String::isEmpty");
	CHECK(!str.isEmpty());
}
