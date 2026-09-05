# Domain: CMake Build System

**Quick Reference for CMake and build configuration in FalconFIX.**

---

## 🛡️ GOLDEN RULES

**DON'T MODIFY** root `CMakeLists.txt` unless:

1. You have approval in GitHub issue
2. You explain clearly WHY
3. You've tested locally first
4. You provide rollback plan

**DO USE** existing CMake helpers in `cmake/`

---

## 📁 Build Directory (Critical!)

```powershell
# ✅ CORRECT - Use persistent directory
cmake --preset windows-debug
cmake --build build/windows-debug --config Debug
# Change code...
cmake --build build/windows-debug --config Debug  # FAST! (incremental)

# ❌ WRONG - Creating new directories
cmake -B build-v1 -DCMAKE_BUILD_TYPE=Debug
cmake --build build-v1
# Later...
cmake -B build-v2 -DCMAKE_BUILD_TYPE=Debug  # Recompiles EVERYTHING!
cmake --build build-v2