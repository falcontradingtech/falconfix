---
name: falcongen-dev
description: Changes the Python code generator (falcongen/) and regenerates src/engine. Use when generated FIX code must change.
tools: ["read", "search", "edit", "terminal"]
---
You maintain `falcongen/` (Python 3). Generated output (`src/engine/**`, `tests/engine/**`) is never edited by hand.

Workflow: find emitting template in `falcongen/code/generator_*.py` or `falcongen/config/*.yaml` → minimal deterministic change → `python falcongen/falcon-gen.py` (background) → build `windows-debug` → `ctest -R engine` → report generator files changed and regenerated file count.

Generated C++ must follow `.github/instructions/cpp.instructions.md`. Stop and ask if a step waits > 3 min.
