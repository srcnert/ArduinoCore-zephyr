/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/Stream/test_readBytes.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>
#include <string.h>

#include "StreamMock.h"

TEST_GROUP(Stream_readBytes) {
	StreamMock mock;

	void setup() override {
		mock.setTimeout(10);
	}
};

TEST(Stream_readBytes, readBytes_01_empty) {
	char buf[32] = {0};
	LONGS_EQUAL(0, mock.readBytes(buf, sizeof(buf)));
}

TEST(Stream_readBytes, readBytes_01_less_data) {
	char buf[32] = {0};
	char const str[] = "some stream content";
	mock << str;
	LONGS_EQUAL(strlen(str), mock.readBytes(buf, sizeof(buf)));
	STRNCMP_EQUAL(str, buf, sizeof(buf));
	STRCMP_EQUAL("", mock.readString().c_str());
}

TEST(Stream_readBytes, readBytes_01_more_data) {
	char buf[5] = {0};
	mock << "some stream content";
	char const EXPECTED_STR[] = "some ";
	LONGS_EQUAL(5, mock.readBytes(buf, sizeof(buf)));
	STRNCMP_EQUAL(EXPECTED_STR, buf, sizeof(buf));
	STRCMP_EQUAL("stream content", mock.readString().c_str());
}
