# Copyright (c) 2026 RAKwireless Technology Limited
# Author: sercan.erat@rakwireless.com
# SPDX-License-Identifier: Apache-2.0

"""Check for leftover merge conflict markers.

Usage:
    west rak-conflict-check                  # files changed since rak-main
    west rak-conflict-check -v               # also list the files checked

Only the unambiguous markers are matched (the '<' / '>' / '|' styles at
the start of a line). The '=======' separator is deliberately not
searched for: it would false-positive on Markdown/RST heading
underlines, and it never appears in a conflict without the others.
"""

import sys
from pathlib import Path

from west import log

sys.path.insert(0, str(Path(__file__).resolve().parent))
import rak_utils as utils

MARKERS = (b"<<<<<<< ", b">>>>>>> ", b"|||||||")


class ConflictCheck(utils.CheckCommand):
    def __init__(self):
        super().__init__(
            "rak-conflict-check",
            "check for merge conflict markers",
            "Checks changed files for leftover merge conflict markers.",
        )

    def do_run(self, args, unknown_args):
        files, ignored = self.select_files(args.verbose)
        utils.check_files(self.name, files, _check_file, ignored=ignored)


def _check_file(path):
    """Return True if no line of the file starts with a conflict marker."""
    try:
        lines = path.read_bytes().splitlines()
    except OSError as e:
        log.wrn(f"Could not read {path}: {e}")
        return True

    ok = True
    for lineno, line in enumerate(lines, 1):
        if line.startswith(MARKERS):
            log.err(f"Merge conflict marker: {path}:{lineno}")
            ok = False
    return ok
