/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/IPAddress/test_toString.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

TEST_GROUP(IPAddress_toString){};

TEST(IPAddress_toString, toString_01) {
	arduino::IPAddress ip(129, 168, 1, 2);
	STRCMP_EQUAL("129.168.1.2", ip.toString().c_str());
}

TEST(IPAddress_toString, toString_02) {
	arduino::IPAddress ip(0x20, 0x01, 0xd, 0xb8, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xa, 0xb, 0xc);
	STRCMP_EQUAL("2001:0db8:0102:0304:0506:0708:090a:0b0c", ip.toString().c_str());
}

TEST(IPAddress_toString, toString_03) {
	arduino::IPAddress ip;
	STRCMP_EQUAL("0.0.0.0", ip.toString().c_str());
}
