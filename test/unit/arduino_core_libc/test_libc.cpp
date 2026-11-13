/*
 * Copyright (c) 2026 RAKwireless Technology Limited
 * Author: sercan.erat@rakwireless.com
 * SPDX-License-Identifier: Apache-2.0
 */

#include <Arduino.h>
#include <CppUTest/TestHarness.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// The XSI strerror_r() that std::error_code calls
extern "C" int __xpg_strerror_r(int errnum, char *buf, size_t len);

TEST_GROUP(Libc){};

TEST(Libc, sscanf) {
	int i1 = 0;
	int i2 = 0;
	unsigned int hex = 0;
	char token[16] = "";
	float f1 = 0.0f;

	LONGS_EQUAL(4, sscanf("42 -7 ff word", "%d %d %x %15s", &i1, &i2, &hex, token));
	LONGS_EQUAL(42, i1);
	LONGS_EQUAL(-7, i2);
	UNSIGNED_LONGS_EQUAL(0xff, hex);
	STRCMP_EQUAL("word", token);
	LONGS_EQUAL(1, sscanf("3.25", "%f", &f1));
	DOUBLES_EQUAL(3.25, f1, 1e-6);
}

TEST(Libc, string_to_number) {
	DOUBLES_EQUAL(2500.0, strtod("2.5e3", NULL), 1e-9);
	DOUBLES_EQUAL(-1.5, atof("-1.5"), 1e-12);
	LONGS_EQUAL(31, strtol("0x1F", NULL, 16));
	UNSIGNED_LONGS_EQUAL(4000000000UL, strtoul("4000000000", NULL, 10));
	LONGS_EQUAL(123, atoi("123"));
	LONGS_EQUAL(-99999, atol("-99999"));
}

TEST(Libc, string_functions) {
	char text[32];

	strcpy(text, "abc");
	strcat(text, "def");
	strncpy(text + 6, "XYZ", 4);
	STRCMP_EQUAL("abcdefXYZ", text);
	LONGS_EQUAL(9, strlen(text));
	CHECK(strcmp("abc", "abe") < 0);
	LONGS_EQUAL(0, strncmp("abc", "abe", 2));
	LONGS_EQUAL(0, strcasecmp("RAK", "rak"));
	LONGS_EQUAL(0, strncasecmp("RAKx", "raky", 3));
	STRCMP_EQUAL("defXYZ", strchr(text, 'd'));
	STRCMP_EQUAL("/c", strrchr("a/b/c", '/'));
	STRCMP_EQUAL("fXYZ", strstr(text, "fX"));
	POINTERS_EQUAL(text + 6, memmem(text, 9, "XY", 2));
	POINTERS_EQUAL(text + 4, memchr(text, 'e', 9));
	memmove(text + 1, text, 3);
	MEMCMP_EQUAL("aabc", text, 4);
	memset(text, '-', 3);
	LONGS_EQUAL('-', text[2]);
}

TEST(Libc, strtok) {
	char list[] = "a,b,c";
	int tokens = 0;

	for (char *tok = strtok(list, ","); tok != NULL; tok = strtok(NULL, ",")) {
		tokens++;
	}
	LONGS_EQUAL(3, tokens);
}

TEST(Libc, heap) {
	char *dup = strdup("copy");

	STRCMP_EQUAL("copy", dup);
	free(dup);

	char *mem = (char *)malloc(8);

	CHECK(mem != NULL);
	strcpy(mem, "heap");
	mem = (char *)realloc(mem, 32);
	CHECK(mem != NULL);
	strcat(mem, "-ok");
	STRCMP_EQUAL("heap-ok", mem);
	free(mem);

	int *zeros = (int *)calloc(4, sizeof(int));

	CHECK(zeros != NULL);
	LONGS_EQUAL(0, zeros[0]);
	LONGS_EQUAL(0, zeros[3]);
	free(zeros);

	int *numbers = new int[4];

	numbers[3] = 7;
	LONGS_EQUAL(7, numbers[3]);
	delete[] numbers;
}

TEST(Libc, time) {
	struct tm tm = {};

	tm.tm_year = 2026 - 1900;
	tm.tm_mon = 10;
	tm.tm_mday = 3;
	tm.tm_hour = 12;
	tm.tm_min = 34;
	tm.tm_sec = 56;

	time_t t = mktime(&tm);
	struct tm *g = gmtime(&t);

	LONGS_EQUAL(1793709296L, (long)t);
	CHECK(g != NULL);
	LONGS_EQUAL(12, g->tm_hour);
	LONGS_EQUAL(34, g->tm_min);
	LONGS_EQUAL(56, g->tm_sec);
}

TEST(Libc, strerror) {
	char msg[48];

	STRCMP_EQUAL("No such file or directory", strerror(ENOENT));
	__xpg_strerror_r(EINVAL, msg, sizeof(msg));
	STRCMP_EQUAL("Invalid argument", msg);
}
