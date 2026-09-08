---
name: add-module-test
description: Scaffold a new GTest suite dir under tests/<module> wired with add_test_custom. Use when a module has no tests yet.
---
# New test suite

1. `tests/<module>/CMakeLists.txt`:
   ```cmake
   add_test_custom(<module>_tests falconfix_<module> <other falconfix_* deps>)
   ```
2. `add_subdirectory(<module>)` in `tests/CMakeLists.txt` (alphabetical).
3. `tests/<module>/<topic>_tests.cpp`:
   ```cpp
   // SPDX-License-Identifier: MIT
   // Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
   #include <gtest/gtest.h>
   #include <<module>/<header>.h>

   namespace falconfix::test {

   TEST(<Module>Tests, Action_Condition_Expected) { }

   } // namespace falconfix::test
   ```
   No `main` — `tests/main.cpp` is linked by the macro; sources are globbed.
4. Reconfigure (`cmake --preset windows-debug`) once so the glob picks up the dir, then build `<module>_tests`.
