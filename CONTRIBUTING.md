# Contributing to FalconFIX

Thank you for contributing to FalconFIX! This document covers the main requirements and workflow for submitting changes.

## Getting Started

1. **Fork the repository** on GitHub.
2. **Clone your fork:**

   ```bash
   git clone https://github.com/YOUR_USERNAME/falconfix.git
   cd falconfix
   git remote add upstream https://github.com/falcontradingtech/falconfix.git
   ```

## Development Setup

### Prerequisites

* C++20 compiler (GCC 11+, MSVC 2022, Clang 14+)
* CMake 3.16+
* Ninja
* Git
* vcpkg

### Linux

```bash
sudo apt-get update
sudo apt-get install -y cmake ninja-build pkg-config build-essential

git clone https://github.com/Microsoft/vcpkg.git
./vcpkg/bootstrap-vcpkg.sh

export VCPKG_ROOT=$(pwd)/vcpkg
./build.sh
```

### Windows

```powershell
choco install cmake ninja -y

git clone https://github.com/Microsoft/vcpkg.git
.\vcpkg\bootstrap-vcpkg.bat

$env:VCPKG_ROOT = "$(Get-Location)\vcpkg"
.\build.ps1 -Preset windows-release
```

macOS support is planned.

## Making Changes

### Branches

Use descriptive names:

* `feature/description`
* `fix/description`
* `docs/description`
* `refactor/description`
* `perf/description`

Example:

```bash
git checkout -b fix/session-timeout-handling
```

### Commits

* Use imperative mood (`Add feature`, not `Added feature`)
* Keep the first line under 50 characters
* Reference issues when applicable (`Fix #123`)
* Add context in the commit body when needed

## Testing

All contributions must:

* Pass all existing tests on **Linux and Windows**
* Include appropriate tests for new functionality
* Maintain or improve coverage
* Achieve at least **80% coverage for new code** and **90%+ for critical paths**

Run tests with:

```bash
# Linux
./build.sh
cd build/linux-release
ctest --output-on-failure

# Windows
.\build.ps1 -Preset windows-release
cd build\windows-release
ctest --output-on-failure -C Release
```

## Code Quality

* **Codacy:** No quality regressions or new critical issues.
* **Duplication:** Avoid duplication in manually written code. Generated FIX4.x code is excluded from duplication checks.
* **Formatting:** Follow `.clang-format` and run `clang-format` before committing.
* **Documentation:** Document public APIs and non-obvious implementation decisions.
* **Dependencies:** Avoid unnecessary dependencies.

### C++ Guidelines

* Use C++20 features appropriately
* Prefer const correctness
* Avoid exceptions in performance-critical paths
* Document thread-safety assumptions
* Use `FFStatus` for error propagation
* Profile performance-sensitive changes

## Pull Requests

Before submitting:

```bash
git fetch upstream
git rebase upstream/main
```

Then run the full test suite and verify coverage and Codacy results.

Create the PR from your fork with:

* A clear title and description
* Related issues (`Closes #123`)
* Explanation of **what and why**
* Any breaking changes
* Testing performed

### PR Checklist

* [ ] Builds without warnings on Linux and Windows
* [ ] All tests pass
* [ ] Coverage maintained or improved
* [ ] clang-format applied
* [ ] Documentation updated
* [ ] No unnecessary dependencies
* [ ] Performance impact evaluated where applicable

Maintainers will review the PR and may request changes before merging.

## Building

### Linux

```bash
./build.sh --preset linux-release
./build.sh --preset linux-debug --clean
./build.sh --target my_target
./build.sh --bench
```

### Windows

```powershell
.\build.ps1 -Preset windows-release
.\build.ps1 -Preset windows-debug -Clean
.\build.ps1 -Target my_target
.\build.ps1 -Mode bench
```

Build output:

* Linux: `build/linux-release/` or `build/linux-debug/`
* Windows: `build/windows-release/` or `build/windows-debug/`

## Performance

FalconFIX is a **high-performance** library. Performance regressions must be justified and documented. Include benchmark results for performance-critical changes.

## Questions

* See `README-BUILD.md` for build details
* Open an issue for questions
* Contact the maintainers through GitHub

Thank you for helping make FalconFIX better! 🚀