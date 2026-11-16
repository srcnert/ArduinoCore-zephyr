/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/Stream/test_readStringUntil.cpp
 * millis_autoOn() is gone: these tests use the real millis().
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

#include "StreamMock.h"

TEST_GROUP(Stream_readStringUntil) {
	StreamMock mock;
};

TEST(Stream_readStringUntil, readStringUntil_01) {
	mock.setTimeout(10);
	mock << "This is test! lorem ipsum lalala";
	STRCMP_EQUAL("This is test", mock.readStringUntil('!').c_str());
}

TEST(Stream_readStringUntil, readStringUntil_02) {
	mock.setTimeout(10);
	mock << "This is test ... lorem ipsum lalala";
	STRCMP_EQUAL("This is test ... lorem ipsum lalala", mock.readStringUntil('!').c_str());
}
