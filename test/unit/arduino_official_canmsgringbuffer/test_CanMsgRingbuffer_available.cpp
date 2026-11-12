/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2
 * test/src/CanMsgRingbuffer/test_available.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>
#include <api/CanMsgRingbuffer.h>

TEST_GROUP(CanMsgRingbuffer_available){};

TEST(CanMsgRingbuffer_available, available_01) {
	arduino::CanMsgRingbuffer ringbuffer;
	LONGS_EQUAL(0, ringbuffer.available());
}

TEST(CanMsgRingbuffer_available, available_02) {
	arduino::CanMsgRingbuffer ringbuffer;
	arduino::CanMsg msg;
	ringbuffer.enqueue(msg);
	LONGS_EQUAL(1, ringbuffer.available());
	ringbuffer.enqueue(msg);
	LONGS_EQUAL(2, ringbuffer.available());
	ringbuffer.dequeue();
	LONGS_EQUAL(1, ringbuffer.available());
}
