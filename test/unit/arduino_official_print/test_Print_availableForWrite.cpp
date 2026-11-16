/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2
 * test/src/Print/test_availableForWrite.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

#include "PrintMock.h"

TEST_GROUP(Print_availableForWrite) {
	PrintMock mock;
};

TEST(Print_availableForWrite, availableForWrite_01) {
	LONGS_EQUAL(0, mock.availableForWrite());
}
