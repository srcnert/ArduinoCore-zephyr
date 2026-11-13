/*
 * Copyright (c) 2026 RAKwireless Technology Limited
 * Author: sercan.erat@rakwireless.com
 * SPDX-License-Identifier: Apache-2.0
 */

#include <errno.h>
#include <sys/types.h>
#include <zephyr/llext/symbol.h>

// CppUTest references fopen() for its JUnit output, and picolibc's fopen() needs these

__attribute__((weak)) int open(const char *name, int flags, ...) {
	errno = ENOSYS;
	return -1;
}

__attribute__((weak)) int close(int fd) {
	errno = ENOSYS;
	return -1;
}

__attribute__((weak)) ssize_t read(int fd, void *buf, size_t count) {
	errno = ENOSYS;
	return -1;
}

__attribute__((weak)) ssize_t write(int fd, const void *buf, size_t count) {
	errno = ENOSYS;
	return -1;
}

__attribute__((weak)) off_t lseek(int fd, off_t offset, int whence) {
	errno = ENOSYS;
	return (off_t)-1;
}

// What the CppUTest macros and helpers use from a sketch

#define FORCE_EXPORT_SYM(name)                                                                     \
	extern void name(void);                                                                        \
	EXPORT_SYMBOL(name);

FORCE_EXPORT_SYM(PlatformSpecificFPuts);
FORCE_EXPORT_SYM(_Z10StringFromPKc);
FORCE_EXPORT_SYM(_Z10StringFromPKv);
FORCE_EXPORT_SYM(_Z10StringFromRK12SimpleString);
FORCE_EXPORT_SYM(_Z10StringFromb);
FORCE_EXPORT_SYM(_Z10StringFromc);
FORCE_EXPORT_SYM(_Z10StringFromdi);
FORCE_EXPORT_SYM(_Z10StringFromi);
FORCE_EXPORT_SYM(_Z10StringFromj);
FORCE_EXPORT_SYM(_Z10StringFroml);
FORCE_EXPORT_SYM(_Z10StringFromm);
FORCE_EXPORT_SYM(_Z13HexStringFroml);
FORCE_EXPORT_SYM(_Z13HexStringFromm);
FORCE_EXPORT_SYM(_Z16StringFromFormatPKcz);
FORCE_EXPORT_SYM(_Z16StringFromOrNullPKc);
FORCE_EXPORT_SYM(_Z30BracketsFormattedHexStringFromi);
FORCE_EXPORT_SYM(_ZN10UtestShell10addFailureERK11TestFailure);
FORCE_EXPORT_SYM(_ZN10UtestShell10assertTrueEbPKcS1_S1_S1_jRK14TestTerminator);
FORCE_EXPORT_SYM(_ZN10UtestShell10countCheckEv);
FORCE_EXPORT_SYM(_ZN10UtestShell10countTestsEv);
FORCE_EXPORT_SYM(_ZN10UtestShell10getCurrentEv);
FORCE_EXPORT_SYM(_ZN10UtestShell10runOneTestEP10TestPluginR10TestResult);
FORCE_EXPORT_SYM(_ZN10UtestShell11destroyTestEP5Utest);
FORCE_EXPORT_SYM(_ZN10UtestShell12assertEqualsEbPKcS1_S1_S1_jRK14TestTerminator);
FORCE_EXPORT_SYM(_ZN10UtestShell13assertCompareEbPKcS1_S1_S1_jRK14TestTerminator);
FORCE_EXPORT_SYM(_ZN10UtestShell13setRunIgnoredEv);
FORCE_EXPORT_SYM(_ZN10UtestShell15assertBitsEqualEmmmjPKcS1_jRK14TestTerminator);
FORCE_EXPORT_SYM(_ZN10UtestShell15assertCstrEqualEPKcS1_S1_S1_jRK14TestTerminator);
FORCE_EXPORT_SYM(_ZN10UtestShell16assertCstrNEqualEPKcS1_jS1_S1_jRK14TestTerminator);
FORCE_EXPORT_SYM(_ZN10UtestShell16assertLongsEqualEllPKcS1_jRK14TestTerminator);
FORCE_EXPORT_SYM(_ZN10UtestShell16printVeryVerboseEPKc);
FORCE_EXPORT_SYM(_ZN10UtestShell17assertBinaryEqualEPKvS1_jPKcS3_jRK14TestTerminator);
FORCE_EXPORT_SYM(_ZN10UtestShell18assertCstrContainsEPKcS1_S1_S1_j);
FORCE_EXPORT_SYM(_ZN10UtestShell18assertDoublesEqualEdddPKcS1_jRK14TestTerminator);
FORCE_EXPORT_SYM(_ZN10UtestShell19assertPointersEqualEPKvS1_PKcS3_jRK14TestTerminator);
FORCE_EXPORT_SYM(
	_ZN10UtestShell20assertLongLongsEqualE17cpputest_longlongS0_PKcS2_jRK14TestTerminator);
