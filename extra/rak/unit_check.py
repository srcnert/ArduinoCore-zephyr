# Copyright (c) 2026 RAKwireless Technology Limited
# Author: sercan.erat@rakwireless.com
# SPDX-License-Identifier: Apache-2.0

"""Build and run the unit tests in test/unit with Twister.

Usage:
    west rak-unit-check                   # run the unit tests in QEMU
    west rak-unit-check -b rak4631        # only build them for a board
    west rak-unit-check -v                # verbose Twister output

Twister compiles every project under test/unit as a sketch with
arduino-cli (extra/rak/twister.cmake) against the loader last built for
the board. That loader needs the cpputest snippet:
./extra/build.sh <board> -S cpputest. Twister runs the tests in QEMU on
mps2_an386 (qemu-system-arm); on hardware boards it only builds them.
"""

import subprocess
import sys
from pathlib import Path

from west import log

sys.path.insert(0, str(Path(__file__).resolve().parent))
import rak_utils as utils

DEFAULT_BOARD = "mps2_an386"


class UnitCheck(utils.CheckCommand):
    def __init__(self):
        super().__init__(
            "rak-unit-check",
            "build and run the unit tests in test/unit (Twister)",
            "Builds the unit tests in test/unit with Twister against a "
            "previously built loader and runs them where Twister can.",
        )

    def do_add_parser(self, parser_adder):
        parser = super().do_add_parser(parser_adder)
        parser.add_argument(
            "-b",
            "--board",
            default=DEFAULT_BOARD,
            help=f"board name from boards.local.txt (default: {DEFAULT_BOARD})",
        )
        return parser

    def do_run(self, args, unknown_args):
        variant = utils.board_property(args.board, "build.variant")
        target = utils.board_property(args.board, "build.zephyr_target")
        if not variant or not target:
            log.die(f"Board '{args.board}' not found in boards.txt / boards.local.txt")

        config = utils.REPO_ROOT / "firmwares" / f"zephyr-{variant}.config"
        if not config.is_file() or (
            "CONFIG_RAK_CPPUTEST=y" not in config.read_text().splitlines()
        ):
            log.die(
                f"No loader with the cpputest snippet for '{args.board}'. "
                f"Build it first: ./extra/build.sh {args.board} -S cpputest"
            )

        cmd = [
            "west",
            "twister",
            "-T",
            "test/unit",
            "-p",
            target,
            "-x",
            f"RAK_BOARD={args.board}",
            "-x",
            f"RAK_VARIANT={variant}",
        ]
        if args.verbose:
            cmd.append("-v")

        log.inf(f"Running the unit tests for {target}...")
        result = subprocess.run(cmd, cwd=utils.REPO_ROOT, check=False)
        if result.returncode != 0:
            log.die(utils.MSG_ERR)
        log.inf(utils.MSG_SUCCESS)
