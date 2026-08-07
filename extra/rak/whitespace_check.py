# Copyright (c) 2026 RAKwireless Technology Limited
# Author: sercan.erat@rakwireless.com
# SPDX-License-Identifier: Apache-2.0

"""Check for trailing whitespace in source and config files.

Usage:
    west rak-whitespace-check                # files changed since rak-main
    west rak-whitespace-check -v             # also list the files checked
"""

import sys
from pathlib import Path

from west import log

sys.path.insert(0, str(Path(__file__).resolve().parent))
import rak_utils as utils

TARGET_EXTS = {
    ".c",
    ".h",
    ".cpp",
    ".hpp",
    ".dts",
    ".dtsi",
    ".overlay",
    ".conf",
    ".py",
    ".sh",
    ".yml",
    ".yaml",
    ".cmake",
    ".txt",
    ".rst",
    ".md",
}


class WhitespaceCheck(utils.CheckCommand):
    def __init__(self):
        super().__init__(
            "rak-whitespace-check",
            "check for trailing whitespace",
            "Checks for trailing whitespace in source and config files.",
        )

    def do_run(self, args, unknown_args):
        files, ignored = self.select_files(args.verbose, extensions=TARGET_EXTS)
        utils.check_files(self.name, files, _check_file, ignored=ignored)


def _check_file(path):
    """Return True if no line of the file ends with trailing whitespace."""
    try:
        lines = path.read_bytes().splitlines()
    except OSError as e:
        log.wrn(f"Could not read {path}: {e}")
        return True

    ok = True
    for lineno, line in enumerate(lines, 1):
        if line != line.rstrip():
            log.err(f"Trailing whitespace: {path}:{lineno}")
            ok = False
    return ok
