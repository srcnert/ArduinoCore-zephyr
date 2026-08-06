# Copyright (c) 2026 RAKwireless Technology Limited
# Author: sercan.erat@rakwireless.com
# SPDX-License-Identifier: Apache-2.0

"""Check commit messages between rak-main and HEAD.

Usage:
    west rak-commit-check                    # commits since rak-main

Every commit subject must match '<scope>: <description>' and every
message line must stay between 10 and 80 characters. Merge commits are
skipped. The rak-commit-check CI workflow runs this same command on
pull requests.
"""

import re
import sys
from pathlib import Path

from west import log
from west.commands import WestCommand

sys.path.insert(0, str(Path(__file__).resolve().parent))
import rak_utils as utils

SUBJECT_RE = re.compile(r"^[^!]+: [A-Za-z]+.+ .+$")
MIN_LEN = 10
MAX_LEN = 80


class CommitCheck(WestCommand):
    def __init__(self):
        super().__init__(
            "rak-commit-check",
            "check commit message format",
            "Checks that commits since rak-main follow the "
            "'<scope>: <description>' format and line length rules.",
        )

    def do_add_parser(self, parser_adder):
        return parser_adder.add_parser(
            self.name, help=self.help, description=self.description
        )

    def do_run(self, args, unknown_args):
        shas = utils._git_lines(
            "rev-list", "--no-merges", f"{utils.BASE_REF}..HEAD", cwd=utils.REPO_ROOT
        )
        if not shas:
            log.wrn("No commits found, nothing to check.")
            return

        log.inf(f"Running {self.name} on {len(shas)} commit(s)...")

        failed = 0
        for sha in shas:
            errors = _check_commit(sha)
            for error in errors:
                log.err(f"{sha[:8]}: {error}")
            if errors:
                failed += 1

        log.inf(
            f"Checked {len(shas)} commit(s): {len(shas) - failed} passed, "
            f"{failed} failed."
        )
        if failed:
            log.die(utils.MSG_ERR)
        log.inf(utils.MSG_SUCCESS)


def _check_commit(sha):
    lines = utils._git_lines("log", "-1", "--format=%B", sha, cwd=utils.REPO_ROOT)
    subject = lines[0] if lines else ""

    errors = []
    if not SUBJECT_RE.match(subject):
        errors.append(f"subject must match '<scope>: <description>': '{subject}'")
    for line in lines:
        if line.startswith("#"):
            continue
        if not MIN_LEN <= len(line) <= MAX_LEN:
            errors.append(
                f"line must be {MIN_LEN}-{MAX_LEN} characters "
                f"(is {len(line)}): '{line}'"
            )
    return errors
