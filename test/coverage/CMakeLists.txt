# Copyright (c) 2026 RAKwireless Technology Limited
# Author: sercan.erat@rakwireless.com
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

# get value of NORMALIZED_BOARD_TARGET early
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE} COMPONENTS yaml boards)

set(VARIANT_DIR ${CMAKE_CURRENT_LIST_DIR}/../../variants/${NORMALIZED_BOARD_TARGET})
set(DTC_OVERLAY_FILE ${VARIANT_DIR}/${NORMALIZED_BOARD_TARGET}.overlay)
set(EXTRA_CONF_FILE ${VARIANT_DIR}/${NORMALIZED_BOARD_TARGET}.conf)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(coverage)

# The TEST()s of every test project, run by main.cpp
file(GLOB test_sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_LIST_DIR}/../unit/*/*.cpp)
target_sources(app PRIVATE main.cpp ${test_sources})

# As in the sketch build (loader/CMakeLists.txt, platform.txt)
target_compile_definitions(app PRIVATE _XOPEN_SOURCE=700 _GNU_SOURCE)
target_include_directories(app PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/../../cores/arduino/api/deprecated-avr-comp
  ${CMAKE_CURRENT_LIST_DIR}/../unit/common
)

zephyr_compile_options(-Wno-unused-variable -Wno-comment)
