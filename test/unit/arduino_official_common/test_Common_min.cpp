/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/Common/test_min.cpp
 * typeid() needs RTTI, which this build disables, so min_04 compares types with same_type.
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

TEST_GROUP(Common_min){};

TEST(Common_min, min_01_both_positive) {
	LONGS_EQUAL(1, min(1, 5));
}

TEST(Common_min, min_01_a_negative) {
	LONGS_EQUAL(-1, min(-1, 5));
}

TEST(Common_min, min_01_both_negative) {
	LONGS_EQUAL(-5, min(-5, -1));
}

TEST(Common_min, min_02_both_positive) {
	LONGS_EQUAL(1, min(5, 1));
}

TEST(Common_min, min_02_b_negative) {
	LONGS_EQUAL(-1, min(5, -1));
}

TEST(Common_min, min_02_both_negative) {
	LONGS_EQUAL(-5, min(-1, -5));
}

TEST(Common_min, min_03_positive) {
	LONGS_EQUAL(5, min(5, 5));
}

TEST(Common_min, min_03_negative) {
	LONGS_EQUAL(-5, min(-5, -5));
}

TEST(Common_min, min_03_zero) {
	LONGS_EQUAL(0, min(0, 0));
}

TEST(Common_min, min_04_uint8_uint16) {
	uint8_t const a = 32;
	uint16_t const b = 10;
	CHECK((same_type<decltype(min(a, b)), int>::value));
}

TEST(Common_min, min_04_uint16_uint32) {
	uint16_t const a = 32;
	uint32_t const b = 10;
	CHECK((same_type<decltype(min(a, b)), unsigned int>::value));
}

TEST(Common_min, min_04_uint32_uint64) {
	uint32_t const a = 32;
	uint64_t const b = 10;
	CHECK((same_type<decltype(min(a, b)), uint64_t>::value));
}

TEST(Common_min, min_04_int8_int16) {
	int8_t const a = -32;
	int16_t const b = -10;
	CHECK((same_type<decltype(min(a, b)), int>::value));
}

TEST(Common_min, min_04_int16_int32) {
	int16_t const a = -32;
	int32_t const b = -10;
	CHECK((same_type<decltype(min(a, b)), int>::value));
}

TEST(Common_min, min_04_int32_int64) {
	int32_t const a = -32;
	int64_t const b = -10;
	CHECK((same_type<decltype(min(a, b)), int64_t>::value));
}
