/*
 * Copyright (c) 2026 RAKwireless Technology Limited
 * Author: sercan.erat@rakwireless.com
 * SPDX-License-Identifier: Apache-2.0
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>
#include <math.h>
#include <stdio.h>

// volatile keeps the compiler from computing the result at build time
static volatile double a = 0.5;

static const char *format(const char *fmt, double value) {
	static char buf[48];

	snprintf(buf, sizeof(buf), fmt, value);
	return buf;
}

TEST_GROUP(Formatting) {
	char buf[64];
};

TEST(Formatting, integers) {
	snprintf(buf, sizeof(buf), "%d %u %x %X %o", -5, 7u, 255, 255, 8);
	STRCMP_EQUAL("-5 7 ff FF 10", buf);
	snprintf(buf, sizeof(buf), "%ld %lld %lu", 123456L, -1234567890123LL, 4000000000UL);
	STRCMP_EQUAL("123456 -1234567890123 4000000000", buf);
	snprintf(buf, sizeof(buf), "[%5d|%-5d|%05d]", 42, 42, 42);
	STRCMP_EQUAL("[   42|42   |00042]", buf);
}

TEST(Formatting, strings) {
	snprintf(buf, sizeof(buf), "%s %c %% %.3s", "str", 'c', "abcdef");
	STRCMP_EQUAL("str c % abc", buf);
	sprintf(buf, "%d-%s", 7, "x");
	STRCMP_EQUAL("7-x", buf);
}

TEST(Formatting, floating_point) {
	STRCMP_EQUAL("3.141593", format("%f", 3.14159265));
	STRCMP_EQUAL("3.14", format("%.2f", 3.14159265));
	STRCMP_EQUAL("4", format("%.0f", 3.7));
	STRCMP_EQUAL("   1.500", format("%8.3f", 1.5));
	STRCMP_EQUAL("1.500   ", format("%-8.3f", 1.5));
	STRCMP_EQUAL("-001.500", format("%08.3f", -1.5));
	STRCMP_EQUAL("+2.0", format("%+.1f", 2.0));
	STRCMP_EQUAL("1.234500e+03", format("%e", 1234.5));
	STRCMP_EQUAL("0.0001234", format("%g", 0.0001234));
}

TEST(Formatting, special_values) {
	STRCMP_EQUAL("nan", format("%f", (double)NAN));
	STRCMP_EQUAL("-inf", format("%f", log(0.0 * a)));
}
