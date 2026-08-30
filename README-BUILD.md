# Build Guide

This project supports two equivalent build flows on Linux:

1. `./build.sh`, which wraps the recommended CMake presets.
2. Direct CMake commands, if you prefer not to use the helper script.

## Prerequisites

- Linux environment
- `cmake`
- `ninja`
- a C++20 compiler (`g++` or `clang++`)
- `VCPKG_ROOT` set to your local vcpkg installation

Example:

```bash
export VCPKG_ROOT="$HOME/path/to/vcpkg"
```

## Recommended: Use the Helper Script

From the project root:

```bash
./build.sh
```

This configures and builds the `linux-release` preset.

## Build And Run All Tests

If you just want the standard full flow on Linux:

```bash
export VCPKG_ROOT="$HOME/path/to/vcpkg"
./build.sh
ctest --preset linux-release 
```

If you want test failures printed inline:

```bash
ctest --preset linux-release --output-on-failure
```

For a debug build:

```bash
export VCPKG_ROOT="$HOME/path/to/vcpkg"
./build.sh --preset linux-debug
ctest --preset linux-debug
```

You can also run an individual test binary directly after the build. Examples:

```bash
./build/linux-release/tests/session/session_tests
./build/linux-release/tests/multiversion/multiversion_tests
./build/linux-release/tests/store/store_tests
```

And if you want to filter a specific test case with GoogleTest:

```bash
./build/linux-release/tests/session/session_tests \
  --gtest_filter=FIXSessionTests.ClientServerLogonHandshake
```

Useful variations:

```bash
./build.sh --clean
./build.sh --preset linux-debug
./build.sh --preset linux-release --jobs 8
./build.sh --bench
./build.sh --target app_tests
```

## Build And Run Benchmarks

To build the benchmark targets only:

```bash
export VCPKG_ROOT="$HOME/path/to/vcpkg"
./build.sh --bench
```

This builds the main benchmark executables:

- `build/linux-release/benchmark/socket/socket_boost_bench`
- `build/linux-release/benchmark/store/store_bench`

There is also a utility benchmark target available in the regular full build:

- `build/linux-release/benchmark/utils/utils_bench`

If you want that one explicitly without doing a full build:

```bash
./build.sh --target utils_bench
```

To run them directly:

```bash
./build/linux-release/benchmark/socket/socket_boost_bench
./build/linux-release/benchmark/store/store_bench
./build/linux-release/benchmark/utils/utils_bench
```

To build everything and run all benchmark executables in sequence:

```bash
export VCPKG_ROOT="$HOME/path/to/vcpkg"
./build.sh
./build/linux-release/benchmark/socket/socket_boost_bench
./build/linux-release/benchmark/store/store_bench
./build/linux-release/benchmark/utils/utils_bench
```

If you want a single shell line:

```bash
export VCPKG_ROOT="$HOME/path/to/vcpkg"
./build.sh && \
./build/linux-release/benchmark/socket/socket_boost_bench && \
./build/linux-release/benchmark/store/store_bench && \
./build/linux-release/benchmark/utils/utils_bench
```

## Direct CMake Usage

If you want to run CMake manually, use the presets that are versioned with the repository:

```bash
cmake --preset linux-release
cmake --build --preset linux-release --parallel
```

For benchmark targets only:

```bash
cmake --preset linux-release
cmake --build --preset linux-release --parallel --target socket_boost_bench store_bench
```

For a debug build:

```bash
cmake --preset linux-debug
cmake --build --preset linux-debug --parallel
ctest --preset linux-debug
```

## Generating `compile_commands.json`

`compile_commands.json` is generated automatically by the presets and by `build.sh`.

Release preset:

- `build/linux-release/compile_commands.json`

Debug preset:

- `build/linux-debug/compile_commands.json`

If your editor expects this file in the project root, create a symlink:

```bash
ln -sf build/linux-release/compile_commands.json compile_commands.json
```

## Common Mistake

If you run:

```bash
mkdir -p build
cd build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```

it fails because CMake was not told where the source tree is. In that case, CMake assumes the current directory is both the build directory and the source directory, then complains that there is no `CMakeLists.txt` there.

If you want to configure from inside `build/`, the minimum correct command is:

```bash
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
```

That said, the safer and more reproducible options for this repository are:

```bash
cmake --preset linux-release
```

or:

```bash
./build.sh
```

## Notes

- `build.sh` exits on the first error with `set -euo pipefail`.
- If `VCPKG_ROOT` is missing, configuration stops immediately.
- If `vcpkg.json` changes and vcpkg reports that the manifest has no baseline, refresh it with `"$VCPKG_ROOT/vcpkg" x-update-baseline --add-initial-baseline` before configuring again.
- Benchmark comparison code expects the release preset output under `build/linux-release`, so keep that preset available.
- If vcpkg reports that the baseline commit is missing locally, update your local clone with `git -C "$VCPKG_ROOT" fetch --all --tags --prune`.

## About Ignored Files and Directories

Build artifacts, temporary files, editor settings, local dependencies, and generated `compile_commands.json` files are intentionally ignored. Each developer should generate these locally after cloning the repository so the repo stays portable and free from machine-specific paths.

## Code Coverage

Code coverage reports are generated in CI/CD pipelines and uploaded to [Codecov](https://app.codecov.io/gh/falcontradingtech/falconfix/).

### Local Coverage Report (Linux)

To generate a local coverage report on Linux:

```bash
export VCPKG_ROOT="$HOME/path/to/vcpkg"
export CXXFLAGS="--coverage"
export LDFLAGS="--coverage"

./build.sh
ctest --preset linux-release

# Generate coverage report
lcov --directory build/linux-release --capture --output-file coverage.info
lcov --remove coverage.info '*/vcpkg/*' '*/build/*' '*/usr/include/*' --output-file coverage.info
lcov --list coverage.info
```

### Coverage on Windows

On Windows, use [OpenCppCoverage](https://opencppcoverage.codeplex.com/) with the build.ps1 script. The CI/CD workflow handles coverage automatically.

### Viewing Coverage Reports

Coverage reports are automatically uploaded to [Codecov](https://app.codecov.io/gh/falcontradingtech/falconfix/) on every push to `main` and pull request.

