/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2
 * test/src/IPAddress/test_operator_parentheses.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

TEST_GROUP(IPAddress_operator_parentheses){};

TEST(IPAddress_operator_parentheses, uint32_01) {
	arduino::IPAddress ip(129, 168, 1, 2);
	uint32_t const val_expected = ip;
	uint32_t const val_actual = (129 | (168 << 8) | (1 << 16) | (2 << 24));
	// NOTE: Only correct on little-endian systems
	UNSIGNED_LONGS_EQUAL(val_actual, val_expected);
}
