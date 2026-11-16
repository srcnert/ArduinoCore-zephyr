/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/Common/test_makeWord.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

TEST_GROUP(Common_makeWord){};

TEST(Common_makeWord, makeWord_01) {
	LONGS_EQUAL(0xDEAD, makeWord(0xDEAD));
	LONGS_EQUAL(0x00DE, makeWord(0xDE));
}

TEST(Common_makeWord, makeWord_02) {
	LONGS_EQUAL(0xDEAD, makeWord(0xDE, 0xAD));
}

TEST(Common_makeWord, makeWord_03) {
	LONGS_EQUAL(0xDEAD, word(0xDEAD));
	LONGS_EQUAL(0xDEAD, word(0xDE, 0xAD));
}
