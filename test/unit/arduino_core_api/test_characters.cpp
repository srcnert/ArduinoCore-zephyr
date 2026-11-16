/*
 * Copyright (c) 2026 RAKwireless Technology Limited
 * Author: sercan.erat@rakwireless.com
 * SPDX-License-Identifier: Apache-2.0
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

TEST_GROUP(Characters){};

TEST(Characters, classify) {
	CHECK(isAlpha('a'));
	CHECK_FALSE(isAlpha('1'));
	CHECK(isAlphaNumeric('7'));
	CHECK_FALSE(isAlphaNumeric('-'));
	CHECK(isDigit('7'));
	CHECK_FALSE(isDigit('x'));
	CHECK(isHexadecimalDigit('F'));
	CHECK_FALSE(isHexadecimalDigit('G'));
	CHECK(isSpace(' '));
	CHECK(isWhitespace('\t'));
	CHECK(isUpperCase('Q'));
	CHECK(isLowerCase('q'));
	CHECK(isPunct('!'));
	CHECK(isPrintable('a'));
	CHECK(isControl('\n'));
}

TEST(Characters, convert) {
	LONGS_EQUAL('A', toUpperCase('a'));
	LONGS_EQUAL('z', toLowerCase('Z'));
}
