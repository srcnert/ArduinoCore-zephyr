/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2
 * test/src/IPAddress/test_operator_comparison.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

TEST_GROUP(IPAddress_operator_comparison){};

TEST(IPAddress_operator_comparison, equal_01_same) {
	arduino::IPAddress ip1(129, 168, 1, 2), ip2(129, 168, 1, 2);
	CHECK(ip1 == ip2);
}

TEST(IPAddress_operator_comparison, equal_02_different) {
	arduino::IPAddress ip1(129, 168, 1, 2), ip2(10, 0, 0, 1);
	CHECK_FALSE(ip1 == ip2);
}

TEST(IPAddress_operator_comparison, equal_03_bytes_same) {
	arduino::IPAddress ip1(129, 168, 1, 2);
	uint8_t const ip2[] = {129, 168, 1, 2};
	CHECK(ip1 == ip2);
}

TEST(IPAddress_operator_comparison, equal_04_bytes_different) {
	arduino::IPAddress ip1(129, 168, 1, 2);
	uint8_t const ip2[] = {10, 0, 0, 1};
	CHECK_FALSE(ip1 == ip2);
}

TEST(IPAddress_operator_comparison, not_equal_05_different) {
	arduino::IPAddress ip1(129, 168, 1, 2), ip2(10, 0, 0, 1);
	CHECK(ip1 != ip2);
}

TEST(IPAddress_operator_comparison, not_equal_05_same) {
	arduino::IPAddress ip1(129, 168, 1, 2), ip2(129, 168, 1, 2);
	CHECK_FALSE(ip1 != ip2);
}
