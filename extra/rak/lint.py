# Copyright (c) 2026 RAKwireless Technology Limited
# Author: sercan.erat@rakwireless.com
# SPDX-License-Identifier: Apache-2.0

"""Run cppcheck static analysis on C/C++ sources.

Usage:
    west rak-lint                            # files changed since rak-main
    west rak-lint -v                         # also list the files checked
"""

import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import rak_utils as utils

TARGET_EXTS = {".c", ".h", ".cpp", ".hpp"}
SKIP_FILES = {"llext_exports.c"}

CPPCHECK_ARGS = [
    "--enable=style,performance,portability",
    "--error-exitcode=1",
    "--quiet",
    "--force",
    "--inline-suppr",
]


class Lint(utils.CheckCommand):
    def __init__(self):
        super().__init__(
            "rak-lint",
            "run cppcheck on changed C/C++ files",
            "Performs static analysis using 'cppcheck'.",
        )

    def do_run(self, args, unknown_args):
        files, ignored = self.select_files(
            args.verbose, extensions=TARGET_EXTS, ignore_names=SKIP_FILES
        )

        cmd = ["cppcheck"] + CPPCHECK_ARGS
        utils.run_tool(cmd, name=self.name, files=files, ignored=ignored)
