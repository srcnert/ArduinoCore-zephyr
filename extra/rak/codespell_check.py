# Copyright (c) 2026 RAKwireless Technology Limited
# Author: sercan.erat@rakwireless.com
# SPDX-License-Identifier: Apache-2.0

"""Check spelling in source and documentation files with codespell.

Usage:
    west rak-codespell                       # files changed since rak-main
    west rak-codespell -v                    # also list the files checked

All changed text files are checked; obvious binary formats are skipped
by suffix. False positives can be silenced per line with
'codespell:ignore' comments or globally via --ignore-words-list in the
command below.
"""

import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import rak_utils as utils

BINARY_SUFFIXES = {
    ".a",
    ".bin",
    ".elf",
    ".gif",
    ".gz",
    ".hex",
    ".ico",
    ".jpeg",
    ".jpg",
    ".o",
    ".pdf",
    ".png",
    ".so",
    ".uf2",
    ".zip",
}


class CodespellCheck(utils.CheckCommand):
    def __init__(self):
        super().__init__(
            "rak-codespell",
            "check spelling (codespell)",
            "Checks source and documentation files using 'codespell'.",
        )

    def do_run(self, args, unknown_args):
        files, ignored = self.select_files(
            args.verbose, ignore_suffixes=BINARY_SUFFIXES
        )

        cmd = ["codespell"]
        utils.run_tool(cmd, name=self.name, files=files, ignored=ignored)
