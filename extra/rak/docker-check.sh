#!/bin/bash

# Copyright (c) 2026 RAKwireless Technology Limited
# Author: sercan.erat@rakwireless.com
# SPDX-License-Identifier: Apache-2.0

# Run the RAKwireless CI checks in a container, without needing any tool
# (python, black, clang-format, cppcheck, Zephyr SDK) on the host.
#
# Usage:
#   ./extra/rak/docker-check.sh                 # full rak-checkall incl. loader build
#   ./extra/rak/docker-check.sh --quick         # checks only, skip the build
#   ./extra/rak/docker-check.sh rak-lint -v     # any single west command
#
# The whole west workspace (parent of this repo) is mounted into the
# container, so the host checkout including the local rak-main ref and
# the updated zephyr modules are used as-is. Check tools are installed
# from extra/rak/requirements.txt — the same pinned versions the CI
# workflows use. Note this is NOT an exact CI replica: rak-checks runs
# on a bare runner with a fresh workspace, and rak-build uses the
# smaller ci-base image.

set -e

# The 'ci' image bundles the Zephyr SDK needed by the loader build.
IMAGE="zephyrprojectrtos/ci:latest"
CMD=(west rak-checkall)

if [ "$1" = "--quick" ]; then
	# Checks only; the small image without the Zephyr SDK is enough.
	IMAGE="zephyrprojectrtos/ci-base:latest"
	CMD=(west rak-checkall --quick)
	shift
fi
if [ $# -gt 0 ]; then
	CMD=(west "$@")
fi

REPO_ROOT=$(git rev-parse --show-toplevel)
WORKSPACE=$(dirname "$REPO_ROOT")
REPO_DIR=$(basename "$REPO_ROOT")

TTY_FLAGS=""
[ -t 0 ] && TTY_FLAGS="-it"

echo "Image:   $IMAGE"
echo "Command: ${CMD[*]}"

docker run --rm $TTY_FLAGS \
	--user root \
	-v "$WORKSPACE":/workdir \
	-w "/workdir/$REPO_DIR" \
	-e CMAKE_PREFIX_PATH=/opt/toolchains \
	"$IMAGE" \
	bash -lc '
		git config --global --add safe.directory "*"
		pip3 install --quiet --upgrade west -r extra/rak/requirements.txt 2>/dev/null ||
			pip3 install --quiet --upgrade --break-system-packages west \
				-r extra/rak/requirements.txt
		command -v cppcheck >/dev/null ||
			{ apt-get update -qq && apt-get install -y -qq cppcheck; }
		mkdir -p venv/bin && touch venv/bin/activate
		exec "$@"
	' _ "${CMD[@]}"
