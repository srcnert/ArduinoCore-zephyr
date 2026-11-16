/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/Print/test_print.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>
#include <math.h>

#include "PrintMock.h"
#include "PrintableMock.h"

TEST_GROUP(Print_print) {
	PrintMock mock;
};

TEST(Print_print, print_01) {
	mock.print('A');
	STRCMP_EQUAL("A", mock._str);
}

TEST(Print_print, print_02) {
	arduino::String const str("Test String");
	mock.print(str);
	STRCMP_EQUAL("Test String", mock._str);
}

TEST(Print_print, print_03) {
	const char str[] = "Test String";
	mock.print(str);
	STRCMP_EQUAL("Test String", mock._str);
}

// Upstream runs on a 64-bit host. int and long print through unsigned long, which has 32 bits
// on these targets, so -1 prints as 32 ones in print_04 and print_06.

TEST(Print_print, print_04_dec) {
	int const val = -1;
	mock.print(val, DEC);
	STRCMP_EQUAL("-1", mock._str);
}

TEST(Print_print, print_04_hex) {
	int const val = -1;
	mock.print(val, HEX);
	STRCMP_EQUAL("FFFFFFFF", mock._str);
}

TEST(Print_print, print_04_oct) {
	int const val = -1;
	mock.print(val, OCT);
	STRCMP_EQUAL("37777777777", mock._str);
}

TEST(Print_print, print_04_bin) {
	int const val = -1;
	mock.print(val, BIN);
	STRCMP_EQUAL("11111111111111111111111111111111", mock._str);
}

TEST(Print_print, print_05_dec) {
	unsigned int const val = 17;
	mock.print(val, DEC);
	STRCMP_EQUAL("17", mock._str);
}

TEST(Print_print, print_05_hex) {
	unsigned int const val = 17;
	mock.print(val, HEX);
	STRCMP_EQUAL("11", mock._str);
}

TEST(Print_print, print_05_oct) {
	unsigned int const val = 17;
	mock.print(val, OCT);
	STRCMP_EQUAL("21", mock._str);
}

TEST(Print_print, print_05_bin) {
	unsigned int const val = 17;
	mock.print(val, BIN);
	STRCMP_EQUAL("10001", mock._str);
}

TEST(Print_print, print_06_dec) {
	long const val = -1;
	mock.print(val, DEC);
	STRCMP_EQUAL("-1", mock._str);
}

TEST(Print_print, print_06_hex) {
	long const val = -1;
	mock.print(val, HEX);
	STRCMP_EQUAL("FFFFFFFF", mock._str);
}

TEST(Print_print, print_06_oct) {
	long const val = -1;
	mock.print(val, OCT);
	STRCMP_EQUAL("37777777777", mock._str);
}

TEST(Print_print, print_06_bin) {
	long const val = -1;
	mock.print(val, BIN);
	STRCMP_EQUAL("11111111111111111111111111111111", mock._str);
}

TEST(Print_print, print_07_dec) {
	unsigned long const val = 17;
	mock.print(val, DEC);
	STRCMP_EQUAL("17", mock._str);
}

TEST(Print_print, print_07_hex) {
	unsigned long const val = 17;
	mock.print(val, HEX);
	STRCMP_EQUAL("11", mock._str);
}

TEST(Print_print, print_07_oct) {
	unsigned long const val = 17;
	mock.print(val, OCT);
	STRCMP_EQUAL("21", mock._str);
}

TEST(Print_print, print_07_bin) {
	unsigned long const val = 17;
	mock.print(val, BIN);
	STRCMP_EQUAL("10001", mock._str);
}

TEST(Print_print, print_08_dec) {
	long long const val = -1;
	mock.print(val, DEC);
	STRCMP_EQUAL("-1", mock._str);
}

TEST(Print_print, print_08_hex) {
	long long const val = -1;
	mock.print(val, HEX);
	STRCMP_EQUAL("FFFFFFFFFFFFFFFF", mock._str);
}

TEST(Print_print, print_08_oct) {
	long long const val = -1;
	mock.print(val, OCT);
	STRCMP_EQUAL("1777777777777777777777", mock._str);
}

TEST(Print_print, print_08_bin) {
	long long const val = -1;
	mock.print(val, BIN);
	STRCMP_EQUAL("1111111111111111111111111111111111111111111111111111111111111111", mock._str);
}

