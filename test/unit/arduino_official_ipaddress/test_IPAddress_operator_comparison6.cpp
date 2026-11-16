/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2
 * test/src/IPAddress/test_operator_comparison6.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

TEST_GROUP(IPAddress_operator_comparison6){};

TEST(IPAddress_operator_comparison6, equal6_01_same) {
	arduino::IPAddress ip1(0x20, 0x01, 0xd, 0xb8, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb, 0xc),
		ip2(0x20, 0x01, 0xd, 0xb8, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb, 0xc);
	CHECK(ip1 == ip2);
}

TEST(IPAddress_operator_comparison6, equal6_02_different) {
	arduino::IPAddress ip1(0x20, 0x01, 0xd, 0xb8, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb, 0xc),
		ip2(0xfd, 0x12, 0x34, 0x56, 0x78, 0x9a, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1);
	CHECK_FALSE(ip1 == ip2);
}

TEST(IPAddress_operator_comparison6, not_equal6_03_different) {
	arduino::IPAddress ip1(0x20, 0x01, 0xd, 0xb8, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb, 0xc),
		ip2(0xfd, 0x12, 0x34, 0x56, 0x78, 0x9a, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1);
	CHECK(ip1 != ip2);
}

TEST(IPAddress_operator_comparison6, not_equal6_04_same) {
	arduino::IPAddress ip1(0x20, 0x01, 0xd, 0xb8, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb, 0xc),
		ip2(0x20, 0x01, 0xd, 0xb8, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb, 0xc);
	CHECK_FALSE(ip1 != ip2);
}

// IPv4 and IPv6 differ based on type (irrespective of bytes)

TEST(IPAddress_operator_comparison6, equal6_05_v4_vs_v6) {
	arduino::IPAddress ip1(10, 0, 0, 1),
		ip2(0x20, 0x01, 0xd, 0xb8, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb, 0xc);
	CHECK_FALSE(ip1 == ip2);
}

TEST(IPAddress_operator_comparison6, equal6_05_v4_compatible) {
	arduino::IPAddress ip1(10, 0, 0, 1), ip2(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 1);
	CHECK_FALSE(ip1 == ip2);
}

TEST(IPAddress_operator_comparison6, equal6_05_v4_vs_v6_localhost) {
	arduino::IPAddress ip1(127, 0, 0, 1), ip2(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 1);
	CHECK_FALSE(ip1 == ip2);
}

TEST(IPAddress_operator_comparison6, equal6_05_v4_compatible_localhost) {
	arduino::IPAddress ip1(0, 0, 0, 1), ip2(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1);
	CHECK_FALSE(ip1 == ip2);
}

TEST(IPAddress_operator_comparison6, equal6_06_raw_bytes) {
	arduino::IPAddress ip1(0x20, 0x01, 0xd, 0xb8, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb, 0xc);
	uint8_t const ip2[] = {0x20, 0x01, 0xd, 0xb8, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb, 0xc};
	CHECK_FALSE(ip1 == ip2);
}
