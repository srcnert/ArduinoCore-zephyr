# Copyright (c) 2026 RAKwireless Technology Limited
# Author: sercan.erat@rakwireless.com
# SPDX-License-Identifier: Apache-2.0

"""Check files against the repository's .editorconfig rules.

Usage:
    west rak-editorconfig-check              # files changed since rak-main
    west rak-editorconfig-check -v           # also list the files checked

Requires the 'editorconfig-checker' pip package (installed via
extra/rak/requirements.txt), which provides the 'ec' executable.
"""

import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import rak_utils as utils


class EditorConfigCheck(utils.CheckCommand):
    def __init__(self):
        super().__init__(
            "rak-editorconfig-check",
            "validate files against .editorconfig",
            "Runs 'editorconfig-checker' on the files changed since rak-main.",
        )

    def do_run(self, args, unknown_args):
        files, ignored = self.select_files(args.verbose)
        # 'ec' is the executable installed by the 'editorconfig-checker'
        # pip package (see extra/rak/requirements.txt).
        utils.run_tool(["ec"], name=self.name, files=files, ignored=ignored)
