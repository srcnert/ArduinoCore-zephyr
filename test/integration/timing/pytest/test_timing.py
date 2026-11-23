# Copyright (c) 2026 RAKwireless Technology Limited
# Author: sercan.erat@rakwireless.com
# SPDX-License-Identifier: Apache-2.0

"""Host side of the timing sketch: the CppUTest TESTs on the board do the checks,
this answers the "@clock" requests of timing.cpp and reports each TEST as its own
pytest test, so Twister lists them one by one."""

import re
import time
from pathlib import Path

import pytest
from twister_harness import DeviceAdapter

SKETCH_DIR = Path(__file__).parents[1]
TEST = re.compile(r"(IGNORE_)?TEST\((\w+), (\w+)\)")
SKIP = pytest.mark.skip(reason="IGNORE_TEST")
TESTS = [
    pytest.param(f"{m[2]}.{m[3]}", marks=SKIP if m[1] else ())
    for source in sorted(SKETCH_DIR.glob("*.cpp"))
    for line in source.read_text().splitlines()
    if (m := TEST.match(line))
]


@pytest.fixture(scope="module")
def results(dut: DeviceAdapter) -> dict[str, list[str]]:
    """Runs the sketch to its result line, returns the output of each TEST."""
    results = {}
    name = None
    start = time.monotonic_ns()
    while not (line := dut.readline(timeout=60)).startswith("PROJECT EXECUTION"):
        if line == "@clock":
            dut.write(f"{(time.monotonic_ns() - start) // 1000}\n".encode())
        elif match := TEST.match(line):
            name = f"{match[2]}.{match[3]}"
            results[name] = []
        elif name:
            results[name].append(line)
    return results


@pytest.mark.parametrize("name", TESTS)
def test_timing(results: dict[str, list[str]], name: str):
    assert name in results, "did not run"
    output = results[name]
    assert not any("error: Failure in" in line for line in output), "\n".join(output)
