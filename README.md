# MicroReef — Algae / Coral / Scavenger Simulation (C++17 + GTKmm 4)

A small **2D ecosystem simulation** (“drop of water”) with three lifeforms:
- **Algae**
- **Corals**
- **Scavengers**

The simulation comes with a **GTKmm 4 GUI** and a set of **input files** (`tests/inputs/t*.txt`) that initialize the world.

The original assignment statement is in French: `docs/00_Instructions.pdf`.

Additional internal notes were translated to English and are available in `docs/`. The original French versions are preserved in `docs/fr/`.

---

## Recruiter Quickstart (60 seconds)

1) **Install dependencies** (pick your OS section below)

2) **Build**
```bash
make
```

3) **Run (GUI)**
```bash
./projet tests/inputs/t27.txt
```

4) **Run the full input sweep (headless, no GUI)**
```bash
make test
```

---

## What you’ll see

![GUI](docs/screenshots/gui.png)

- Green dots = algae, blue polylines = corals, red circles = scavengers.
- Left panel: controls (open / save / step / run / stop) and live entity counts.

---

## Repository layout

```text
.
├── src/                  # C++ sources
├── include/              # C++ headers
├── tests/inputs/         # Input test files (t*.txt)
├── tests/README.md       # How to run inputs/tests
├── docs/                 # Assignment PDF (FR) + English notes (FR originals in docs/fr/)
├── scripts/              # Helper scripts (run, batch tests)
└── .github/workflows/    # CI (Ubuntu)
```

---

## Requirements

- A C++17 compiler (`g++` or `clang++`)
- `make`
- `pkg-config`
- **GTKmm 4.0** development package (provides `gtkmm-4.0` for `pkg-config`)

Sanity check once installed:
```bash
pkg-config --modversion gtkmm-4.0
```

---

## Install dependencies

### macOS (Homebrew)

1) Install Homebrew (if needed):
```bash
brew --version
```
If `brew` is missing, install it from the Homebrew website.

2) Install dependencies:
```bash
brew update
brew install pkg-config make gcc gtkmm4
```

3) Verify GTKmm is visible:
```bash
pkg-config --modversion gtkmm-4.0
```

> **Note (macOS)**: if `make` behaves oddly, try Homebrew’s `gmake`:
```bash
gmake clean && gmake
```

### Linux (Ubuntu/Debian)

```bash
sudo apt-get update
sudo apt-get install -y g++ make pkg-config libgtkmm-4.0-dev
```

Verify:
```bash
pkg-config --modversion gtkmm-4.0
```

> Other distros: install the equivalent GTKmm 4 development package + `pkg-config`.

### Windows

GTKmm 4 on native Windows can be painful to set up reliably. The recommended path is:

#### Option A (recommended): WSL2 + Ubuntu
1) Install **WSL2** and the **Ubuntu** distribution.
2) In the Ubuntu terminal, run the Linux install commands above.
3) Build and run from WSL.

> GUI note: for a GTK GUI under WSL, you’ll need an X server (Windows 10) or WSLg (Windows 11). If you only want to validate inputs, `make test` runs in headless mode.

#### Option B (advanced): MSYS2
Possible, but toolchain/package names change often. Prefer WSL2 unless you already know MSYS2 + GTKmm.

---

## Build

From the repository root:
```bash
make
```

Output:
- `./projet` (executable)

Useful targets:
```bash
make help
make clean
```

---

## Run (GUI)

Run with a sample input file:
```bash
./projet tests/inputs/t27.txt
```

Or:
```bash
make run
# Optionally:
make run TEST=tests/inputs/t00.txt
```

Behavior:
- If a filename is provided, it initializes the simulation from that file.
- If no filename is provided, the GUI starts and you can open a file from the interface.

---

## Tests / batch input sweep (headless)

Because the program normally opens a GTK window, batch execution would block.
For CI and quick regression checks, the repository provides an **optional headless mode**:

```bash
export MICRORECIF_HEADLESS=1
./projet tests/inputs/t27.txt
```

- This **does not change default behavior**.
- It simply avoids opening the GUI when the environment variable is set.

Run all inputs:
```bash
make test
# or
./scripts/run_all_tests.sh
```

Expected output:
```
52 test files, OK: 52, FAIL: 0
```

Outputs (`stdout` / `stderr`) are stored under `tmp/` (ignored by git).

More details: `tests/README.md`.

---

## Scripts

- `scripts/run.sh [path/to/input.txt]` — runs a single input in headless mode
- `scripts/run_all_tests.sh` — runs all `tests/inputs/t*.txt` in headless mode

If you cloned this repo and scripts are not executable:
```bash
chmod +x scripts/*.sh
```

---

## Troubleshooting

### `Error: gtkmm-4.0 not found via pkg-config`
- macOS: `brew install pkg-config gtkmm4`
- Ubuntu/Debian: `sudo apt-get install -y pkg-config libgtkmm-4.0-dev`

Then verify:
```bash
pkg-config --modversion gtkmm-4.0
```

### `pkg-config: command not found`
Install it:
- macOS: `brew install pkg-config`
- Ubuntu/Debian: `sudo apt-get install -y pkg-config`

### `./projet: No such file or directory`
- Ensure you built successfully: `make`
- Ensure you are at the repository root (where the `Makefile` is)

### macOS: `ld: library not found` / runtime library issues
Homebrew libraries may require your terminal to see Homebrew’s paths. Try:
```bash
brew doctor
```
Then open a new terminal and rebuild.

---

## License

No explicit open-source license was provided with the original project.
This repository is published as **All Rights Reserved** by default (see `LICENSE`).

---

## Author

- **Alice Lemaire**
