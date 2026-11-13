/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/IPAddress/test_fromString6.cpp
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

TEST_GROUP(IPAddress_fromString6) {
	arduino::IPAddress ip;
};

TEST(IPAddress_fromString6, fromString6_01) {
	CHECK(ip.fromString("2001:db8:102:304:506:708:90a:b0c"));
	CHECK(ip.type() == arduino::IPType::IPv6);
	CHECK_IPV6_BYTES(ip, 0x20, 0x01, 0xd, 0xb8, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb, 0xc);
}

TEST(IPAddress_fromString6, fromString6_02) {
	arduino::String const ip_addr_str("2001:db8:102:304:506:708:90a:b0c");
	CHECK(ip.fromString(ip_addr_str));
	CHECK(ip.type() == arduino::IPType::IPv6);
	CHECK_IPV6_BYTES(ip, 0x20, 0x01, 0xd, 0xb8, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb, 0xc);
}

TEST(IPAddress_fromString6, fromString6_03_any) {
	CHECK(ip.fromString("::"));
	CHECK(ip.type() == arduino::IPType::IPv6);
	CHECK_IPV6_BYTES(ip, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

TEST(IPAddress_fromString6, fromString6_04_localhost) {
	CHECK(ip.fromString("::1"));
	CHECK(ip.type() == arduino::IPType::IPv6);
	CHECK_IPV6_BYTES(ip, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1);
}

TEST(IPAddress_fromString6, fromString6_05_segment_lengths) {
	CHECK(ip.fromString("abcd:ef1:23:0:4::"));
	CHECK(ip.type() == arduino::IPType::IPv6);
	CHECK_IPV6_BYTES(ip, 0xab, 0xcd, 0xe, 0xf1, 0, 0x23, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0);
}

TEST(IPAddress_fromString6, fromString6_06_zero_start) {
	CHECK(ip.fromString("0:2:3:4:5:6:7:8"));
	CHECK(ip.type() == arduino::IPType::IPv6);
	CHECK_IPV6_BYTES(ip, 0, 0, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7, 0, 8);
}

TEST(IPAddress_fromString6, fromString6_07_zero_end) {
	CHECK(ip.fromString("1:2:3:4:5:6:7:0"));
	CHECK(ip.type() == arduino::IPType::IPv6);
	CHECK_IPV6_BYTES(ip, 0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7, 0, 0);
}

TEST(IPAddress_fromString6, fromString6_08_two_zero_start) {
	CHECK(ip.fromString("::3:4:5:6:7:0"));
	CHECK(ip.type() == arduino::IPType::IPv6);
	CHECK_IPV6_BYTES(ip, 0, 0, 0, 0, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7, 0, 0);
}

TEST(IPAddress_fromString6, fromString6_09_two_zero_end) {
	CHECK(ip.fromString("0:2:3:4:5:6::"));
	CHECK(ip.type() == arduino::IPType::IPv6);
	CHECK_IPV6_BYTES(ip, 0, 0, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 0, 0, 0);
}

// Non-canonical

TEST(IPAddress_fromString6, fromString6_10_any_long_form) {
	CHECK(ip.fromString("0:0:0:0:0:0:0:0"));
	CHECK(ip.type() == arduino::IPType::IPv6);
	CHECK_IPV6_BYTES(ip, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

TEST(IPAddress_fromString6, fromString6_11_upper_case) {
	CHECK(ip.fromString("2001:DB8:102:304:506:708:90A:B0C"));
	CHECK(ip.type() == arduino::IPType::IPv6);
	CHECK_IPV6_BYTES(ip, 0x20, 0x01, 0xd, 0xb8, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb, 0xc);
}

TEST(IPAddress_fromString6, fromString6_10_explicit_start_zero) {
	CHECK(ip.fromString("0::"));
	CHECK(ip.type() == arduino::IPType::IPv6);
	CHECK_IPV6_BYTES(ip, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

TEST(IPAddress_fromString6, fromString6_10_explicit_end_zero) {
	CHECK(ip.fromString("::0"));
	CHECK(ip.type() == arduino::IPType::IPv6);
	CHECK_IPV6_BYTES(ip, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

TEST(IPAddress_fromString6, fromString6_10_one_zero_group) {
	CHECK(ip.fromString("1::3:4:5:6:7:8"));
	CHECK(ip.type() == arduino::IPType::IPv6);
	CHECK_IPV6_BYTES(ip, 0, 1, 0, 0, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7, 0, 8);
}

// Invalid cases

TEST(IPAddress_fromString6, fromString6_12_invalid) {
	// three colons by self
	CHECK_FALSE(ip.fromString(":::"));
	// two compressions
	CHECK_FALSE(ip.fromString("::3:4:5:6::"));
	// 5 character field
	CHECK_FALSE(ip.fromString("2001:db8:102:10304:506:708:90a:b0c"));
	// invalid character
	CHECK_FALSE(ip.fromString("200x:db8:102:304:506:708:90a:b0c"));
	// double colon with 8 other fields (so not a compression)
	CHECK_FALSE(ip.fromString("2001:db8:102:304::506:708:90a:b0c"));
	// three colons in middle
	CHECK_FALSE(ip.fromString("2001:db8:102:304:::708:90a:b0c"));
	// 9 fields
	CHECK_FALSE(ip.fromString("2001:db8:102:304:506:708:90a:b0c:d0e"));
	// missing last group (but has a colon)
	CHECK_FALSE(ip.fromString("2001:db8:102:304:506:708:90a:"));
	// only seven groups
	CHECK_FALSE(ip.fromString("2001:db8:102:304:506:708:90a"));
	// nine zeros
	CHECK_FALSE(ip.fromString("0:0:0:0:0:0:0:0:0"));
	// extra colon
	CHECK_FALSE(ip.fromString("0:0:0:0:0:0:0:0:"));
	// missing last group (but has a colon)
	CHECK_FALSE(ip.fromString("0:0:0:0:0:0:0:"));
	// only seven groups
	CHECK_FALSE(ip.fromString("0:0:0:0:0:0:0"));
}
