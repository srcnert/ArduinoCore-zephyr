# Copyright (c) 2026 RAKwireless Technology Limited
# Author: sercan.erat@rakwireless.com
# SPDX-License-Identifier: Apache-2.0

"""Build the Zephyr samples in samples/ with west build.

Usage:
    west rak-sample-check                          # build the default samples
    west rak-sample-check -b rak4631/nrf52840      # pick another board
    west rak-sample-check -s samples/fade          # build another sample

The sample supplies the variant overlay via its CMakeLists.txt and the Kconfig
fragment via <sample>/boards/<board>.conf. DEFAULT_SAMPLES therefore lists the
samples that ship such a fragment for the RAK boards; it is the single source
of truth for what rak-checkall and the rak-build workflow build.
"""

import subprocess
import sys
from pathlib import Path

from west import log

sys.path.insert(0, str(Path(__file__).resolve().parent))
import rak_utils as utils

DEFAULT_BOARD = "rak4631/nrf52840"
DEFAULT_SAMPLES = ["samples/blinky_arduino", "samples/hello_arduino"]


class SampleCheck(utils.CheckCommand):
    def __init__(self):
        super().__init__(
            "rak-sample-check",
            "build the samples in samples/ (west build)",
            "Builds the selected Zephyr samples with the variant overlay "
            "and Kconfig fragment of the target board.",
        )

    def do_add_parser(self, parser_adder):
        parser = super().do_add_parser(parser_adder)
        parser.add_argument(
            "-b",
            "--board",
            default=DEFAULT_BOARD,
            help=f"west board target (default: {DEFAULT_BOARD})",
        )
        parser.add_argument(
            "-s",
            "--sample",
            action="append",
            metavar="PATH",
            help="sample directory to build, repeatable "
            f"(default: {', '.join(DEFAULT_SAMPLES)})",
        )
        return parser

    def do_run(self, args, unknown_args):
        samples = [utils.REPO_ROOT / s for s in (args.sample or DEFAULT_SAMPLES)]
        for sample in samples:
            if not (sample / "CMakeLists.txt").is_file():
                log.die(f"Not a Zephyr application: {sample}")

        log.inf(f"Building {len(samples)} sample(s) for {args.board}...")

        failed = []
        for sample in samples:
            log.inf(f"--- {sample.name} ---")
            cmd = [
                "west",
                "build",
                "-p",
                "-d",
                str(sample / "build"),
                "-b",
                args.board,
                str(sample),
            ]

            result = subprocess.run(cmd, cwd=utils.REPO_ROOT, check=False)
            if result.returncode != 0:
                log.err(f"Build failed: {sample.name}")
                failed.append(sample.name)

        log.inf(
            f"Built {len(samples)} sample(s): {len(samples) - len(failed)} passed, "
            f"{len(failed)} failed"
        )
        if failed:
            log.die(utils.MSG_ERR)
        log.inf(utils.MSG_SUCCESS)
