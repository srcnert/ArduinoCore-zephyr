# Copyright (c) 2026 RAKwireless Technology Limited
# Author: sercan.erat@rakwireless.com
# SPDX-License-Identifier: Apache-2.0

"""Run every rak-* check in sequence and summarize the results.

Usage:
    west rak-checkall                        # all checks + loader build
    west rak-checkall --quick                # skip the heavy build step
    west rak-checkall -b nrf52840dk          # build for another board
    west rak-checkall -v                     # pass -v to every check
    west rak-checkall -q                     # only show output of failures

The cppcheck lint and the sketch compilation run after the loader
build, against that build's artifacts. The sample build is independent
of the loader but just as heavy, so it runs last; --quick skips all
four.

Those four steps build one board at a time, selected with -b; every
board defined in boards.txt works.
"""

import subprocess
import sys
from pathlib import Path

from west import log
from west.commands import WestCommand

sys.path.insert(0, str(Path(__file__).resolve().parent))
import rak_utils as utils

# Board built by the heavy steps unless -b says otherwise
DEFAULT_BOARD = "rak4631"

# (description, command) pairs, run in this order from the repo root.
# West commands get -v appended when running verbose.
CHECKS = [
    ("binary files", ["west", "rak-binary-check"]),
    ("Python formatting", ["west", "rak-black-check"]),
    ("C/C++ formatting", ["west", "rak-clang-format"]),
    ("spelling (codespell)", ["west", "rak-codespell"]),
    ("commit messages", ["west", "rak-commit-check"]),
    ("merge conflict markers", ["west", "rak-conflict-check"]),
    ("editorconfig compliance", ["west", "rak-editorconfig-check"]),
    ("license headers", ["west", "rak-license-check"]),
    ("trailing newlines", ["west", "rak-newline-check"]),
    ("Python lint (ruff)", ["west", "rak-ruff"]),
    ("trailing whitespace", ["west", "rak-whitespace-check"]),
]


def _build_checks(board):
    """The heavy steps, in the order they must run, for one board.

    The variant (build directory) and the Zephyr board target come from
    the board's definition in boards.txt, so -b takes the same name as
    ./extra/build.sh and arduino-cli.
    """
    variant = utils.board_property(board, "build.variant")
    target = utils.board_property(board, "build.zephyr_target")
    if not variant or not target:
        log.die(f"Board '{board}' not found in boards.txt / boards.local.txt")

    return [
        (f"loader build ({board})", ["./extra/build.sh", board]),
        ("cppcheck static analysis", ["west", "rak-lint", "-b", f"build/{variant}"]),
        (f"sketch compilation ({board})", ["west", "rak-sketch-check", "-b", board]),
        (f"sample build ({target})", ["west", "rak-sample-check", "-b", target]),
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
            help="skip heavy steps (loader build, lint, sketch and sample build)",
        )
        parser.add_argument(
            "-b",
            "--board",
            default=DEFAULT_BOARD,
            metavar="BOARD",
            help="board the heavy steps build for, as named in boards.txt "
            f"(default: {DEFAULT_BOARD})",
        )
        return parser

    def do_run(self, args, unknown_args):
        checks = list(CHECKS)
        if not args.quick:
            checks += _build_checks(args.board)

        root = utils.REPO_ROOT
        failed = []
        total = len(checks)

        for idx, (description, base_cmd) in enumerate(checks, 1):
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
