/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/Stream/test_parseFloat.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>
#include <float.h>

#include "StreamMock.h"

// Catch2's Approx() accepts a relative difference of 100 * FLT_EPSILON
#define APPROX_MARGIN(value) (100 * FLT_EPSILON * (value))

TEST_GROUP(Stream_parseFloat) {
	StreamMock mock;

	void setup() override {
		mock.setTimeout(10);
	}
};

TEST(Stream_parseFloat, parseFloat_01_integer) {
	mock << "12";
	DOUBLES_EQUAL(12.0f, mock.parseFloat(), 0.0);
}

TEST(Stream_parseFloat, parseFloat_01_positive) {
	mock << "12.34";
	DOUBLES_EQUAL(12.34f, mock.parseFloat(), 0.0);
}

TEST(Stream_parseFloat, parseFloat_01_negative) {
	mock << "-12.34";
	DOUBLES_EQUAL(-12.34f, mock.parseFloat(), 0.0);
}

TEST(Stream_parseFloat, parseFloat_01_after_letters) {
	mock << "abcdef12.34";
	DOUBLES_EQUAL(12.34f, mock.parseFloat(), 0.0);
}

TEST(Stream_parseFloat, parseFloat_01_after_whitespace) {
	mock << "\r\n\t 12.34";
	DOUBLES_EQUAL(12.34f, mock.parseFloat(), 0.0);
}

TEST(Stream_parseFloat, parseFloat_01_too_many_digits) {
	mock << "3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628"
			"0348253421170679821480865132823066470938446095505822317253594081284811174502841027019"
			"3852110555964462294895493038196442881097566593344612847564823378678316527120190914564"
			"85669234603486104543266482133936072602491412737245870064";
	DOUBLES_EQUAL(3.141592654f, mock.parseFloat(), APPROX_MARGIN(3.141592654f));
}

TEST(Stream_parseFloat, parseFloat_01_larger_than_long_max) {
	mock << "602200000000000000000000.00";
	DOUBLES_EQUAL(6.022e23f, mock.parseFloat(), APPROX_MARGIN(6.022e23f));
}

TEST(Stream_parseFloat, parseFloat_02_float) {
	mock << "12.34";
	DOUBLES_EQUAL(12.34f, mock.parseFloat(SKIP_NONE), 0.0);
	STRCMP_EQUAL("", mock.readString().c_str());
}

TEST(Stream_parseFloat, parseFloat_02_after_letters) {
	mock << "abcdef12.34";
	DOUBLES_EQUAL(0.0, mock.parseFloat(SKIP_NONE), 0.0);
	STRCMP_EQUAL("abcdef12.34", mock.readString().c_str());
}

TEST(Stream_parseFloat, parseFloat_02_after_whitespace) {
	mock << "\r\n\t 12.34";
	DOUBLES_EQUAL(0.0, mock.parseFloat(SKIP_NONE), 0.0);
	STRCMP_EQUAL("\r\n\t 12.34", mock.readString().c_str());
}

TEST(Stream_parseFloat, parseFloat_03_after_whitespace) {
	mock << "\r\n\t 12.34";
	DOUBLES_EQUAL(12.34f, mock.parseFloat(SKIP_WHITESPACE), 0.0);
	STRCMP_EQUAL("", mock.readString().c_str());
}

TEST(Stream_parseFloat, parseFloat_04_float) {
	mock << "12.34";
	DOUBLES_EQUAL(12.34f, mock.parseFloat(SKIP_ALL, 'a'), 0.0);
	STRCMP_EQUAL("", mock.readString().c_str());
}

TEST(Stream_parseFloat, parseFloat_04_only_ignore_chars) {
	mock << "12a.3a4a";
	DOUBLES_EQUAL(12.34f, mock.parseFloat(SKIP_ALL, 'a'), 0.0);
	STRCMP_EQUAL("", mock.readString().c_str());
}

TEST(Stream_parseFloat, parseFloat_04_other_chars) {
	mock << "1bed234";
	DOUBLES_EQUAL(1.0f, mock.parseFloat(SKIP_ALL, 'a'), 0.0);
	STRCMP_EQUAL("bed234", mock.readString().c_str());
}
