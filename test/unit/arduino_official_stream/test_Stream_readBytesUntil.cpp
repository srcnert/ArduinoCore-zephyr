/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/Stream/test_readBytesUntil.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>
#include <string.h>

#include "StreamMock.h"

TEST_GROUP(Stream_readBytesUntil) {
	StreamMock mock;

	void setup() override {
		mock.setTimeout(10);
	}
};

TEST(Stream_readBytesUntil, readBytesUntil_01_empty) {
	char buf[32] = {0};
	LONGS_EQUAL(0, mock.readBytesUntil(' ', buf, sizeof(buf)));
}

TEST(Stream_readBytesUntil, readBytesUntil_01_terminator) {
	char buf[32] = {0};
	char const str[] = "some stream content";
	char const EXPECTED_STR[] = "some";
	mock << str;
	LONGS_EQUAL(strlen("some"), mock.readBytesUntil(' ', buf, sizeof(buf)));
	STRNCMP_EQUAL(EXPECTED_STR, buf, sizeof(buf));
	STRCMP_EQUAL("stream content", mock.readString().c_str());
}

TEST(Stream_readBytesUntil, readBytesUntil_01_no_terminator) {
	char buf[32] = {0};
	char const STR[] = "some stream content";
	mock << STR;
	LONGS_EQUAL(strlen(STR), mock.readBytesUntil('!', buf, sizeof(buf)));
	STRNCMP_EQUAL(STR, buf, sizeof(buf));
	STRCMP_EQUAL("", mock.readString().c_str());
}
