/*
 * Copyright (c) Arduino s.r.l. and/or its affiliated companies
 * Copyright (c) 2026 RAKwireless Technology Limited
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Converted from Catch2 to CppUTest: ArduinoCore-API 1.5.2 test/src/String/test_comparisonFunc.cpp
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>

TEST_GROUP(String_comparisonFunc){};

TEST(String_comparisonFunc, equals_01) {
	arduino::String str1("Hello"), str2("Hello");
	CHECK(str1.equals(str2));
}

TEST(String_comparisonFunc, equals_01_operator_eq) {
	arduino::String str1("Hello"), str2("Hello");
	CHECK(str1 == str2);
}

TEST(String_comparisonFunc, equals_01_operator_ne) {
	arduino::String str1("Hello"), str2("Hello");
	CHECK_FALSE(str1 != str2);
}

TEST(String_comparisonFunc, equals_02) {
	arduino::String str1("Hello"), str2("World");
	CHECK_FALSE(str1.equals(str2));
}

TEST(String_comparisonFunc, equals_02_operator_eq) {
	arduino::String str1("Hello"), str2("World");
	CHECK_FALSE(str1 == str2);
}

TEST(String_comparisonFunc, equals_02_operator_ne) {
	arduino::String str1("Hello"), str2("World");
	CHECK(str1 != str2);
}

TEST(String_comparisonFunc, equals_03) {
	arduino::String str1("Hello");
	CHECK(str1.equals("Hello"));
}

TEST(String_comparisonFunc, equals_03_operator_eq) {
	arduino::String str1("Hello");
	CHECK(str1 == "Hello");
}

TEST(String_comparisonFunc, equals_03_operator_ne) {
	arduino::String str1("Hello");
	CHECK_FALSE(str1 != "Hello");
}

TEST(String_comparisonFunc, equals_04) {
	arduino::String str1("Hello");
	CHECK_FALSE(str1.equals("World"));
}

TEST(String_comparisonFunc, equals_04_operator_eq) {
	arduino::String str1("Hello");
	CHECK_FALSE(str1 == "World");
}

TEST(String_comparisonFunc, equals_04_operator_ne) {
	arduino::String str1("Hello");
	CHECK(str1 != "World");
}

TEST(String_comparisonFunc, equalsIgnoreCase_05) {
	arduino::String str1("Hello"), str2("Hello");
	CHECK(str1.equalsIgnoreCase(str2));
}

TEST(String_comparisonFunc, equalsIgnoreCase_06) {
	arduino::String str1("Hello"), str2("Hel"); // codespell:ignore
	CHECK_FALSE(str1.equalsIgnoreCase(str2));
}

TEST(String_comparisonFunc, equalsIgnoreCase_07) {
	arduino::String str1("Hello"), str2("World");
	CHECK_FALSE(str1.equalsIgnoreCase(str2));
}

TEST(String_comparisonFunc, equalsIgnoreCase_08) {
	arduino::String str1("Hello");
	CHECK(str1.equalsIgnoreCase(str1));
}

TEST(String_comparisonFunc, startsWith_09_str2_larger) {
	arduino::String str1("Hello");
	arduino::String str2("Hello World");
	CHECK_FALSE(str1.startsWith(str2));
}

TEST(String_comparisonFunc, startsWith_09_match) {
	arduino::String str1("Hello World");
	arduino::String str2("Hello");
	CHECK(str1.startsWith(str2));
}

TEST(String_comparisonFunc, startsWith_09_no_match) {
	arduino::String str1("Hello World");
	arduino::String str2("Helo");
	CHECK_FALSE(str1.startsWith(str2));
}

TEST(String_comparisonFunc, endsWith_10_str2_larger) {
	arduino::String str1("Hello");
	arduino::String str2("Hello World");
	CHECK_FALSE(str1.endsWith(str2));
}

TEST(String_comparisonFunc, endsWith_10_match) {
	arduino::String str1("Hello World");
	arduino::String str2("World");
	CHECK(str1.endsWith(str2));
}

TEST(String_comparisonFunc, endsWith_10_no_match) {
	arduino::String str1("Hello World");
	arduino::String str2("Helo");
	CHECK_FALSE(str1.endsWith(str2));
}
