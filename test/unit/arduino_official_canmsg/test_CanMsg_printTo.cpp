/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/CanMsg/test_printTo.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>
#include <api/CanMsg.h>

#include "PrintMock.h"

TEST_GROUP(CanMsg_printTo) {
	PrintMock mock;
};

TEST(CanMsg_printTo, printTo_01_standard_id) {
	uint8_t const std_msg_data[] = {0xBE, 0xEF};
	CanMsg const std_msg(CanStandardId(0x20), sizeof(std_msg_data), std_msg_data);
	mock.print(std_msg);
	STRCMP_EQUAL("[020] (2) : BEEF", mock._str);
}

TEST(CanMsg_printTo, printTo_02_extended_id) {
	uint8_t const ext_msg_data[] = {0xDE, 0xAD, 0xC0, 0xDE};
	CanMsg const ext_msg(CanExtendedId(0x20), sizeof(ext_msg_data), ext_msg_data);
	mock.print(ext_msg);
	STRCMP_EQUAL("[00000020] (4) : DEADC0DE", mock._str);
}
