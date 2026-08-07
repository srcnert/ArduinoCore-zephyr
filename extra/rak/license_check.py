# Copyright (c) 2026 RAKwireless Technology Limited
# Author: sercan.erat@rakwireless.com
# SPDX-License-Identifier: Apache-2.0

"""Check that files changed since rak-main carry a proper license header.

Usage:
    west rak-license-check                     # files changed since rak-main
    west rak-license-check -v                  # also list the files checked

Files added or modified between rak-main and HEAD are checked unless
their suffix is in IGNORE_SUFFIXES. Checked files must contain an
'SPDX-License-Identifier:' line and a copyright line within the first
30 lines. Every copyright line mentioning RAKwireless or Arduino must
match the official wording exactly.
"""

import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import rak_utils as utils

IGNORE_SUFFIXES = {".md", ".rst", ".txt", ".pde", ".json"}
# Never checked, matched by exact file name (dotfiles have no suffix).
IGNORE_NAMES = {".gitignore"}
# Checked even though their suffix is ignored.
SOURCE_NAMES = {"CMakeLists.txt"}

# The official wordings are composed from COPYRIGHT_MARK so that these
# source lines never literally contain the mark themselves: _check_file
# scans the first HEADER_LINES lines of every file, including this one.
COPYRIGHT_MARK = "Copyright (c)"
RAK_COPYRIGHT_FORMAT = f"{COPYRIGHT_MARK} <year> RAKwireless Technology Limited"
RAK_COPYRIGHT_RE = re.compile(
    r"Copyright \(c\) \d{4}(-\d{4})? RAKwireless Technology Limited"
)
ARDUINO_COPYRIGHT = f"{COPYRIGHT_MARK} Arduino s.r.l. and/or its affiliated companies"

HEADER_LINES = 30


class LicenseCheck(utils.CheckCommand):
    def __init__(self):
        super().__init__(
            "rak-license-check",
            "check license headers of changed files",
            "Checks that files changed since rak-main carry a copyright + "
            "SPDX header with the official RAKwireless/Arduino wording.",
        )

    def do_run(self, args, unknown_args):
        files, ignored = self.select_files(
            args.verbose,
            include_names=SOURCE_NAMES,
            ignore_names=IGNORE_NAMES,
            ignore_suffixes=IGNORE_SUFFIXES,
        )
        utils.check_files(self.name, files, _check_file, ignored=ignored)


def _error(path, message):
    # GitHub Actions annotation; the file path must be repo-relative or
    # GitHub cannot attach it to the PR diff.
    try:
        path = Path(path).relative_to(utils.REPO_ROOT)
    except ValueError:
        pass
    print(f"::error file={path},line=1::{message}")


def _check_file(path):
    """Return True if the file header is acceptable."""
    with open(path, encoding="utf-8", errors="replace") as f:
        header = [line for _, line in zip(range(HEADER_LINES), f)]

    if not any("SPDX-License-Identifier:" in line for line in header):
        _error(path, "Missing 'SPDX-License-Identifier:' header")
        return False

    # Only lines with the actual declaration mark count: matching on the
    # bare word "copyright" would also catch prose that merely talks
    # about copyright lines (e.g. this file's own docstring).
    copyright_lines = [line for line in header if COPYRIGHT_MARK in line]
    if not copyright_lines:
        _error(path, f"Missing '{COPYRIGHT_MARK}' line")
        return False

    ok = True
    for line in copyright_lines:
        lowered = line.lower()
        if "rak" in lowered and not RAK_COPYRIGHT_RE.search(line):
            _error(path, f"RAKwireless copyright must match: '{RAK_COPYRIGHT_FORMAT}'")
            ok = False
        if "arduino" in lowered and ARDUINO_COPYRIGHT not in line:
            _error(path, f"Arduino copyright must be exactly: '{ARDUINO_COPYRIGHT}'")
            ok = False

    return ok
