/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/CanMsg/test_CanMsg_CopyCtor.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>
#include <api/CanMsg.h>

TEST_GROUP(CanMsg_CopyCtor){};

TEST(CanMsg_CopyCtor, CopyCtor_01) {
	uint8_t const msg_data[4] = {0xDE, 0xAD, 0xC0, 0xDE};
	CanMsg const msg_1(CanStandardId(0x20), sizeof(msg_data), msg_data);
	CanMsg const msg_2(msg_1);
	LONGS_EQUAL(msg_1.data_length, msg_2.data_length);
	for (size_t i = 0; i < msg_1.data_length; i++) {
		LONGS_EQUAL(msg_data[i], msg_1.data[i]);
		LONGS_EQUAL(msg_data[i], msg_2.data[i]);
	}
}
