/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/Print/test_println.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

#include "PrintMock.h"
#include "PrintableMock.h"

TEST_GROUP(Print_println) {
	PrintMock mock;
};

TEST(Print_println, println_01) {
	mock.println('A');
	STRCMP_EQUAL("A\r\n", mock._str);
}

TEST(Print_println, println_02) {
	arduino::String const str("Test String");
	mock.println(str);
	STRCMP_EQUAL("Test String\r\n", mock._str);
}

TEST(Print_println, println_03) {
	const char str[] = "Test String";
	mock.println(str);
	STRCMP_EQUAL("Test String\r\n", mock._str);
}

TEST(Print_println, println_04) {
	int const val = -1;
	mock.println(val);
	STRCMP_EQUAL("-1\r\n", mock._str);
}

TEST(Print_println, println_05) {
	unsigned int const val = 17;
	mock.println(val);
	STRCMP_EQUAL("17\r\n", mock._str);
}

TEST(Print_println, println_06) {
	long const val = -1;
	mock.println(val);
	STRCMP_EQUAL("-1\r\n", mock._str);
}

TEST(Print_println, println_07) {
	unsigned long const val = 17;
	mock.println(val);
	STRCMP_EQUAL("17\r\n", mock._str);
}

TEST(Print_println, println_08) {
	long long const val = -1;
	mock.println(val);
	STRCMP_EQUAL("-1\r\n", mock._str);
}

TEST(Print_println, println_09) {
	unsigned long long const val = 17;
	mock.println(val);
	STRCMP_EQUAL("17\r\n", mock._str);
}

TEST(Print_println, println_10) {
	double const val = 3.1459;
	mock.println(val);
	STRCMP_EQUAL("3.15\r\n", mock._str);
}

TEST(Print_println, println_11) {
	PrintableMock printable;
	printable._i = 1;
	mock.println(printable);
	STRCMP_EQUAL("PrintableMock i = 1\r\n", mock._str);
}

TEST(Print_println, println_12) {
	mock.println('A', DEC);
	STRCMP_EQUAL("65\r\n", mock._str);
}

TEST(Print_println, println_13) {
	mock.println(F("Hello flash string"));
	STRCMP_EQUAL("Hello flash string\r\n", mock._str);
}
