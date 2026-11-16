/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/Common/test_max.cpp
 * typeid() needs RTTI, which this build disables, so max_04 compares types with same_type.
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

namespace {
template <typename A, typename B> struct same_type {
	static const bool value = false;
};

template <typename A> struct same_type<A, A> {
	static const bool value = true;
};
} // namespace

TEST_GROUP(Common_max){};

TEST(Common_max, max_01_both_positive) {
	LONGS_EQUAL(5, max(1, 5));
}

TEST(Common_max, max_01_a_negative) {
	LONGS_EQUAL(5, max(-1, 5));
}

TEST(Common_max, max_01_both_negative) {
	LONGS_EQUAL(-1, max(-5, -1));
}

TEST(Common_max, max_02_both_positive) {
	LONGS_EQUAL(5, max(5, 1));
}

TEST(Common_max, max_02_b_negative) {
	LONGS_EQUAL(5, max(5, -1));
}

TEST(Common_max, max_02_both_negative) {
	LONGS_EQUAL(-1, max(-1, -5));
}

TEST(Common_max, max_03_positive) {
	LONGS_EQUAL(5, max(5, 5));
}

TEST(Common_max, max_03_negative) {
	LONGS_EQUAL(-5, max(-5, -5));
}

TEST(Common_max, max_03_zero) {
	LONGS_EQUAL(0, max(0, 0));
}

TEST(Common_max, max_04_uint8_uint16) {
	uint8_t const a = 32;
	uint16_t const b = 10;
	CHECK((same_type<decltype(max(a, b)), int>::value));
}

TEST(Common_max, max_04_uint16_uint32) {
	uint16_t const a = 32;
	uint32_t const b = 10;
	CHECK((same_type<decltype(max(a, b)), unsigned int>::value));
}

TEST(Common_max, max_04_uint32_uint64) {
	uint32_t const a = 32;
	uint64_t const b = 10;
	CHECK((same_type<decltype(max(a, b)), uint64_t>::value));
}

TEST(Common_max, max_04_int8_int16) {
	int8_t const a = -32;
	int16_t const b = -10;
	CHECK((same_type<decltype(max(a, b)), int>::value));
}

TEST(Common_max, max_04_int16_int32) {
	int16_t const a = -32;
	int32_t const b = -10;
	CHECK((same_type<decltype(max(a, b)), int>::value));
}

TEST(Common_max, max_04_int32_int64) {
	int32_t const a = -32;
	int64_t const b = -10;
	CHECK((same_type<decltype(max(a, b)), int64_t>::value));
}
