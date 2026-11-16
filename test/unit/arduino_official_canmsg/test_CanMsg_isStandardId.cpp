/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/CanMsg/test_isStandardId.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>
#include <api/CanMsg.h>

TEST_GROUP(CanMsg_isStandardId){};

TEST(CanMsg_isStandardId, isStandardId_01) {
	CanMsg const msg_std_id(CanStandardId(0x020), 0, nullptr);
	CHECK(msg_std_id.isStandardId());
}

TEST(CanMsg_isStandardId, isStandardId_02) {
	CanMsg const msg_ext_id(CanExtendedId(0x020), 0, nullptr);
	CHECK_FALSE(msg_ext_id.isStandardId());
}
