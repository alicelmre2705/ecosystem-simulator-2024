#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

if ! make -s; then
  echo "[error] Build failed. See the Makefile dependency message above." >&2
  exit 1
fi

export MICRORECIF_HEADLESS=1

mkdir -p tmp
rm -f tmp/*.stdout tmp/*.stderr 2>/dev/null || true

TOTAL=0
CRASHED=0
NONZERO=0

shopt -s nullglob
for f in tests/inputs/t*.txt; do
  ((TOTAL++)) || true
  base="$(basename "$f")"

  set +e
  LC_ALL=fr_FR.UTF-8 LANG=fr_FR.UTF-8 ./projet "$f" >"tmp/${base}.stdout" 2>"tmp/${base}.stderr"
  rc=$?
  set -e

  # Treat signals / segfault-like exits as failures.
  if [ "$rc" -ge 128 ]; then
    echo "[FAIL] $base exited with $rc (possible crash/signal)"
    ((CRASHED++)) || true
  elif [ "$rc" -ne 0 ]; then
    echo "[WARN] $base exited with $rc (non-zero, may be expected for invalid inputs)"
    ((NONZERO++)) || true
  else
    echo "[OK]   $base"
  fi

done

echo ""
echo "Summary: $TOTAL test files" 
echo "  OK/WARN: $((TOTAL-CRASHED))" 
echo "  WARN (non-zero): $NONZERO" 
echo "  FAIL (crash/signal): $CRASHED"

# Fail only on crashes/signals.
if [ "$CRASHED" -ne 0 ]; then
  exit 1
fi
