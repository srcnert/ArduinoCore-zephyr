# Copyright (c) 2026 RAKwireless Technology Limited
# Author: sercan.erat@rakwireless.com
# SPDX-License-Identifier: Apache-2.0

"""Check Python code formatting with black.

Usage:
    west rak-black-check                   # files changed since rak-main
    west rak-black-check -v                # also list files and show diffs
"""

import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import rak_utils as utils

TARGET_EXTS = {".py"}


class BlackCheck(utils.CheckCommand):
    def __init__(self):
        super().__init__(
            "rak-black-check",
            "check Python code formatting",
            "Checks Python source files using 'black'.",
        )

    def do_run(self, args, unknown_args):
        files, ignored = self.select_files(args.verbose, extensions=TARGET_EXTS)

        # Run black from west's own interpreter so PATH does not matter.
        # --quiet drops black's per-file cheering; failures are named by
        # run_tool via fail_msg instead.
        cmd = [sys.executable, "-m", "black", "--check", "--quiet"]
        if args.verbose:
            cmd.append("--diff")
        utils.run_tool(
            cmd,
            name=self.name,
            files=files,
            ignored=ignored,
            fail_msg="Would reformat",
        )
