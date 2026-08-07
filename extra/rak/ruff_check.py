# Copyright (c) 2026 RAKwireless Technology Limited
# Author: sercan.erat@rakwireless.com
# SPDX-License-Identifier: Apache-2.0

"""Lint Python sources with ruff.

Usage:
    west rak-ruff                            # files changed since rak-main
    west rak-ruff -v                         # also list the files checked

Complements rak-black-check: black only enforces formatting, ruff
catches real defects (unused imports, undefined names, ...). Rule
configuration lives in .ruff.toml at the repository root.
"""

import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import rak_utils as utils

TARGET_EXTS = {".py"}


class RuffCheck(utils.CheckCommand):
    def __init__(self):
        super().__init__(
            "rak-ruff",
            "lint Python sources (ruff)",
            "Checks Python source files using 'ruff check'.",
        )

    def do_run(self, args, unknown_args):
        files, ignored = self.select_files(args.verbose, extensions=TARGET_EXTS)

        # Run ruff from west's own interpreter so PATH does not matter.
        # --quiet drops the 'All checks passed!' banner; diagnostics are
        # still printed.
        cmd = [sys.executable, "-m", "ruff", "check", "--quiet"]
        utils.run_tool(cmd, name=self.name, files=files, ignored=ignored)
