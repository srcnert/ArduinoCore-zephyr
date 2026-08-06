# Copyright (c) 2026 RAKwireless Technology Limited
# Author: sercan.erat@rakwireless.com
# SPDX-License-Identifier: Apache-2.0

"""Check trailing newlines in source and config files.

Usage:
    west rak-newline-check                   # files changed since rak-main
    west rak-newline-check -v                # also list the files checked
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


class NewlineCheck(utils.CheckCommand):
    def __init__(self):
        super().__init__(
            "rak-newline-check",
            "ensure files end with a newline",
            "Checks for missing trailing newlines in source and config files.",
        )

    def do_run(self, args, unknown_args):
        files, ignored = self.select_files(args.verbose, extensions=TARGET_EXTS)
        utils.check_files(self.name, files, _check_file, ignored=ignored)


def _check_file(path):
    """Return True if the file is empty or ends with a newline."""
    try:
        with open(path, "rb") as f:
            f.seek(0, 2)
            if f.tell() == 0:
                return True
            f.seek(-1, 2)
            if f.read(1) == b"\n":
                return True
    except OSError as e:
        log.wrn(f"Could not read {path}: {e}")
        return True

    log.err(f"Missing trailing newline: {path}")
    return False
