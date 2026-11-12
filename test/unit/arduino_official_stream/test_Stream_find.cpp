/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/Stream/test_find.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

#include "StreamMock.h"

TEST_GROUP(Stream_find) {
	StreamMock mock;

	void setup() override {
		mock.setTimeout(10);
	}
};

TEST(Stream_find, find_01_found) {
	mock << "This is a test string";
	CHECK(mock.find("test"));
	STRCMP_EQUAL(" string", mock.readString().c_str());
}

TEST(Stream_find, find_01_not_found) {
	mock << "This is a string";
	CHECK_FALSE(mock.find("test"));
	STRCMP_EQUAL("", mock.readString().c_str());
}

TEST(Stream_find, find_02_found) {
	mock << "This is a test string";
	/* 'length' should actually be '4' or strlen("test"). I'd rather
	 * think this API should not be exposed at all.
	 */
	CHECK(mock.find("test", 3));
	STRCMP_EQUAL("t string", mock.readString().c_str());
}

TEST(Stream_find, find_02_not_found) {
	mock << "This is a string";
	CHECK_FALSE(mock.find("test", 3));
	STRCMP_EQUAL("", mock.readString().c_str());
}

TEST(Stream_find, find_03_found) {
	mock << "This is a test string";
	CHECK(mock.find('t'));
	STRCMP_EQUAL("est string", mock.readString().c_str());
}

TEST(Stream_find, find_03_not_found) {
	mock << "This is a string";
	CHECK_FALSE(mock.find('!'));
	STRCMP_EQUAL("", mock.readString().c_str());
}
