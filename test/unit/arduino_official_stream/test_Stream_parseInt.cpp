/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/Stream/test_parseInt.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

#include "StreamMock.h"

TEST_GROUP(Stream_parseInt) {
	StreamMock mock;

	void setup() override {
		mock.setTimeout(10);
	}
};

TEST(Stream_parseInt, parseInt_01_positive) {
	mock << "1234";
	LONGS_EQUAL(1234, mock.parseInt());
}

TEST(Stream_parseInt, parseInt_01_negative) {
	mock << "-1234";
	LONGS_EQUAL(-1234, mock.parseInt());
}

TEST(Stream_parseInt, parseInt_01_after_letters) {
	mock << "abcdef1234";
	LONGS_EQUAL(1234, mock.parseInt());
}

TEST(Stream_parseInt, parseInt_01_after_whitespace) {
	mock << "\r\n\t 1234";
	LONGS_EQUAL(1234, mock.parseInt());
}

TEST(Stream_parseInt, parseInt_02_positive) {
	mock << "1234";
	LONGS_EQUAL(1234, mock.parseInt(SKIP_NONE));
	STRCMP_EQUAL("", mock.readString().c_str());
}

TEST(Stream_parseInt, parseInt_02_after_letters) {
	mock << "abcdef1234";
	LONGS_EQUAL(0, mock.parseInt(SKIP_NONE));
	STRCMP_EQUAL("abcdef1234", mock.readString().c_str());
}

TEST(Stream_parseInt, parseInt_02_after_whitespace) {
	mock << "\r\n\t 1234";
	LONGS_EQUAL(0, mock.parseInt(SKIP_NONE));
	STRCMP_EQUAL("\r\n\t 1234", mock.readString().c_str());
}

TEST(Stream_parseInt, parseInt_03_after_whitespace) {
	mock << "\r\n\t 1234";
	LONGS_EQUAL(1234, mock.parseInt(SKIP_WHITESPACE));
	STRCMP_EQUAL("", mock.readString().c_str());
}

TEST(Stream_parseInt, parseInt_04_positive) {
	mock << "1234";
	LONGS_EQUAL(1234, mock.parseInt(SKIP_ALL, 'a'));
	STRCMP_EQUAL("", mock.readString().c_str());
}

TEST(Stream_parseInt, parseInt_04_only_ignore_chars) {
	mock << "12a3a4a";
	LONGS_EQUAL(1234, mock.parseInt(SKIP_ALL, 'a'));
	STRCMP_EQUAL("", mock.readString().c_str());
}

TEST(Stream_parseInt, parseInt_04_other_chars) {
	mock << "1bed234";
	LONGS_EQUAL(1, mock.parseInt(SKIP_ALL, 'a'));
	STRCMP_EQUAL("bed234", mock.readString().c_str());
}
