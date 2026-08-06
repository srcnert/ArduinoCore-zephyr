# Copyright (c) 2026 RAKwireless Technology Limited
# Author: sercan.erat@rakwireless.com
# SPDX-License-Identifier: Apache-2.0

"""Check that no unexpected binary files are being added.

Usage:
    west rak-binary-check                    # files changed since rak-main
    west rak-binary-check -v                 # also list the files checked

Build outputs and vendored blobs are already excluded by the common
directory filter (build/, firmwares/, loader/blobs, ...); anything else
that looks binary is rejected. Documentation image formats are allowed
via ALLOWED_SUFFIXES.

A file is considered binary when its first chunk contains a NUL byte,
the same heuristic git itself uses.
"""

import sys
from pathlib import Path

from west import log

sys.path.insert(0, str(Path(__file__).resolve().parent))
import rak_utils as utils

# Documentation images are the only binary content expected in commits.
ALLOWED_SUFFIXES = {".gif", ".ico", ".jpeg", ".jpg", ".png"}

CHUNK_SIZE = 8192


class BinaryCheck(utils.CheckCommand):
    def __init__(self):
        super().__init__(
            "rak-binary-check",
            "check for unexpected binary files",
            "Checks that changed files do not add unexpected binaries.",
        )

    def do_run(self, args, unknown_args):
        files, ignored = self.select_files(
            args.verbose, ignore_suffixes=ALLOWED_SUFFIXES
        )
        utils.check_files(self.name, files, _check_file, ignored=ignored)


def _check_file(path):
    """Return True if the file does not look binary."""
    try:
        with open(path, "rb") as f:
            chunk = f.read(CHUNK_SIZE)
    except OSError as e:
        log.wrn(f"Could not read {path}: {e}")
        return True

    if b"\0" in chunk:
        log.err(f"Binary file: {path}")
        return False
    return True
