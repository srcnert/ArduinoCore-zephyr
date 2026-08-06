# Copyright (c) 2026 RAKwireless Technology Limited
# Author: sercan.erat@rakwireless.com
# SPDX-License-Identifier: Apache-2.0

"""Check C/C++ formatting with clang-format.

Usage:
    west rak-clang-format                   # files changed since rak-main
    west rak-clang-format -v                # also list the files checked

Uses the repository's .clang-format style file.
"""

import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import rak_utils as utils

TARGET_EXTS = {".c", ".h", ".cpp", ".hpp"}
SKIP_FILES = {"llext_exports.c"}


class ClangFormat(utils.CheckCommand):
    def __init__(self):
        super().__init__(
            "rak-clang-format",
            "check C/C++ coding style",
            "Checks C/C++ source files using 'clang-format'.",
        )

    def do_run(self, args, unknown_args):
        files, ignored = self.select_files(
            args.verbose, extensions=TARGET_EXTS, ignore_names=SKIP_FILES
        )

        cmd = ["clang-format", "--style=file", "-n", "--Werror"]
        utils.run_tool(cmd, name=self.name, files=files, ignored=ignored)