TEST(Print_print, print_09_zero_dec) {
	unsigned long long const val = 0;
	mock.print(val, DEC);
	STRCMP_EQUAL("0", mock._str);
}

TEST(Print_print, print_09_zero_hex) {
	unsigned long long const val = 0;
	mock.print(val, HEX);
	STRCMP_EQUAL("0", mock._str);
}

TEST(Print_print, print_09_zero_oct) {
	unsigned long long const val = 0;
	mock.print(val, OCT);
	STRCMP_EQUAL("0", mock._str);
}

TEST(Print_print, print_09_zero_bin) {
	unsigned long long const val = 0;
	mock.print(val, BIN);
	STRCMP_EQUAL("0", mock._str);
}

TEST(Print_print, print_09_nonzero_dec) {
	unsigned long long const val = 17;
	mock.print(val, DEC);
	STRCMP_EQUAL("17", mock._str);
}

TEST(Print_print, print_09_nonzero_hex) {
	unsigned long long const val = 17;
	mock.print(val, HEX);
	STRCMP_EQUAL("11", mock._str);
}

TEST(Print_print, print_09_nonzero_oct) {
	unsigned long long const val = 17;
	mock.print(val, OCT);
	STRCMP_EQUAL("21", mock._str);
}

TEST(Print_print, print_09_nonzero_bin) {
	unsigned long long const val = 17;
	mock.print(val, BIN);
	STRCMP_EQUAL("10001", mock._str);
}

TEST(Print_print, print_10_positive_digits_0) {
	double const val = 3.1459;
	mock.print(val, 0);
	STRCMP_EQUAL("3", mock._str);
}

TEST(Print_print, print_10_positive_digits_1) {
	double const val = 3.1459;
	mock.print(val, 1);
	STRCMP_EQUAL("3.1", mock._str);
}

TEST(Print_print, print_10_positive_digits_2_default) {
	double const val = 3.1459;
	mock.print(val);
	STRCMP_EQUAL("3.15", mock._str);
}

TEST(Print_print, print_10_positive_digits_3) {
	double const val = 3.1459;
	mock.print(val, 3);
	STRCMP_EQUAL("3.146", mock._str);
}

TEST(Print_print, print_10_positive_digits_4) {
	double const val = 3.1459;
	mock.print(val, 4);
	STRCMP_EQUAL("3.1459", mock._str);
}

TEST(Print_print, print_10_positive_digits_5) {
	double const val = 3.1459;
	mock.print(val, 5);
	STRCMP_EQUAL("3.14590", mock._str);
}

TEST(Print_print, print_10_negative_digits) {
	double const val = 3.1459;
	mock.print(val, -1);
	STRCMP_EQUAL("3.15", mock._str);
}

TEST(Print_print, print_10_negative_value) {
	double const val = -3.1459;
	mock.print(val);
	STRCMP_EQUAL("-3.15", mock._str);
}

// NAN and INFINITY are float; the casts keep -Wdouble-promotion quiet

TEST(Print_print, print_10_nan) {
	mock.print(static_cast<double>(NAN));
	STRCMP_EQUAL("nan", mock._str);
}

TEST(Print_print, print_10_infinity) {
	mock.print(static_cast<double>(INFINITY));
	STRCMP_EQUAL("inf", mock._str);
}

TEST(Print_print, print_11) {
	PrintableMock printable;
	printable._i = 1;
	mock.print(printable);
	STRCMP_EQUAL("PrintableMock i = 1", mock._str);
}

TEST(Print_print, print_12_dec) {
	mock.print('A', DEC);
	STRCMP_EQUAL("65", mock._str);
}

TEST(Print_print, print_12_hex) {
	mock.print('A', HEX);
	STRCMP_EQUAL("41", mock._str);
}

TEST(Print_print, print_12_oct) {
	mock.print('A', OCT);
	STRCMP_EQUAL("101", mock._str);
}

TEST(Print_print, print_12_bin) {
	mock.print('A', BIN);
	STRCMP_EQUAL("1000001", mock._str);
}

TEST(Print_print, print_13) {
	mock.print(F("Hello flash string"));
	STRCMP_EQUAL("Hello flash string", mock._str);
}
