#!/usr/bin/env bash

# Copyright (c) 2026 RAKwireless Technology Limited
# Author: sercan.erat@rakwireless.com
# SPDX-License-Identifier: Apache-2.0

# Twister --flash-command for the test sketches: loads the sketch that
# extra/rak/twister.cmake built with pyOCD.
#
# Usage: --flash-command extra/rak/twister_flash.sh,<pyocd_target>,<sketch_address>
# Twister puts --build-dir <dir> (and --board-id <id>) before these two.

set -eu

while [ $# -gt 0 ]; do
	case $1 in
	--build-dir) build_dir=$2; shift 2 ;;
	--board-id) shift 2 ;;
	*) break ;;
	esac
done

[ -n "${build_dir:-}" ] || { echo "no --build-dir in: $*" >&2; exit 1; }

sketch=("$build_dir"/sketch/*.ino.elf-zsk.bin)
[ -f "${sketch[0]}" ] || { echo "sketch binary not found in $build_dir/sketch" >&2; exit 1; }

pyocd load -t "$1" -a "$2" "${sketch[0]}"
