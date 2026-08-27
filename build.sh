#!/usr/bin/env bash
set -euo pipefail

cat > CMakePresets.json <<'EOF'
{
  "version": 3,
  "configurePresets": [
    {
      "name": "linux-base",
      "hidden": true,
      "generator": "Ninja",
      "binaryDir": "${sourceDir}/build/${presetName}",
      "cacheVariables": {
        "CMAKE_EXPORT_COMPILE_COMMANDS": "ON"
      }
    },
    {
      "name": "linux-debug",
      "inherits": "linux-base",
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Debug"
      }
    },
    {
      "name": "linux-release",
      "inherits": "linux-base",
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Release"
      }
    }
  ],
  "buildPresets": [
    {
      "name": "linux-debug",
      "configurePreset": "linux-debug"
    },
    {
      "name": "linux-release",
      "configurePreset": "linux-release"
    }
  ],
  "testPresets": [
    {
      "name": "linux-debug",
      "configurePreset": "linux-debug",
      "output": {
        "outputOnFailure": true
      }
    },
    {
      "name": "linux-release",
      "configurePreset": "linux-release",
      "output": {
        "outputOnFailure": true
      }
    }
  ]
}
EOF

echo "CMakePresets.json generated."

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PRESET="linux-release"
JOBS="$(nproc)"
MODE="all"
TARGET=""
CLEAN="false"
BUILD_DIR=""

usage() {
    cat <<'EOF'
Usage: ./build.sh [options]

Options:
  --preset <name>         CMake preset to use (default: linux-release)
  --jobs <N>              Parallel jobs for build (default: nproc)
  --all                   Build default targets from preset (default)
  --bench                 Build benchmark targets only (socket_boost_bench + store_bench)
  --target <name>         Build a specific target name
    --clean                 Remove build/ before configure
  -h, --help              Show this help

Examples:
  ./build.sh
  ./build.sh --bench
  ./build.sh --clean
  ./build.sh --target app_tests
  ./build.sh --preset linux-debug --jobs 8
  ./build.sh --preset linux-release --jobs 8
EOF
}

while [[ $# -gt 0 ]]; do
    case "$1" in
        --preset)
            PRESET="${2:-}"
            shift 2
            ;;
        --jobs)
            JOBS="${2:-}"
            shift 2
            ;;
        --all)
            MODE="all"
            shift
            ;;
        --bench)
            MODE="bench"
            shift
            ;;
        --target)
            MODE="target"
            TARGET="${2:-}"
            shift 2
            ;;
        --clean)
            CLEAN="true"
            shift
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        *)
            echo "Unknown option: $1" >&2
            usage
            exit 1
            ;;
    esac
done

if [[ -z "${VCPKG_ROOT:-}" ]]; then
    echo "Error: VCPKG_ROOT is not set."
    echo "Example: export VCPKG_ROOT=$HOME/vcpkg"
    exit 1
fi

if ! [[ "$JOBS" =~ ^[0-9]+$ ]] || [[ "$JOBS" -lt 1 ]]; then
    echo "Error: --jobs must be a positive integer." >&2
    exit 1
fi

if [[ "$MODE" == "target" && -z "$TARGET" ]]; then
    echo "Error: --target requires a target name." >&2
    exit 1
fi

cd "$ROOT_DIR"

BUILD_DIR="$ROOT_DIR/build/$PRESET"

if [[ "$CLEAN" == "true" ]]; then
    echo "[clean] Removing $BUILD_DIR"
    rm -rf "$BUILD_DIR"
fi

mkdir -p "$ROOT_DIR/build"

echo "[1/2] Configuring preset: $PRESET"
cmake --preset "$PRESET"

echo "[2/2] Building"
case "$MODE" in
    all)
        cmake --build --preset "$PRESET" --parallel "$JOBS"
        ;;
    bench)
        cmake --build --preset "$PRESET" --parallel "$JOBS" --target socket_boost_bench store_bench
        ;;
    target)
        cmake --build --preset "$PRESET" --parallel "$JOBS" --target "$TARGET"
        ;;
esac

echo "Build finished successfully."
