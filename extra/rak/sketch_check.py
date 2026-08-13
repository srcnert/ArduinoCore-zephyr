# Copyright (c) 2026 RAKwireless Technology Limited
# Author: sercan.erat@rakwireless.com
# SPDX-License-Identifier: Apache-2.0

"""Compile every sketch in sketch/ with arduino-cli.

Usage:
    west rak-sketch-check                    # compile all sketches
    west rak-sketch-check -v                 # verbose compiler output
    west rak-sketch-check -b nrf52840dk      # pick another board

Each direct subdirectory of sketch/ that contains a .ino file is
compiled with 'arduino-cli compile -b rak:zephyr:<board>'.
"""

import subprocess
import sys
from pathlib import Path

from west import log

sys.path.insert(0, str(Path(__file__).resolve().parent))
import rak_utils as utils

DEFAULT_BOARD = "nrf52840dk"

SKETCH_DIR = utils.REPO_ROOT / "sketch"
BOARDS_TXT = utils.REPO_ROOT / "boards.txt"
BOARDS_LOCAL = utils.REPO_ROOT / "boards.local.txt"


class SketchCheck(utils.CheckCommand):
    def __init__(self):
        super().__init__(
            "rak-sketch-check",
            "compile the sketches in sketch/ (arduino-cli)",
            "Compiles every sketch under sketch/ with arduino-cli "
            "against a previously built loader.",
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
        variant = _board_variant(args.board)
        edk = utils.REPO_ROOT / "variants" / variant / "llext-edk"
        if not edk.is_dir():
            log.die(
                f"No llext-edk for variant '{variant}'. "
                "Build the loader first: ./extra/build.sh <target>"
            )

        sketches = _find_sketches()
        if not sketches:
            log.wrn("No sketches found in sketch/, nothing to compile.")
            return

        fqbn = f"rak:zephyr:{args.board}"
        log.inf(f"Compiling {len(sketches)} sketch(es) for {fqbn}...")

        failed = []
        for sketch in sketches:
            log.inf(f"--- {sketch.name} ---")
            cmd = ["arduino-cli", "compile", "-b", fqbn, str(sketch)]
            if args.verbose:
                cmd.append("-v")
            try:
                result = subprocess.run(cmd, cwd=utils.REPO_ROOT, check=False)
            except FileNotFoundError:
                log.err("arduino-cli not found. Please install it.")
                log.die(utils.MSG_ERR)
            if result.returncode != 0:
                log.err(f"Compilation failed: {sketch.name}")
                failed.append(sketch.name)

        log.inf(
            f"Compiled {len(sketches)} sketch(es): "
            f"{len(sketches) - len(failed)} passed, {len(failed)} failed"
        )
        if failed:
            log.die(utils.MSG_ERR)
        log.inf(utils.MSG_SUCCESS)


def _find_sketches():
    """Direct subdirectories of sketch/ containing a .ino file."""
    if not SKETCH_DIR.is_dir():
        return []
    return sorted(
        p for p in SKETCH_DIR.iterdir() if p.is_dir() and any(p.glob("*.ino"))
    )


def _board_variant(board):
    """Map a board name to its variant via boards.local.txt."""
    key = f"{board}.build.variant="
    for file in (BOARDS_LOCAL, BOARDS_TXT):
        if not file.is_file():
            continue
        for line in file.read_text().splitlines():
            if line.startswith(key):
                return line[len(key) :]
    log.die(f"Board '{board}' not found in boards.txt / boards.local.txt")
