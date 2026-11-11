# Copyright (c) 2026 RAKwireless Technology Limited
# Author: sercan.erat@rakwireless.com
# SPDX-License-Identifier: Apache-2.0

# Included by each test project in test/; Twister passes RAK_BOARD and RAK_VARIANT with -x

get_filename_component(sketch_name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
set(firmware ${CMAKE_CURRENT_LIST_DIR}/../../firmwares/zephyr-${RAK_VARIANT})

# Twister reads zephyr/.config after every CMake run and, with the
# default harness, the ELF in zephyr/ after the build
configure_file(${firmware}.config zephyr/.config COPYONLY)

add_custom_target(sketch ALL
  COMMAND arduino-cli compile -b rak:zephyr:${RAK_BOARD} --warnings more
          --build-path sketch ${CMAKE_CURRENT_SOURCE_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy sketch/${sketch_name}.ino.elf zephyr/
)

add_custom_target(run
  COMMAND ${CMAKE_COMMAND} -E touch ${QEMU_PIPE}
  COMMAND qemu-system-arm -machine mps2-an386 -nographic
          -icount shift=7,align=off,sleep=off -rtc clock=vm
          -pidfile qemu.pid
          -chardev pipe,id=con,mux=on,path=${QEMU_PIPE}
          -serial chardev:con -mon chardev=con,mode=readline
          -kernel ${firmware}.bin
          -device loader,file=sketch/${sketch_name}.ino.elf-zsk.bin,addr=0x20380000
  USES_TERMINAL
)
