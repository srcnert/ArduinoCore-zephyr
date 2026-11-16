/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/IPAddress/test_printTo.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

#include "PrintMock.h"

TEST_GROUP(IPAddress_printTo) {
	PrintMock mock;
};

TEST(IPAddress_printTo, printTo_01) {
	arduino::IPAddress const ip(192, 168, 1, 2);
	mock.print(ip);
	STRCMP_EQUAL("192.168.1.2", mock._str);
}
