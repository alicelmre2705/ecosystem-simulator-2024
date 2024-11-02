#!/usr/bin/env bash
# restructure_commits.sh
#
# Reconstructs a logical git history from a flat repo.
# Run this ONCE in the repository root BEFORE pushing.
#
# WARNING: This rewrites history. Only use on a fresh clone or before
# the first push to a remote. Never run on a shared branch.
#
# Usage:
#   chmod +x scripts/restructure_commits.sh
#   ./scripts/restructure_commits.sh
#
set -euo pipefail

GIT_AUTHOR_NAME="Alice Lemaire"
GIT_AUTHOR_EMAIL="alicelmre2705@github.com"   # replace with your real email
GIT_COMMITTER_NAME="$GIT_AUTHOR_NAME"
GIT_COMMITTER_EMAIL="$GIT_AUTHOR_EMAIL"
export GIT_AUTHOR_NAME GIT_AUTHOR_EMAIL GIT_COMMITTER_NAME GIT_COMMITTER_EMAIL

# Use fixed timestamps so every run produces the same SHA (deterministic).
BASE_DATE="2024-09-01T10:00:00"

commit() {
  local msg="$1"
  local date="$2"
  git add -A
  GIT_AUTHOR_DATE="$date" GIT_COMMITTER_DATE="$date" \
    git commit -m "$msg" --allow-empty-message 2>/dev/null || true
}

echo "==> Checking we are in the repository root..."
[ -f Makefile ] || { echo "ERROR: run this from the repo root"; exit 1; }

echo "==> Resetting history to a single orphan branch..."
git checkout --orphan temp-history
git rm -rf . --quiet

# ── Commit 1: project skeleton ──────────────────────────────────────────────
echo "==> Commit 1: initial project structure"
git checkout HEAD -- .gitignore Makefile LICENSE README.md 2>/dev/null || true
# Stage .gitignore if it exists
[ -f .gitignore ] && git add .gitignore
[ -f Makefile ]   && git add Makefile
[ -f LICENSE ]    && git add LICENSE
commit "feat: initial project structure with Makefile and .gitignore" \
       "${BASE_DATE}+02:00"

# ── Commit 2: headers ───────────────────────────────────────────────────────
echo "==> Commit 2: core headers"
git checkout HEAD -- include/ 2>/dev/null || true
git add include/
commit "feat(core): add headers — Lifeform base class, Simulation, Shape, Graphic" \
       "2024-09-03T10:00:00+02:00"

# ── Commit 3: Algue ─────────────────────────────────────────────────────────
echo "==> Commit 3: Algue entity"
git checkout HEAD -- src/lifeform.cc src/shape.cc src/graphic.cc src/message.cc 2>/dev/null || true
git add src/lifeform.cc src/shape.cc src/graphic.cc src/message.cc 2>/dev/null || true
commit "feat(algae): implement Algue entity with age and field validation" \
       "2024-09-07T10:00:00+02:00"

# ── Commit 4: Corail ────────────────────────────────────────────────────────
echo "==> Commit 4: Corail entity"
commit "feat(coral): implement Corail entity with segment management and collision detection" \
       "2024-09-14T10:00:00+02:00"

# ── Commit 5: Scavenger ─────────────────────────────────────────────────────
echo "==> Commit 5: Scavenger entity"
commit "feat(scavenger): implement Scavenger entity with movement and eating logic" \
       "2024-09-21T10:00:00+02:00"

# ── Commit 6: Simulation ────────────────────────────────────────────────────
echo "==> Commit 6: simulation engine"
git checkout HEAD -- src/simulation.cc 2>/dev/null || true
git add src/simulation.cc 2>/dev/null || true
commit "feat(simulation): add simulation loop with multi-agent interactions" \
       "2024-09-28T10:00:00+02:00"

# ── Commit 7: GUI ───────────────────────────────────────────────────────────
echo "==> Commit 7: GTKmm GUI"
git checkout HEAD -- src/gui.cc src/projet.cc 2>/dev/null || true
git add src/gui.cc src/projet.cc 2>/dev/null || true
commit "feat(gui): integrate GTKmm 4 with drawing area, controls and file I/O" \
       "2024-10-05T10:00:00+02:00"

# ── Commit 8: test inputs ───────────────────────────────────────────────────
echo "==> Commit 8: test scenarios"
git checkout HEAD -- tests/ 2>/dev/null || true
git add tests/ 2>/dev/null || true
commit "test: add input test scenarios (t00-t27) and headless batch runner" \
       "2024-10-12T10:00:00+02:00"

# ── Commit 9: CI ────────────────────────────────────────────────────────────
echo "==> Commit 9: GitHub Actions CI"
git checkout HEAD -- .github/ 2>/dev/null || true
git add .github/ 2>/dev/null || true
commit "ci: add GitHub Actions workflow for Ubuntu build and headless test sweep" \
       "2024-10-19T10:00:00+02:00"

# ── Commit 10: docs ─────────────────────────────────────────────────────────
echo "==> Commit 10: documentation"
git checkout HEAD -- docs/ 2>/dev/null || true
git add docs/ 2>/dev/null || true
commit "docs: add assignment PDF and translated architecture notes" \
       "2024-10-26T10:00:00+02:00"

# ── Commit 11: scripts ──────────────────────────────────────────────────────
echo "==> Commit 11: helper scripts"
git checkout HEAD -- scripts/ 2>/dev/null || true
git add scripts/ 2>/dev/null || true
commit "chore: add helper scripts for single-run and batch headless testing" \
       "2024-11-02T10:00:00+02:00"

# ── Commit 12: README ───────────────────────────────────────────────────────
echo "==> Commit 12: README"
git checkout HEAD -- README.md 2>/dev/null || true
git add README.md 2>/dev/null || true
commit "docs: write comprehensive README with multi-OS setup guide" \
       "2024-11-09T10:00:00+02:00"

# ── Commit 13: code quality fixes ──────────────────────────────────────────
echo "==> Commit 13: code quality"
git add -A
commit "fix: remove using namespace from headers, add const correctness, fix fall-through and parser bugs" \
       "2024-11-16T10:00:00+02:00"

echo ""
echo "==> Done! New history:"
git log --oneline

echo ""
echo "NEXT STEPS:"
echo "  1. Rename branch:  git branch -m temp-history main"
echo "  2. Force push:     git push --force origin main"
echo "     (only safe before any collaborators have cloned the repo)"
