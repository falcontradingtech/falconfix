---
applyTo: "src/engine/**,tests/engine/**"
---
# GENERATED CODE
Output of `falcongen/`. The final fix must live in the generator; a hand edit here is never the deliverable.

`src/engine/**`: do not edit. Change `falcongen/code/*.py` / `falcongen/config/*.yaml`, regenerate. Layout (messages/components/core/engine/utils) and the `FIXMessageRef` usage pattern: skill `engine-layout`.

`tests/engine/**` (FIX4.2/4.3/4.4) workflow:
1. Identify which `falcongen` script emits the file (e.g. `falcongen/gen_serializer_tests.py`, `falcongen/code/generator_*_tests.py`).
2. Prototype the change in the `_tests.cpp`; build + run that suite to validate.
3. Port the validated change to the generator.
4. Regenerate: `python falcongen/gen_serializer_tests.py` (or the owning script).
5. Rebuild, re-run `ctest -R engine`; confirm regenerated output matches the prototype.
