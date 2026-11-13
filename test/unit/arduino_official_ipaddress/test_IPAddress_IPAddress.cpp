/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/IPAddress/test_IPAddress.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

TEST_GROUP(IPAddress_IPAddress){};

TEST(IPAddress_IPAddress, Ctor_01) {
	arduino::IPAddress ip;
	LONGS_EQUAL(0, ip[0]);
	LONGS_EQUAL(0, ip[1]);
	LONGS_EQUAL(0, ip[2]);
	LONGS_EQUAL(0, ip[3]);
}

TEST(IPAddress_IPAddress, Ctor_02) {
	arduino::IPAddress ip(129, 168, 1, 2);
	LONGS_EQUAL(129, ip[0]);
	LONGS_EQUAL(168, ip[1]);
	LONGS_EQUAL(1, ip[2]);
	LONGS_EQUAL(2, ip[3]);
}

TEST(IPAddress_IPAddress, Ctor_03) {
	arduino::IPAddress ip(129 | (168 << 8) | (1 << 16) | (2 << 24));
	LONGS_EQUAL(129, ip[0]);
	LONGS_EQUAL(168, ip[1]);
	LONGS_EQUAL(1, ip[2]);
	LONGS_EQUAL(2, ip[3]);
}

TEST(IPAddress_IPAddress, Ctor_04) {
	uint8_t const ip_addr_array[] = {129, 168, 1, 2};
	arduino::IPAddress ip(ip_addr_array);
	LONGS_EQUAL(129, ip[0]);
	LONGS_EQUAL(168, ip[1]);
	LONGS_EQUAL(1, ip[2]);
	LONGS_EQUAL(2, ip[3]);
}
