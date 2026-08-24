# Copyright (c) Arduino s.r.l. and/or its affiliated companies
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

# get value of NORMALIZED_BOARD_TARGET early
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE} COMPONENTS yaml boards)

set(VARIANT_DIR ${CMAKE_CURRENT_LIST_DIR}/../../variants/${NORMALIZED_BOARD_TARGET})
set(DTC_OVERLAY_FILE ${VARIANT_DIR}/${NORMALIZED_BOARD_TARGET}.overlay)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(hello_world)

target_sources(app PRIVATE src/app.cpp)

# for USB device stack NEXT
target_sources_ifdef(CONFIG_USB_DEVICE_STACK_NEXT app PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../cores/arduino/usb_device_descriptor.c
)

zephyr_compile_options(-Wno-unused-variable -Wno-comment)
