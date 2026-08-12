# Copyright (c) 2026 RAKwireless Technology Limited
# Author: sercan.erat@rakwireless.com
# SPDX-License-Identifier: Apache-2.0

"""Run every rak-* check in sequence and summarize the results.

Usage:
    west rak-checkall                        # all checks + loader build
    west rak-checkall --quick                # skip the heavy build step
    west rak-checkall -v                     # pass -v to every check
    west rak-checkall -q                     # only show output of failures

The cppcheck lint runs after the loader build, against that build's
real Zephyr headers; --quick skips both (see lint.py).
"""

import subprocess
import sys
from pathlib import Path

from west import log
from west.commands import WestCommand

sys.path.insert(0, str(Path(__file__).resolve().parent))
import rak_utils as utils

# Build target used by the full run
LOADER_BUILD_TARGET = "nrf52840dk/nrf52840"
LOADER_BUILD_DIR = "build/nrf52840dk_nrf52840"

# (description, command, heavy) triples, run in this order from the repo
# root. Heavy entries are skipped with --quick. West commands get -v
# appended when running verbose.
CHECKS = [
    ("binary files", ["west", "rak-binary-check"], False),
    ("Python formatting", ["west", "rak-black-check"], False),
    ("C/C++ formatting", ["west", "rak-clang-format"], False),
    ("spelling (codespell)", ["west", "rak-codespell"], False),
    ("commit messages", ["west", "rak-commit-check"], False),
    ("merge conflict markers", ["west", "rak-conflict-check"], False),
    ("editorconfig compliance", ["west", "rak-editorconfig-check"], False),
    ("license headers", ["west", "rak-license-check"], False),
    ("trailing newlines", ["west", "rak-newline-check"], False),
    ("Python lint (ruff)", ["west", "rak-ruff"], False),
    ("trailing whitespace", ["west", "rak-whitespace-check"], False),
    (
        "loader build " f"({LOADER_BUILD_TARGET})",
        ["./extra/build.sh", LOADER_BUILD_TARGET],
        True,
    ),
    (
        "cppcheck static analysis",
        ["west", "rak-lint", "-b", LOADER_BUILD_DIR],
        True,
    ),
]


class CheckAll(WestCommand):
    def __init__(self):
        super().__init__(
            "rak-checkall",
            "run all rak checks in sequence",
            "Runs every rak-* check (and the loader build) and reports "
            "a PASS/FAIL summary.",
        )

    def do_add_parser(self, parser_adder):
        parser = parser_adder.add_parser(
            self.name, help=self.help, description=self.description
        )
        parser.add_argument(
            "-v",
            "--verbose",
            action="store_true",
            help="pass -v to every check",
        )
        parser.add_argument(
            "-q",
            "--quiet",
            action="store_true",
            help="only show the output of failing checks",
        )
        parser.add_argument(
            "--quick",
            action="store_true",
            help="skip heavy steps (loader build and cppcheck lint)",
        )
        return parser

    def do_run(self, args, unknown_args):
        checks = CHECKS
        if args.quick:
            checks = [c for c in CHECKS if not c[2]]

        root = utils.REPO_ROOT
        failed = []
        total = len(checks)

        for idx, (description, base_cmd, _) in enumerate(checks, 1):
            tag = f"[{idx}/{total}]"
            cmd = list(base_cmd)
            if args.verbose and cmd[0] == "west":
                cmd.append("-v")

            if args.quiet:
                result = subprocess.run(
                    cmd, capture_output=True, text=True, cwd=root, check=False
                )
            else:
                if idx > 1:
                    log.inf("")
                log.inf(f"--- {tag} {description} ({' '.join(base_cmd)}) ---")
                result = subprocess.run(cmd, cwd=root, check=False)

            if result.returncode == 0:
                summary = _summary_line(result.stdout) if args.quiet else None
                suffix = f": {summary}" if summary else ""
                log.inf(f"{tag} \033[32mPASS\033[0m  {description}{suffix}")
                continue

            if args.quiet:
                for stream in (result.stdout, result.stderr):
                    if stream:
                        print(stream, end="")
            log.inf(f"{tag} \033[31mFAIL\033[0m  {description}")
            failed.append(description)

        if failed:
            log.die("\033[31m❌ Failed checks: " + ", ".join(failed) + "\033[0m")
        log.inf(f"\033[32m🎉 All {total} checks passed!\033[0m")


def _summary_line(output):
    """Extract a check's 'Checked ...' summary line from captured output."""
    for line in (output or "").splitlines():
        if line.startswith("Checked "):
            return line
    return None
