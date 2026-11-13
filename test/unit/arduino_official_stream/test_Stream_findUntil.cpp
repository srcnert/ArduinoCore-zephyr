/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/Stream/test_findUntil.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

#include "StreamMock.h"

TEST_GROUP(Stream_findUntil) {
	StreamMock mock;

	void setup() override {
		mock.setTimeout(10);
	}
};

TEST(Stream_findUntil, findUntil_01_terminator_before_target) {
	mock << "This is a : test string";
	CHECK_FALSE(mock.findUntil("test", ": "));
	STRCMP_EQUAL("test string", mock.readString().c_str());
}

TEST(Stream_findUntil, findUntil_01_terminator_after_target) {
	mock << "This is a test : string";
	CHECK(mock.findUntil("test", ": "));
	STRCMP_EQUAL(" : string", mock.readString().c_str());
}

TEST(Stream_findUntil, findUntil_01_no_terminator) {
	mock << "This is a test string";
	CHECK(mock.findUntil("test", ": "));
	STRCMP_EQUAL(" string", mock.readString().c_str());
}

TEST(Stream_findUntil, findUntil_01_target_missing) {
	mock << "This is a test string";
	CHECK_FALSE(mock.findUntil("abc", "def"));
	STRCMP_EQUAL("", mock.readString().c_str());
}
