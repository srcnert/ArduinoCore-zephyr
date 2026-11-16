/*
 * Copyright (c) 2026 RAKwireless Technology Limited
 * Author: sercan.erat@rakwireless.com
 * SPDX-License-Identifier: Apache-2.0
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>
#include <math.h>

// volatile keeps the compiler from computing the results at build time
static volatile double a = 0.5;
static volatile double b = 2.0;
static volatile double c = 0.75;
static volatile float af = 0.5f;
static volatile float bf = 2.0f;

TEST_GROUP(Math){};

TEST(Math, trigonometry) {
	DOUBLES_EQUAL(0.479425538604203, sin(a), 1e-12);
	DOUBLES_EQUAL(0.8775825618903728, cos(a), 1e-12);
	DOUBLES_EQUAL(0.5463024898437905, tan(a), 1e-12);
	DOUBLES_EQUAL(0.5235987755982989, asin(a), 1e-12);
	DOUBLES_EQUAL(1.0471975511965979, acos(a), 1e-12);
	DOUBLES_EQUAL(0.4636476090008061, atan(a), 1e-12);
	DOUBLES_EQUAL(0.24497866312686414, atan2(a, b), 1e-12);
}

TEST(Math, exponential) {
	DOUBLES_EQUAL(1.6487212707001282, exp(a), 1e-12);
	DOUBLES_EQUAL(1.4142135623730951, exp2(a), 1e-12);
	DOUBLES_EQUAL(-0.6931471805599453, log(a), 1e-12);
	DOUBLES_EQUAL(-1.0, log2(a), 1e-12);
	DOUBLES_EQUAL(-0.3010299956639812, log10(a), 1e-12);
	DOUBLES_EQUAL(1.4142135623730951, pow(b, a), 1e-12);
}

TEST(Math, other) {
	DOUBLES_EQUAL(1.4142135623730951, sqrt(b), 1e-12);
	DOUBLES_EQUAL(0.5, fmod(b, c), 1e-12);
	DOUBLES_EQUAL(4.0, ldexp(a, 3), 1e-12);
}

TEST(Math, float_functions) {
	DOUBLES_EQUAL(0.479425538604203, sinf(af), 1e-6);
	DOUBLES_EQUAL(0.8775825618903728, cosf(af), 1e-6);
	DOUBLES_EQUAL(0.5463024898437905, tanf(af), 1e-6);
	DOUBLES_EQUAL(0.5235987755982989, asinf(af), 1e-6);
	DOUBLES_EQUAL(1.0471975511965979, acosf(af), 1e-6);
	DOUBLES_EQUAL(0.4636476090008061, atanf(af), 1e-6);
	DOUBLES_EQUAL(0.24497866312686414, atan2f(af, bf), 1e-6);
	DOUBLES_EQUAL(-0.6931471805599453, logf(af), 1e-6);
	DOUBLES_EQUAL(1.4142135623730951, sqrtf(bf), 1e-6);
}
