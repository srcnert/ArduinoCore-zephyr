/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/CanMsg/test_CanStandardId.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>
#include <api/CanMsg.h>

TEST_GROUP(CanMsg_CanStandardId){};

TEST(CanMsg_CanStandardId, CanStandardId_01) {
	UNSIGNED_LONGS_EQUAL(0U, CanStandardId(0));
}

TEST(CanMsg_CanStandardId, CanStandardId_02) {
	UNSIGNED_LONGS_EQUAL(0x7FF, CanStandardId(0x7FF));
}

TEST(CanMsg_CanStandardId, CanStandardId_03) {
	UNSIGNED_LONGS_EQUAL(0x000U, CanStandardId(0x800U));
	UNSIGNED_LONGS_EQUAL(0x001U, CanStandardId(0x801U));
	UNSIGNED_LONGS_EQUAL(0x0FFU, CanStandardId(0x8FFU));
	UNSIGNED_LONGS_EQUAL(0x7FFU, CanStandardId(0x1FFFFFFFU));
	UNSIGNED_LONGS_EQUAL(0x7FFU, CanStandardId(0xFFFFFFFFU));
}