FORCE_EXPORT_SYM(_ZN10UtestShell21assertCstrNoCaseEqualEPKcS1_S1_S1_j);
FORCE_EXPORT_SYM(_ZN10UtestShell22assertSignedBytesEqualEaaPKcS1_jRK14TestTerminator);
FORCE_EXPORT_SYM(_ZN10UtestShell23setRunInSeperateProcessEv);
FORCE_EXPORT_SYM(_ZN10UtestShell24assertCstrNoCaseContainsEPKcS1_S1_S1_j);
FORCE_EXPORT_SYM(_ZN10UtestShell24assertUnsignedLongsEqualEmmPKcS1_jRK14TestTerminator);
FORCE_EXPORT_SYM(_ZN10UtestShell24getCurrentTestTerminatorEv);
FORCE_EXPORT_SYM(_ZN10UtestShell26runOneTestInCurrentProcessEP10TestPluginR10TestResult);
FORCE_EXPORT_SYM(_ZN10UtestShell27assertFunctionPointersEqualEPFvvES1_PKcS3_jRK14TestTerminator);
FORCE_EXPORT_SYM(
	_ZN10UtestShell28assertUnsignedLongLongsEqualE18cpputest_ulonglongS0_PKcS2_jRK14TestTerminator);
FORCE_EXPORT_SYM(_ZN10UtestShell4failEPKcS1_jRK14TestTerminator);
FORCE_EXPORT_SYM(_ZN10UtestShell5crashEv);
FORCE_EXPORT_SYM(_ZN10UtestShell5printEPKcS1_j);
FORCE_EXPORT_SYM(_ZN10UtestShell5printERK12SimpleStringPKcj);
FORCE_EXPORT_SYM(_ZN10UtestShell7addTestEPS_);
FORCE_EXPORT_SYM(_ZN10UtestShell8exitTestERK14TestTerminator);
FORCE_EXPORT_SYM(_ZN10UtestShell8failWithERK11TestFailure);
FORCE_EXPORT_SYM(_ZN10UtestShell8failWithERK11TestFailureRK14TestTerminator);
FORCE_EXPORT_SYM(_ZN10UtestShellC2Ev);
FORCE_EXPORT_SYM(_ZN10UtestShellD2Ev);
FORCE_EXPORT_SYM(_ZN11TestFailureC1EP10UtestShellPKcjRK12SimpleString);
FORCE_EXPORT_SYM(_ZN11TestFailureC1EP10UtestShellRK12SimpleString);
FORCE_EXPORT_SYM(_ZN11TestFailureD1Ev);
FORCE_EXPORT_SYM(_ZN12SimpleStringC1EPKc);
FORCE_EXPORT_SYM(_ZN12SimpleStringD1Ev);
FORCE_EXPORT_SYM(_ZN12SimpleStringaSERKS_);
FORCE_EXPORT_SYM(_ZN12SimpleStringpLEPKc);
FORCE_EXPORT_SYM(_ZN12SimpleStringpLERKS_);
FORCE_EXPORT_SYM(_ZN13TestInstallerC1ER10UtestShellPKcS3_S3_j);
FORCE_EXPORT_SYM(_ZN13TestInstallerD1Ev);
FORCE_EXPORT_SYM(_ZN17IgnoredUtestShell10runOneTestEP10TestPluginR10TestResult);
FORCE_EXPORT_SYM(_ZN17IgnoredUtestShell13setRunIgnoredEv);
FORCE_EXPORT_SYM(_ZN17IgnoredUtestShellC2Ev);
FORCE_EXPORT_SYM(_ZN17IgnoredUtestShellD2Ev);
FORCE_EXPORT_SYM(_ZN21CommandLineTestRunner11RunAllTestsEiPKPKc);
FORCE_EXPORT_SYM(_ZN5Utest3runEv);
FORCE_EXPORT_SYM(_ZN5Utest5setupEv);
FORCE_EXPORT_SYM(_ZN5Utest8teardownEv);
FORCE_EXPORT_SYM(_ZN5Utest8testBodyEv);
FORCE_EXPORT_SYM(_ZN5UtestC2Ev);
FORCE_EXPORT_SYM(_ZN5UtestD2Ev);
FORCE_EXPORT_SYM(_ZNK10UtestShell12getMacroNameEv);
FORCE_EXPORT_SYM(_ZNK10UtestShell16getFormattedNameEv);
FORCE_EXPORT_SYM(_ZNK10UtestShell22isRunInSeperateProcessEv);
FORCE_EXPORT_SYM(_ZNK10UtestShell7getNextEv);
FORCE_EXPORT_SYM(_ZNK10UtestShell7willRunEv);
FORCE_EXPORT_SYM(_ZNK10UtestShell9hasFailedEv);
FORCE_EXPORT_SYM(_ZNK12SimpleString12asCharStringEv);
FORCE_EXPORT_SYM(_ZNK12SimpleString4sizeEv);
FORCE_EXPORT_SYM(_ZNK12SimpleString7isEmptyEv);
FORCE_EXPORT_SYM(_ZNK12SimpleString8containsERKS_);
FORCE_EXPORT_SYM(_ZNK12SimpleStringplERKS_);
FORCE_EXPORT_SYM(_ZNK17IgnoredUtestShell12getMacroNameEv);
FORCE_EXPORT_SYM(_ZNK17IgnoredUtestShell7willRunEv);
FORCE_EXPORT_SYM(_ZeqRK12SimpleStringS1_);
FORCE_EXPORT_SYM(_ZneRK12SimpleStringS1_);
