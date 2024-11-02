#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

TEST_FILE="${1:-tests/inputs/t27.txt}"

if ! make -s; then
  echo "[error] Build failed. See the Makefile dependency message above." >&2
  exit 1
fi

# Headless mode avoids opening the GTK window (useful for CI and batch tests).
export MICRORECIF_HEADLESS=1

mkdir -p tmp
OUT_BASENAME="$(basename "$TEST_FILE")"

set +e
LC_ALL=fr_FR.UTF-8 LANG=fr_FR.UTF-8 ./projet "$TEST_FILE" >"tmp/${OUT_BASENAME}.stdout" 2>"tmp/${OUT_BASENAME}.stderr"
RC=$?
set -e

echo "[run.sh] $TEST_FILE -> exit code $RC"
exit 0
