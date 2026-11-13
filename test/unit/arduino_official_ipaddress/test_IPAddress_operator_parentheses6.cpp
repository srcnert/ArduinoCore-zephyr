/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2
 * test/src/IPAddress/test_operator_parentheses6.cpp
 * 129 << 24 does not fit an int, so the network-order value uses 129u.
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

// These comparisons should always return false, as you can't compare an IPv6 to an int32_t

TEST_GROUP(IPAddress_operator_parentheses6){};

TEST(IPAddress_operator_parentheses6, uint32_6_01_le_last_four) {
	// On little endian systems, considering only last four octets (ignoring the rest)
	arduino::IPAddress ip(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 129, 168, 1, 2);
	uint32_t const val_expected = ip;
	uint32_t const val_actual = (129 | (168 << 8) | (1 << 16) | (2 << 24));
	CHECK_FALSE(val_expected == val_actual);
}

TEST(IPAddress_operator_parentheses6, uint32_6_01_le_full) {
	// On little endian systems (full value)
	arduino::IPAddress ip(129, 168, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	uint32_t const val_expected = ip;
	uint32_t const val_actual = (129 | (168 << 8) | (1 << 16) | (2 << 24));
	CHECK_FALSE(val_expected == val_actual);
}

TEST(IPAddress_operator_parentheses6, uint32_6_01_network_order) {
	// Actual value of the 128-bit IPv6 address, which is network byte order
	arduino::IPAddress ip(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 129, 168, 1, 2);
	uint32_t const val_expected = ip;
	uint32_t const val_actual = ((129u << 24) | (168 << 16) | (1 << 8) | 2);
	CHECK_FALSE(val_expected == val_actual);
}
