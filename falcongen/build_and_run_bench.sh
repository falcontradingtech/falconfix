#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$ROOT_DIR"

FIX_VERSION="FIX4.4"
XML_FILE="templates/FIX44.xml"
MESSAGE_FILTER=""
DO_GENERATE=0
EXTRA_BENCH_ARGS=()

usage() {
    cat <<'EOF'
Usage:
  ./build_and_run_bench.sh [options] [-- <benchmark args>]

Options:
  --fix-version <FIXx.y>   FIX version folder under output/bench (default: FIX4.4)
  --xml <path>             XML template to generate from (default: templates/FIX44.xml)
  --message-filter <Name>  Generate only one FIX message and its dependencies
  --generate               Run ./run.sh --tests before compiling
  --help                   Show this help

Examples:
  ./build_and_run_bench.sh --generate
  ./build_and_run_bench.sh -- --benchmark_filter=BM_FalconFix_Encode_MDIncremental
EOF
}

while [[ $# -gt 0 ]]; do
    case "$1" in
        --fix-version)
            FIX_VERSION="$2"
            shift 2
            ;;
        --xml)
            XML_FILE="$2"
            shift 2
            ;;
        --message-filter)
            MESSAGE_FILTER="$2"
            shift 2
            ;;
        --generate)
            DO_GENERATE=1
            shift
            ;;
        --help)
            usage
            exit 0
            ;;
        --)
            shift
            while [[ $# -gt 0 ]]; do
                EXTRA_BENCH_ARGS+=("$1")
                shift
            done
            ;;
        *)
            echo "[error] unknown argument: $1" >&2
            usage
            exit 2
            ;;
    esac
done

if [[ "$DO_GENERATE" -eq 1 ]]; then
    echo "[run] regenerating code + artifacts from ${XML_FILE}"
    RUN_ARGS=(--tests "$XML_FILE")
    if [[ -n "$MESSAGE_FILTER" ]]; then
        RUN_ARGS=(--tests --message-filter "$MESSAGE_FILTER" "$XML_FILE")
    fi
    ./run.sh "${RUN_ARGS[@]}"
fi

BENCH_FALCON_CPP="bench/${FIX_VERSION}/falconfix_bench.cpp"
BENCH_MAIN_CPP="bench/${FIX_VERSION}/bench_main.cpp"
if [[ ! -f "$BENCH_FALCON_CPP" || ! -f "$BENCH_MAIN_CPP" ]]; then
    echo "[error] missing benchmark source(s) under bench/${FIX_VERSION}" >&2
    exit 3
fi

if ! ls /usr/include/benchmark/benchmark.h >/dev/null 2>&1; then
    echo "[error] google benchmark headers not found in /usr/include/benchmark" >&2
    echo "[hint] install package: libbenchmark-dev" >&2
    exit 4
fi

mapfile -t CPP_FILES < <(find "output/cpp/${FIX_VERSION}" -name '*.cpp' | sort)
if [[ ${#CPP_FILES[@]} -eq 0 ]]; then
    echo "[error] no generated cpp files found under output/cpp/${FIX_VERSION}" >&2
    echo "[hint] run: ./run.sh --tests ${XML_FILE}" >&2
    exit 6
fi

BIN="output/bench/${FIX_VERSION}/roundtrip_bench_bin"
OBJ_DIR="output/bench/${FIX_VERSION}/obj"
mkdir -p "$(dirname "$BIN")" "$OBJ_DIR"

CXX="${CXX:-g++}"

BENCH_CFLAGS=()
BENCH_LIBS=()
if command -v pkg-config >/dev/null 2>&1 && pkg-config --exists benchmark; then
    read -r -a BENCH_CFLAGS <<< "$(pkg-config --cflags benchmark)"
    read -r -a BENCH_LIBS <<< "$(pkg-config --libs benchmark)"
else
    BENCH_LIBS=(-lbenchmark -lbenchmark_main)
fi

CXXFLAGS=(
    -std=c++20
    -O3
    -DNDEBUG
    -flto=20
    "${BENCH_CFLAGS[@]}"
    -I"output/include"
    -I"output/include/${FIX_VERSION}"
    -I"../src/include"
    -I"../src"
)
LDFLAGS=(
    "${BENCH_LIBS[@]}"
    -lpthread
)

obj_for_src() {
    local src="$1"
    local rel="${src#output/cpp/${FIX_VERSION}/}"
    rel="${rel%.cpp}.o"
    echo "${OBJ_DIR}/${rel}"
}

compile_obj_if_needed() {
    local src="$1"
    local obj="$2"
    local idx="$3"
    local total="$4"

    mkdir -p "$(dirname "$obj")"

    if [[ -f "$obj" && "$obj" -nt "$src" ]]; then
        echo "[build] [${idx}/${total}] up-to-date ${src}"
        return 0
    fi

    echo "[build] [${idx}/${total}] compiling ${src}"
    "$CXX" "${CXXFLAGS[@]}" -c "$src" -o "$obj"
}

BENCH_FALCON_OBJ="${OBJ_DIR}/falconfix_bench.o"
BENCH_MAIN_OBJ="${OBJ_DIR}/bench_main.o"

echo "[build] compiling benchmark translation unit ${BENCH_FALCON_CPP}"
"$CXX" "${CXXFLAGS[@]}" -c "$BENCH_FALCON_CPP" -o "$BENCH_FALCON_OBJ"

echo "[build] compiling benchmark translation unit ${BENCH_MAIN_CPP}"
"$CXX" "${CXXFLAGS[@]}" -c "$BENCH_MAIN_CPP" -o "$BENCH_MAIN_OBJ"

OBJ_FILES=()
TOTAL_CPP=${#CPP_FILES[@]}
INDEX=0
for src in "${CPP_FILES[@]}"; do
    INDEX=$((INDEX + 1))
    obj="$(obj_for_src "$src")"
    compile_obj_if_needed "$src" "$obj" "$INDEX" "$TOTAL_CPP"
    OBJ_FILES+=("$obj")
done

echo "[build] linking benchmark binary for ${FIX_VERSION}"
"$CXX" \
    "$BENCH_FALCON_OBJ" \
    "$BENCH_MAIN_OBJ" \
    "${OBJ_FILES[@]}" \
    "${LDFLAGS[@]}" \
    -o "$BIN"

echo "[bench] running ${BIN}"
"$BIN" --benchmark_min_time=0.5s "${EXTRA_BENCH_ARGS[@]}"
