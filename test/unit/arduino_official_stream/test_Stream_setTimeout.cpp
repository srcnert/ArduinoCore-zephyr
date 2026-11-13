/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/Stream/test_setTimeout.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

#include "StreamMock.h"

TEST_GROUP(Stream_setTimeout) {
	StreamMock mock;
};

TEST(Stream_setTimeout, setTimeout_01) {
	mock.setTimeout(100);
	UNSIGNED_LONGS_EQUAL(100, mock.getTimeout());
}
