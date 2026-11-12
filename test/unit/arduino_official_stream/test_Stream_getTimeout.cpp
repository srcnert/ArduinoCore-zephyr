/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/Stream/test_getTimeout.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

#include "StreamMock.h"

TEST_GROUP(Stream_getTimeout) {
	StreamMock mock;
};

TEST(Stream_getTimeout, getTimeout_01) {
	UNSIGNED_LONGS_EQUAL(1000, mock.getTimeout());
}
