/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/String/test_move.cpp
 * Zephyr's minimal C++ library has no <utility>, so std::move() is written as a cast.
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

TEST_GROUP(String_move){};

TEST(String_move, move_01) {
	arduino::String a("src");
	char const *const a_str = a.c_str();
	arduino::String b(static_cast<arduino::String &&>(a));
	LONGS_EQUAL(0, a.length());
	POINTERS_EQUAL(NULL, a.c_str());
	POINTERS_EQUAL(a_str, b.c_str());
	LONGS_EQUAL(3, b.length());
}

TEST(String_move, move_02) {
	arduino::String a("src");
	char const *const a_str = a.c_str();
	arduino::String b;
	b = static_cast<arduino::String &&>(a);
	LONGS_EQUAL(0, a.length());
	POINTERS_EQUAL(NULL, a.c_str());
	CHECK(b == arduino::String("src"));
	POINTERS_EQUAL(a_str, b.c_str());
}

TEST(String_move, move_03_self) {
	arduino::String a("src");
	a = static_cast<arduino::String &&>(a);
	STRCMP_EQUAL("src", a.c_str());
}
