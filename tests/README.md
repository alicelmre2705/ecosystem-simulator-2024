# Tests / Input Files

This repository includes a set of **input files** used to initialize the simulation world.

Location:
- `tests/inputs/t*.txt`

These files come from the original assignment and are primarily meant to:
- exercise parsing,
- validate that the program runs without crashing,
- initialize a variety of scenarios.

## What “success” means

There is no official `expected output` folder shipped with this repository.
So the default “test sweep” is a **regression / crash-free run**:

- ✅ Exit code `0` is considered OK.
- ⚠️ Non-zero exit codes may happen if an input is intentionally invalid — they are reported as warnings.
- ❌ Exit codes `>= 128` are treated as potential crashes/signals (e.g., segfault) and fail the sweep.

All `stdout` / `stderr` outputs are captured into `tmp/` (ignored by git).

## Run a single input (headless)

```bash
./scripts/run.sh tests/inputs/t27.txt
```

This script:
- builds the project if needed,
- sets `MICRORECIF_HEADLESS=1` (so the GUI does not open),
- stores outputs in `tmp/`.

## Run all inputs

```bash
make test
# or:
./scripts/run_all_tests.sh
```

The batch script:
- runs all `tests/inputs/t*.txt` in a stable order,
- stores `stdout` / `stderr` under `tmp/`,
- prints a per-file status line (OK/WARN/FAIL).

## Tips

- If scripts are not executable after cloning:
  ```bash
  chmod +x scripts/*.sh
  ```
- To manually run headless without scripts:
  ```bash
  export MICRORECIF_HEADLESS=1
  ./projet tests/inputs/t27.txt
  ```
