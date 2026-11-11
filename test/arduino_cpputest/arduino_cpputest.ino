/*
 * Copyright (c) 2026 RAKwireless Technology Limited
 * Author: sercan.erat@rakwireless.com
 * SPDX-License-Identifier: Apache-2.0
 */

#include <cpputest_sketch.h>

TEST_GROUP(Framework){};

TEST(Framework, checks) {
	CHECK(true);
	LONGS_EQUAL(42, 40 + 2);
	DOUBLES_EQUAL(1.5, 3.0 / 2.0, 1e-9);
	STRCMP_EQUAL("abc", "abc");
}

// Change IGNORE_TEST to TEST to see how a failure is reported
IGNORE_TEST(Framework, failure) {
	FAIL("This test fails on purpose");
}

TEST_GROUP(Arduino){};

TEST(Arduino, string) {
	String text("Arduino");

	text += 1;
	STRCMP_EQUAL("Arduino1", text.c_str());
}

TEST(Arduino, math) {
	LONGS_EQUAL(50, map(5, 0, 10, 0, 100));
	LONGS_EQUAL(10, constrain(15, 0, 10));
}

TEST(Arduino, time) {
	unsigned long start = millis();

	delay(20);
	CHECK(millis() - start >= 20);
}
