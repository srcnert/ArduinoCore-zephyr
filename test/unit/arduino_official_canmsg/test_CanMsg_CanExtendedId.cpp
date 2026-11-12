/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/CanMsg/test_CanExtendedId.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>
#include <api/CanMsg.h>

TEST_GROUP(CanMsg_CanExtendedId){};

TEST(CanMsg_CanExtendedId, CanExtendedId_01) {
	UNSIGNED_LONGS_EQUAL((CanMsg::CAN_EFF_FLAG | 0U), CanExtendedId(0));
}

TEST(CanMsg_CanExtendedId, CanExtendedId_02) {
	UNSIGNED_LONGS_EQUAL((CanMsg::CAN_EFF_FLAG | 0x1FFFFFFFU), CanExtendedId(0x1FFFFFFFU));
}

TEST(CanMsg_CanExtendedId, CanExtendedId_03) {
	UNSIGNED_LONGS_EQUAL((CanMsg::CAN_EFF_FLAG | 0x0FFFFFFFU), CanExtendedId(0x2FFFFFFFU));
	UNSIGNED_LONGS_EQUAL((CanMsg::CAN_EFF_FLAG | 0x1FFFFFFFU), CanExtendedId(0x3FFFFFFFU));
	UNSIGNED_LONGS_EQUAL((CanMsg::CAN_EFF_FLAG | 0x0FFFFFFFU), CanExtendedId(0x4FFFFFFFU));
	UNSIGNED_LONGS_EQUAL((CanMsg::CAN_EFF_FLAG | 0x1FFFFFFFU), CanExtendedId(0x5FFFFFFFU));
	/* ... */
	UNSIGNED_LONGS_EQUAL((CanMsg::CAN_EFF_FLAG | 0x1FFFFFFFU), CanExtendedId(0xFFFFFFFFU));
}
