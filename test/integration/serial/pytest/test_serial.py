# Copyright (c) 2026 RAKwireless Technology Limited
# Author: sercan.erat@rakwireless.com
# SPDX-License-Identifier: Apache-2.0

"""Host side of the serial sketch: the CppUTest TESTs on the board do the checks,
this answers the "@<command> <argument>" requests of serial_usb.cpp and reports
each TEST as its own pytest test, so Twister lists them one by one."""

import os
import re
from pathlib import Path

import pytest
from twister_harness import DeviceAdapter

SKETCH_DIR = Path(__file__).parents[1]
TEST = re.compile(r"(IGNORE_)?TEST\((\w+), (\w+)\)")
REQUEST = re.compile(r"@(\w+) (.*)")
SKIP = pytest.mark.skip(reason="IGNORE_TEST")
TESTS = [
    pytest.param(f"{m[2]}.{m[3]}", marks=SKIP if m[1] else ())
    for source in sorted(SKETCH_DIR.glob("*.cpp"))
    for line in source.read_text().splitlines()
    if (m := TEST.match(line))
]


def reply(line: str, expected: str) -> bytes:
    """Answer to "expect" and "pattern": "ok" or where the line differs."""
    if line == expected:
        return b"ok\n"
    index = len(os.path.commonprefix([line, expected]))
    return f"differs at {index}: {line[index:index + 16]!r}\n".encode()


@pytest.fixture(scope="module")
def results(dut: DeviceAdapter) -> dict[str, list[str]]:
    """Runs the sketch to its result line, returns the output of each TEST."""
    results = {}
    name = None
    while not (line := dut.readline(timeout=60)).startswith("PROJECT EXECUTION"):
        if match := TEST.match(line):
            name = f"{match[2]}.{match[3]}"
            results[name] = []
        elif match := REQUEST.fullmatch(line):
            command, argument = match.groups()
            if command == "echo":
                dut.write(argument.encode() + b"\n")
            elif command == "bulk":
                dut.write(bytes(i & 0xFF for i in range(int(argument))))
            elif command == "expect":
                dut.write(reply(dut.readline(timeout=10), argument))
            elif command == "pattern":
                pattern = "".join(chr(ord("a") + i % 26) for i in range(int(argument)))
                dut.write(reply(dut.readline(timeout=10), pattern))
        elif name:
            results[name].append(line)
    return results


@pytest.mark.parametrize("name", TESTS)
def test_serial(results: dict[str, list[str]], name: str):
    assert name in results, "did not run"
    output = results[name]
    assert not any("error: Failure in" in line for line in output), "\n".join(output)
