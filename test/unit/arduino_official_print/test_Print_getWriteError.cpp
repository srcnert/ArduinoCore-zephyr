/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/Print/test_getWriteError.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

#include "PrintMock.h"

TEST_GROUP(Print_getWriteError) {
	PrintMock mock;
};

TEST(Print_getWriteError, getWriteError_01) {
	LONGS_EQUAL(0, mock.getWriteError());
}

TEST(Print_getWriteError, getWriteError_02) {
	mock.mock_setWriteError(5);
	LONGS_EQUAL(5, mock.getWriteError());
}
