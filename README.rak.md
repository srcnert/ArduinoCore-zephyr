# RAKwireless Port — Development Setup

This document describes how to set up a development environment for the RAKwireless
downstream port of [ArduinoCore-zephyr](https://github.com/arduino/ArduinoCore-zephyr).

## 1. Repository Setup (origin and upstream)

```shell
mkdir ~/rak-arduino-zephyr && cd ~/rak-arduino-zephyr
git clone https://github.com/srcnert/ArduinoCore-zephyr
cd ArduinoCore-zephyr

# Add Arduino's repository as upstream (to keep main in sync)
git remote add upstream https://github.com/arduino/ArduinoCore-zephyr
git fetch upstream

# Create the working branch
git checkout -b rak-main
```

Verify: `git remote -v` should show both `origin` (fork) and `upstream`
(arduino).

This setup is a common pattern for downstream ports: keep `main` as a pristine
mirror of upstream `arduino/ArduinoCore-zephyr` (so `git pull` syncs cleanly
with no local commits in the way), and do all RAKwireless work on `rak-main`,
periodically merging `main` into it.

## 2. Branch Sync

### Sync `main` with upstream

`main` must only fast-forward; if `--ff-only` fails, a local commit
leaked into `main` and should be moved to a feature branch instead.

Tags must be synced too: GitHub does not copy tags into a fork, and
`extra/get_core_version.sh` derives the core version from `git describe`
— without tags the rak-build CI job fails.

```shell
git fetch upstream --tags
git checkout main
git merge --ff-only upstream/main
git push origin main
git push origin --tags
```

### Bring upstream changes into rak-main
Always merge (never rebase): rak-main is a published, shared branch, and
rebasing it would rewrite history for everyone.

Note that `git merge main` merges your *local* `main`, so run the
"Sync `main` with upstream" steps above first — otherwise you merge a
stale `main`.

```shell
git checkout rak-main
git pull --ff-only
git merge main
# resolve conflicts if any, then:
git push origin rak-main
```

`--ff-only` means the pull only fast-forwards: your local rak-main should
never diverge from origin, and if it somehow has, the pull stops with an
error instead of silently creating a merge commit.

### Update a feature branch

As long as you are the only one working on the branch (pushed or not), rebase
is the cleanest way and keeps the history linear:

```shell
git checkout rak-main
git pull --ff-only
git checkout dev/<feature>
git rebase rak-main
git push --force-with-lease
```

Use `--force-with-lease` (not `--force`): it refuses to push if the remote has
commits you haven't fetched.

Safety nets: `git rebase --abort` restores the pre-rebase state; after a
finished rebase, `git reset --hard ORIG_HEAD` undoes it. Nothing is permanent
until you push.

If the rebase stops on a conflict, resolve it and continue:

```shell
git add <resolved_file>
git rebase --continue
```

If two people actively work on the same feature branch, either
coordinate the rebase (the other side runs `git pull --rebase`) or avoid
rewriting history and update the branch with a merge instead:

```shell
git checkout dev/<feature>
git merge rak-main
```

## 3. West Workspace Setup

```shell
# From the repo root: register the existing clone as the manifest repo
west init -l .
cd ..            # workspace root: ~/rak-arduino-zephyr
west update
```

### Verification (from the workspace root)

```shell
west list | head           # zephyr + modules should be listed
west boards | grep -i rak  # rak4631 should appear
cd ArduinoCore-zephyr
mkdir -p venv/bin && touch venv/bin/activate  # create an empty venv placeholder
# use a target defined in boards.txt
./extra/build.sh nrf52840dk
```

## 4. Memory Footprint Reports

```shell
west build -d build/nrf52840dk_nrf52840 -t footprint
west build -d build/nrf52840dk_nrf52840 -t ram_report
west build -d build/nrf52840dk_nrf52840 -t rom_report
```

To save a report to a file:

```shell
west build -d build/nrf52840dk_nrf52840 -t ram_report > ram_report.txt
```

## 5. Validation on RAK4631 + RAK19007

### Install arduino-cli

```shell
brew install arduino-cli
arduino-cli version   # verify the installation
```

### Install the Arduino Zephyr toolchain (one time)

```shell
arduino-cli core install arduino:zephyr_main@0.90.0
```

The official `arduino:zephyr_main` core supplies the cross compiler
referenced by `boards.txt` (`arm-zephyr-eabi` 0.16.8) and the
post-processing tools (`gen-rodata-ld`, `zephyr-sketch-tool`,
`zephyr-check-size`) that the `{runtime.tools.*}` recipes in
`platform.txt` resolve against.

### Optional: use the in-repo host tools without installing core

To test the in-repo tools instead of the released binaries, build them
locally:

```shell
cd ~/rak-arduino-zephyr/ArduinoCore-zephyr
(cd tools/zephyr-sketch-tool && go build)
(cd tools/gen-rodata-ld && go build)
(cd tools/zephyr-check-size && go build)
```

Each binary is produced inside its own tool directory
(e.g. `tools/zephyr-sketch-tool/zephyr-sketch-tool`).

Then create a `platform.local.txt` file containing copies of the four
recipes from `platform.txt`, rewritten to use the in-repo binaries
instead of `{runtime.tools.*}`.
`arduino-cli` overrides the definitions in `platform.txt` with those in
`platform.local.txt`.

### Optional: use a local compiler via `boards.local.txt`

`./extra/build.sh` generates `boards.local.txt` (gitignored), which
`arduino-cli` merges over `boards.txt`. To compile with a locally
installed toolchain instead of the core-installed one, add:

```text
<board>.build.compiler_path=/path/to/zephyr-sdk/arm-zephyr-eabi/bin/
```

As with `platform.local.txt`, this override means local builds no
longer match CI.

### Register the core with arduino-cli (one time)

```shell
mkdir -p ~/Documents/Arduino/hardware/rak
ln -s ~/Documents/rak-arduino-zephyr/ArduinoCore-zephyr ~/Documents/Arduino/hardware/rak/zephyr
arduino-cli core list
```

The `rak:zephyr` core should now be listed.

### Build a sketch

```shell
cd ~/rak-arduino-zephyr/ArduinoCore-zephyr
./extra/build.sh nrf52840dk
arduino-cli compile -b rak:zephyr:nrf52840dk -e sketch/blinky
```

This compiles `blinky.ino`.

## 6. Building Zephyr Samples

```shell
# One-time workspace setup: the core's CMake expects ArduinoCore-API at the
# workspace root, but west checks it out under modules/lib/
ln -s modules/lib/ArduinoCore-API <workspace-root>/ArduinoCore-API
```

After that, a sample can be built from the ArduinoCore-zephyr directory:

```shell
west build -p -d samples/hello_arduino/build -b nrf52840dk/nrf52840 samples/hello_arduino
```

## 7. How to use CI tools

The `extra/rak/` directory provides west commands that run the same checks as
the CI workflows, so problems can be caught locally before opening a PR.

### Install the required tools

```shell
${config:TOOLCHAIN_BASE}/bin/python3 -m pip install -r extra/rak/requirements.txt
brew install cppcheck  # rak-lint
```

### Local check commands

All commands compare the working tree against the merge-base of `rak-main` and
`HEAD`, so committed, staged, unstaged and untracked changes are all seen.

```shell
west rak-binary-check       # no unexpected binary files
west rak-black-check        # Python formatting (black)
west rak-clang-format       # C/C++ formatting (.clang-format style)
west rak-codespell          # spell check (codespell)
west rak-commit-check       # commit message format (commits since rak-main)
west rak-conflict-check     # no leftover merge conflict markers
west rak-editorconfig-check # .editorconfig compliance
west rak-license-check      # copyright + SPDX header check
west rak-lint -b build/<variant>  # cppcheck static analysis (C/C++)
west rak-newline-check      # files must end with a newline
west rak-ruff               # Python lint (ruff, .ruff.toml rules)
west rak-sketch-check       # compile the sketches in sketch/ (arduino-cli)
west rak-whitespace-check   # no trailing whitespace
west rak-checkall           # all of the above + loader build
                            # (--quick skips build, lint and sketches)
```

`west rak-lint` requires a build directory: it checks the files found in its
`compile_commands.json` against the real Zephyr headers, Kconfig options and
devicetree macros of that build. Changed files not compiled in that build
(e.g. `cores/`, compiled per-sketch) are skipped with a warning. In
`west rak-checkall`, the lint therefore runs right after the loader build.

`west rak-sketch-check` compiles every sketch in `sketch/` with
`arduino-cli compile -b rak:zephyr:<board>` (default board: `nrf52840dk`,
override with `-b`). It needs the setup of section 5: a built loader
(`./extra/build.sh <target>`), the in-repo host tools (`tools/*/go build`)
and the `rak:zephyr` core registered with arduino-cli. In
`west rak-checkall` it runs last, after the loader build.

Every command prints a summary line
(`Checked N file(s): X passed, Y failed - (Z ignored)`) and exits non-zero on
failure. Add `-v` to list the files being checked.

### CI workflows

The checks run automatically on GitHub for every pull request targeting
`rak-main` (nothing runs on local commits):

- `rak-checks` runs every `west rak-*` check (except `rak-lint`) as a
  separate step of a single job, so there is one checkout, one tool
  install (from `extra/rak/requirements.txt`) and one `west init` for
  all of them. Steps keep running after a failure, so a single CI run
  reports every problem at once. Adding a new check means adding one
  step to `.github/workflows/rak-checks.yml`.
- `rak-build` (PR + push to `rak-main`) builds the loader for
  target board in its own workflow: it needs the Zephyr
  toolchain container and takes far longer than the checks. It then
  runs `west rak-lint` against that build's real headers, installs
  arduino-cli and the pinned `arduino:zephyr_main` core, and compiles
  the sketches in `sketch/` against that loader with
  `west rak-sketch-check`.

`rak-commit-check` requires every commit subject to match
`<scope>: <description>` (e.g. `variants: rak4631: add board defs`) and every
message line to be 10–80 characters long.

Upstream sync merges (section 2) are normally pushed directly to `rak-main`
and never see these PR checks. If branch protection ever forces the sync
through a PR, add the `upstream-sync` label to it: `rak-checks` skips labeled
sync PRs (upstream content follows Arduino's conventions, not the RAKwireless rules),
while `rak-build` still runs.

### Running the checks in Docker (CI parity)

To run the checks in the same container images the CI uses (instead of the
host toolchain), use the wrapper script:

```shell
./extra/rak/docker-check.sh              # full rak-checkall incl. loader build
./extra/rak/docker-check.sh --quick      # checks only, skip the build
./extra/rak/docker-check.sh rak-lint -v  # any single west command
```
