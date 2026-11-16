/*
 * Copyright (c) 2026 RAKwireless Technology Limited
 * Author: sercan.erat@rakwireless.com
 * SPDX-License-Identifier: Apache-2.0
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

TEST_GROUP(IPAddress){};

TEST(IPAddress, ipv4_constructors) {
	const uint8_t raw[] = {192, 168, 1, 10};

	STRCMP_EQUAL("0.0.0.0", IPAddress().toString().c_str());
	STRCMP_EQUAL("192.168.1.10", IPAddress(192, 168, 1, 10).toString().c_str());
	STRCMP_EQUAL("192.168.1.10", IPAddress(raw).toString().c_str());
	STRCMP_EQUAL("192.168.1.10", IPAddress("192.168.1.10").toString().c_str());
	// The uint32_t form is in memory order, so 192 is the lowest byte on ARM
	STRCMP_EQUAL("192.168.1.10", IPAddress((uint32_t)0x0A01A8C0).toString().c_str());
	UNSIGNED_LONGS_EQUAL(0x0A01A8C0, (uint32_t)IPAddress(192, 168, 1, 10));
	CHECK(IPAddress().type() == IPv4);
}

TEST(IPAddress, ipv4_from_string) {
	IPAddress ip;
	const IPAddress &cip = ip;

	CHECK(ip.fromString("10.0.0.255"));
	LONGS_EQUAL(10, ip[0]);
	LONGS_EQUAL(255, cip[3]);
	CHECK(ip.fromString(String("1.2.3.4")));
	STRCMP_EQUAL("1.2.3.4", ip.toString().c_str());
	CHECK_FALSE(ip.fromString("256.0.0.1"));
	CHECK_FALSE(ip.fromString("1.2.3"));
	CHECK_FALSE(ip.fromString("1..2.3"));
	CHECK_FALSE(ip.fromString("1.2.3.4.5"));
	CHECK_FALSE(ip.fromString("1.2.3."));
	CHECK_FALSE(ip.fromString("a.b.c.d"));
}

TEST(IPAddress, ipv4_operators) {
	const uint8_t raw[] = {172, 16, 0, 1};
	IPAddress ip;

	ip = raw;
	CHECK(ip == raw);
	CHECK(ip == IPAddress(172, 16, 0, 1));
	CHECK(ip != IPAddress(172, 16, 0, 2));
	ip = "8.8.4.4";
	STRCMP_EQUAL("8.8.4.4", ip.toString().c_str());
	ip = (uint32_t)0x0100007F;
	STRCMP_EQUAL("127.0.0.1", ip.toString().c_str());
	ip[3] = 2;
	STRCMP_EQUAL("127.0.0.2", ip.toString().c_str());
}

TEST(IPAddress, ipv6) {
	const uint8_t raw[16] = {0x20, 0x01, 0x0d, 0xb8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
	IPAddress ip;

	CHECK(ip.fromString("2001:DB8::1"));
	CHECK(ip.type() == IPv6);
	CHECK(ip == IPAddress(IPv6, raw));
	CHECK(ip == IPAddress(0x20, 0x01, 0x0d, 0xb8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
	STRCMP_EQUAL("2001:0db8:0000:0000:0000:0000:0000:0001", ip.toString().c_str());
	LONGS_EQUAL(0xb8, ip[3]);
	CHECK(ip.fromString("::1"));
	LONGS_EQUAL(1, ip[15]);
	CHECK_FALSE(ip.fromString("1:2:3"));
	CHECK_FALSE(ip.fromString("1::2::3"));
	CHECK_FALSE(ip.fromString("12345::"));
	CHECK_FALSE(ip.fromString("g::1"));
	CHECK(IPAddress(IPv6).type() == IPv6);
}
