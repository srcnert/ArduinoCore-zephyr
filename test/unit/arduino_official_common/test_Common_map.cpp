/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/Common/test_map.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

TEST_GROUP(Common_map){};

TEST(Common_map, map_01) {
	LONGS_EQUAL(5, map(50, 0, 100, 0, 10));
}

TEST(Common_map, map_02) {
	LONGS_EQUAL(50, map(5, 0, 10, 0, 100));
}

TEST(Common_map, map_03) {
	LONGS_EQUAL(0, map(1, 0, 100, 0, 10)); /* Would be 0.1 if we'd be using floating point. */
}

TEST(Common_map, map_04_negative_from) {
	LONGS_EQUAL(0, map(0, 0, -256, 0, 1024));
	LONGS_EQUAL(1024, map(-256, 0, -256, 0, 1024));
	LONGS_EQUAL(512, map(-128, 0, -256, 0, 1024));
}

TEST(Common_map, map_04_negative_to) {
	LONGS_EQUAL(0, map(0, 0, 256, 0, -1024));
	LONGS_EQUAL(-1024, map(256, 0, 256, 0, -1024));
	LONGS_EQUAL(-512, map(128, 0, 256, 0, -1024));
}
