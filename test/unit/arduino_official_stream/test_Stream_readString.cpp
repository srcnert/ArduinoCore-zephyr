/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/Stream/test_readString.cpp
 * millis_autoOn() is gone: these tests use the real millis().
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

#include "StreamMock.h"

TEST_GROUP(Stream_readString) {
	StreamMock mock;
};

TEST(Stream_readString, readString_01) {
	mock.setTimeout(10);
	mock << "This is test stream content";
	STRCMP_EQUAL("This is test stream content", mock.readString().c_str());
}
