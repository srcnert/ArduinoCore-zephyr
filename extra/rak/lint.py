# Copyright (c) 2026 RAKwireless Technology Limited
# Author: sercan.erat@rakwireless.com
# SPDX-License-Identifier: Apache-2.0

"""Run cppcheck static analysis on C/C++ sources.

Usage:
    west rak-lint -b build/<variant>         # files changed since rak-main
    west rak-lint -b build/<variant> -v      # also list the files checked
"""

import json
import os
import shlex
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import rak_utils as utils
from west import log

TARGET_EXTS = {".c", ".h", ".cpp", ".hpp"}
SKIP_FILES = {"llext_exports.c"}

CPPCHECK_ARGS = [
    "--enable=style,performance,portability",
    "--error-exitcode=1",
    "--quiet",
    "--inline-suppr",
    # Vendored ArduinoCore-API headers; kept in sync with upstream, not ours to fix.
    "--suppress=*:*/cores/arduino/api/*",
    # Only read/used under Kconfig combinations other than the built one.
    "--suppress=unreadVariable:*/loader/main.c",
    "--suppress=unusedStructMember:*/loader/main.c",
    # Zephyr's LOG related
    "--suppress=invalidPointerCast",
    "--suppress=duplicateExpressionTernary",
    "--suppress=literalWithCharPtrCompare",
]


def _compile_db_entries(build_dir):
    """Map resolved source paths to compile_commands.json entries."""
    db_path = build_dir / "compile_commands.json"
    if not db_path.is_file():
        log.die(f"No compile_commands.json in {build_dir}; run the build first.")
    entries = {}
    for entry in json.loads(db_path.read_text()):
        entries.setdefault(Path(entry["file"]).resolve(), entry)
    return db_path, entries


def _gcc_builtins_header(build_dir, entry):
    """Dump the cross-gcc builtin macros for the build's target flags.

    cppcheck does not know the compiler's predefined macros
    (__GNUC__, __BYTE_ORDER__, __ARM_FEATURE_DSP, ...), which Zephyr
    headers require; generate them with 'gcc -dM -E' into a header that
    gets force-included.
    """
    tokens = shlex.split(entry["command"])
    cc, mflags = tokens[0], [t for t in tokens[1:] if t.startswith("-m")]
    header = build_dir / "rak-lint-gcc-builtins.h"
    try:
        result = subprocess.run(
            [cc, *mflags, "-dM", "-E", "-x", "c", os.devnull],
            check=True,
            capture_output=True,
            text=True,
        )
    except (FileNotFoundError, subprocess.CalledProcessError) as e:
        log.die(f"Cannot dump builtin macros with '{cc}': {e}")
    header.write_text(result.stdout)
    return header


def _autoconf_header(build_dir):
    """Locate the generated Kconfig header ('-imacros' is lost on cppcheck)."""
    for rel in (
        "zephyr/include/generated/zephyr/autoconf.h",
        "zephyr/include/generated/autoconf.h",
    ):
        autoconf = build_dir / rel
        if autoconf.is_file():
            return autoconf
    log.die(f"No generated autoconf.h found under {build_dir}.")


def _external_suppressions(build_dir):
    """Silence findings in code we do not own (Zephyr, HALs, generated)."""
    args = [f"--suppress=*:{build_dir}/*"]
    for sibling in sorted(utils.REPO_ROOT.parent.iterdir()):
        if sibling.is_dir() and sibling != utils.REPO_ROOT:
            args.append(f"--suppress=*:{sibling}/*")
    return args


class Lint(utils.CheckCommand):
    def __init__(self):
        super().__init__(
            "rak-lint",
            "run cppcheck on changed C/C++ files",
            "Performs static analysis using 'cppcheck' against the real "
            "headers of a Zephyr build.",
        )

    def do_add_parser(self, parser_adder):
        parser = super().do_add_parser(parser_adder)
        parser.add_argument(
            "-b",
            "--build-dir",
            required=True,
            help="Zephyr build directory providing compile_commands.json "
            "and the generated headers",
        )
        return parser

    def do_run(self, args, unknown_args):
        files, ignored = self.select_files(
            args.verbose, extensions=TARGET_EXTS, ignore_names=SKIP_FILES
        )

        build_dir = Path(args.build_dir).resolve()
        db_path, entries = _compile_db_entries(build_dir)

        in_db = []
        for f in files:
            if f in entries:
                in_db.append(f)
            else:
                log.wrn(f"Not in compile_commands.json, skipping: {f}")
        ignored += len(files) - len(in_db)
        if not in_db:
            log.wrn("No changed files in this build, nothing to check.")
            log.inf(utils.MSG_SUCCESS)
            return

        cmd = (
            ["cppcheck"]
            + CPPCHECK_ARGS
            + _external_suppressions(build_dir)
            + [
                f"--project={db_path}",
                f"--include={_gcc_builtins_header(build_dir, entries[in_db[0]])}",
                f"--include={_autoconf_header(build_dir)}",
            ]
        )

        def check_one(f):
            try:
                run = subprocess.run(cmd + [f"--file-filter={f}"], check=False)
            except FileNotFoundError:
                log.err(f"Executable '{cmd[0]}' not found. Please install it.")
                log.die(utils.MSG_ERR)
            return run.returncode == 0

        utils.check_files(self.name, in_db, check_one, ignored)
