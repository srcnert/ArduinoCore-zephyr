/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/IPAddress/test_printTo6.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

#include "PrintMock.h"

TEST_GROUP(IPAddress_printTo6) {
	PrintMock mock;
};

TEST(IPAddress_printTo6, printTo6_01) {
	arduino::IPAddress ip(0x20, 0x01, 0xd, 0xb8, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb, 0xc);
	mock.print(ip);
	STRCMP_EQUAL("2001:db8:102:304:506:708:90a:b0c", mock._str);
}

TEST(IPAddress_printTo6, printTo6_02_any) {
	arduino::IPAddress const ip(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	mock.print(ip);
	STRCMP_EQUAL("::", mock._str);
}

TEST(IPAddress_printTo6, printTo6_03_localhost) {
	arduino::IPAddress const ip(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1);
	mock.print(ip);
	STRCMP_EQUAL("::1", mock._str);
}

TEST(IPAddress_printTo6, printTo6_04_segment_lengths) {
	arduino::IPAddress const ip(0xab, 0xcd, 0x0e, 0xf1, 0x00, 0x23, 0, 0, 0x00, 0x04, 0, 0, 0, 0, 0,
								0);
	mock.print(ip);
	STRCMP_EQUAL("abcd:ef1:23:0:4::", mock._str);
}

TEST(IPAddress_printTo6, printTo6_05_longest_run_end) {
	arduino::IPAddress const ip(0, 0, 0, 1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0);
	mock.print(ip);
	STRCMP_EQUAL("0:1:0:0:2::", mock._str);
}

TEST(IPAddress_printTo6, printTo6_06_longest_run_mid) {
	arduino::IPAddress const ip(0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0);
	mock.print(ip);
	STRCMP_EQUAL("0:1::2:0:0", mock._str);
}

TEST(IPAddress_printTo6, printTo6_07_start_zero) {
	arduino::IPAddress const ip(0, 0, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7, 0, 8);
	mock.print(ip);
	STRCMP_EQUAL("0:2:3:4:5:6:7:8", mock._str);
}

TEST(IPAddress_printTo6, printTo6_08_ending_zero) {
	arduino::IPAddress const ip(0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7, 0, 0);
	mock.print(ip);
	STRCMP_EQUAL("1:2:3:4:5:6:7:0", mock._str);
}

TEST(IPAddress_printTo6, printTo6_09_start_two_zero) {
	arduino::IPAddress const ip(0, 0, 0, 0, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7, 0, 0);
	mock.print(ip);
	STRCMP_EQUAL("::3:4:5:6:7:0", mock._str);
}

TEST(IPAddress_printTo6, printTo6_10_ending_two_zero) {
	arduino::IPAddress const ip(0, 0, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 0, 0, 0);
	mock.print(ip);
	STRCMP_EQUAL("0:2:3:4:5:6::", mock._str);
}

TEST(IPAddress_printTo6, printTo6_11_first_of_same_length) {
	arduino::IPAddress const ip(0, 1, 0, 0, 0, 0, 0, 4, 0, 5, 0, 0, 0, 0, 0, 8);
	mock.print(ip);
	STRCMP_EQUAL("1::4:5:0:0:8", mock._str);
}

TEST(IPAddress_printTo6, printTo6_12_single_zeros) {
	arduino::IPAddress const ip(0, 1, 0, 0, 0, 3, 0, 0, 0, 5, 0, 0, 0, 7, 0, 8);
	mock.print(ip);
	STRCMP_EQUAL("1:0:3:0:5:0:7:8", mock._str);
}
