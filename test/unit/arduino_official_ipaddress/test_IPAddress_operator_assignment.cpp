/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2
 * test/src/IPAddress/test_operator_assignment.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

TEST_GROUP(IPAddress_operator_assignment){};

TEST(IPAddress_operator_assignment, assignment_01) {
	arduino::IPAddress ip1;
	uint8_t const ip2[] = {192, 168, 1, 2};
	ip1 = ip2;
	CHECK(ip1 == arduino::IPAddress(192, 168, 1, 2));
}

TEST(IPAddress_operator_assignment, assignment_02) {
	arduino::IPAddress ip1;
	uint32_t const ip2 = 192 | (168 << 8) | (1 << 16) | (2 << 24);
	ip1 = ip2;
	// NOTE: Only correct on little-endian systems
	CHECK(ip1 == arduino::IPAddress(192, 168, 1, 2));
}
