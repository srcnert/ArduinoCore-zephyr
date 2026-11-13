/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/Ringbuffer/test_isFull.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>
#include <api/RingBuffer.h>

TEST_GROUP(Ringbuffer_isFull) {
	arduino::RingBufferN<2> ringbuffer;
};

TEST(Ringbuffer_isFull, isFull_01) {
	CHECK_FALSE(ringbuffer.isFull());
}

TEST(Ringbuffer_isFull, isFull_02) {
	ringbuffer.store_char('A');
	CHECK_FALSE(ringbuffer.isFull());
}

TEST(Ringbuffer_isFull, isFull_03) {
	ringbuffer.store_char('A');
	ringbuffer.store_char('B');
	CHECK(ringbuffer.isFull());
}
