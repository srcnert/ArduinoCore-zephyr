/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/IPAddress/test_IPAddress6.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

// Upstream checks the 16 bytes one REQUIRE at a time; MEMCMP_EQUAL prints all of them on failure
#define CHECK_IPV6_BYTES(ip, ...)                                                                  \
	do {                                                                                           \
		uint8_t const expected[16] = {__VA_ARGS__};                                                \
		uint8_t actual[16];                                                                        \
		for (int i = 0; i < 16; i++) {                                                             \
			actual[i] = (ip)[i];                                                                   \
		}                                                                                          \
		MEMCMP_EQUAL(expected, actual, sizeof(expected));                                          \
	} while (0)

TEST_GROUP(IPAddress_IPAddress6){};

TEST(IPAddress_IPAddress6, Ctor6_01) {
	arduino::IPAddress ip(arduino::IPType::IPv6);
	CHECK(ip.type() == arduino::IPType::IPv6);
	CHECK_IPV6_BYTES(ip, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

TEST(IPAddress_IPAddress6, Ctor6_02) {
	arduino::IPAddress ip(0x20, 0x01, 0xd, 0xb8, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb, 0xc);
	CHECK(ip.type() == arduino::IPType::IPv6);
	CHECK_IPV6_BYTES(ip, 0x20, 0x01, 0xd, 0xb8, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb, 0xc);
}

TEST(IPAddress_IPAddress6, Ctor6_03) {
	uint8_t const ip_addr_array[] = {0x20, 0x01, 0xd, 0xb8, 1, 2,   3,   4,
									 5,    6,    7,   8,    9, 0xa, 0xb, 0xc};
	arduino::IPAddress ip(arduino::IPType::IPv6, ip_addr_array);
	CHECK(ip.type() == arduino::IPType::IPv6);
	CHECK_IPV6_BYTES(ip, 0x20, 0x01, 0xd, 0xb8, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb, 0xc);
}
