# Copyright (c) 2026 RAKwireless Technology Limited
# Author: sercan.erat@rakwireless.com
# SPDX-License-Identifier: Apache-2.0

set(CPPUTEST_DIR ${ZEPHYR_BASE}/../modules/lib/cpputest)

file(GLOB CPPUTEST_SOURCES
  ${CPPUTEST_DIR}/src/CppUTest/*.cpp
  ${CPPUTEST_DIR}/src/Platforms/Gcc/*.cpp
)

zephyr_include_directories(${CPPUTEST_DIR}/include .)
zephyr_compile_definitions(
  CPPUTEST_STD_CPP_LIB_DISABLED
  CPPUTEST_MEM_LEAK_DETECTION_DISABLED
)

zephyr_library()
zephyr_library_sources(${CPPUTEST_SOURCES} cpputest_loader.c)
zephyr_library_compile_options($<$<COMPILE_LANGUAGE:CXX>:-fno-threadsafe-statics>)
