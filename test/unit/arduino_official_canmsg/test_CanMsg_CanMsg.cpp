/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/CanMsg/test_CanMsg.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>
#include <api/CanMsg.h>

TEST_GROUP(CanMsg_CanMsg){};

TEST(CanMsg_CanMsg, CanMsg_01) {
	CanMsg const msg(CanStandardId(0x20), 0, nullptr);
	LONGS_EQUAL(0, msg.data_length);
	for (size_t i = 0; i < CanMsg::MAX_DATA_LENGTH; i++) {
		LONGS_EQUAL(0, msg.data[i]);
	}
}

TEST(CanMsg_CanMsg, CanMsg_02) {
	uint8_t const msg_data[4] = {0xDE, 0xAD, 0xC0, 0xDE};
	CanMsg const msg(CanStandardId(0x20), sizeof(msg_data), msg_data);
	LONGS_EQUAL(4, msg.data_length);
	for (size_t i = 0; i < msg.data_length; i++) {
		LONGS_EQUAL(msg_data[i], msg.data[i]);
	}
}

TEST(CanMsg_CanMsg, CanMsg_03) {
	uint8_t const msg_data[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
	CanMsg const msg(CanStandardId(0x20), sizeof(msg_data), msg_data);
	LONGS_EQUAL(8, msg.data_length);
	for (size_t i = 0; i < msg.data_length; i++) {
		LONGS_EQUAL(msg_data[i], msg.data[i]);
	}
}

TEST(CanMsg_CanMsg, CanMsg_04) {
	CanMsg const msg(CanStandardId(0x20), 0, nullptr);
	UNSIGNED_LONGS_EQUAL(0x20, msg.id);
}

TEST(CanMsg_CanMsg, CanMsg_05) {
	CanMsg const msg(CanExtendedId(0x20), 0, nullptr);
	UNSIGNED_LONGS_EQUAL((CanMsg::CAN_EFF_FLAG | 0x20), msg.id);
}
