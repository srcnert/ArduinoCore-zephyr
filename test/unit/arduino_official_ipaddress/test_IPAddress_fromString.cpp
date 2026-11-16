/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/IPAddress/test_fromString.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

TEST_GROUP(IPAddress_fromString){};

TEST(IPAddress_fromString, fromString_01) {
	arduino::IPAddress ip;
	CHECK(ip.fromString("129.168.1.2"));
	LONGS_EQUAL(129, ip[0]);
	LONGS_EQUAL(168, ip[1]);
	LONGS_EQUAL(1, ip[2]);
	LONGS_EQUAL(2, ip[3]);
}

TEST(IPAddress_fromString, fromString_02) {
	arduino::IPAddress ip;
	arduino::String const ip_addr_str("129.168.1.2");
	CHECK(ip.fromString(ip_addr_str));
	LONGS_EQUAL(129, ip[0]);
	LONGS_EQUAL(168, ip[1]);
	LONGS_EQUAL(1, ip[2]);
	LONGS_EQUAL(2, ip[3]);
}

TEST(IPAddress_fromString, fromString_03) {
	arduino::IPAddress ip;
	CHECK_FALSE(ip.fromString(""));
	CHECK_FALSE(ip.fromString("1"));
	CHECK_FALSE(ip.fromString("1."));
	CHECK_FALSE(ip.fromString("1.1"));
	CHECK_FALSE(ip.fromString("1.1."));
	CHECK_FALSE(ip.fromString("1.1.1"));
	CHECK_FALSE(ip.fromString("1.1.1."));
	CHECK_FALSE(ip.fromString("..."));
	CHECK_FALSE(ip.fromString("256.1.1.1"));
	CHECK_FALSE(ip.fromString("a.1.1.1"));
	CHECK_FALSE(ip.fromString("-.1.1.1"));
	CHECK_FALSE(ip.fromString("-1.1.1.1"));
}
