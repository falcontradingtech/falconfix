#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$ROOT_DIR"

FIX_VERSION="FIX4.4"
XML_FILE="templates/FIX44.xml"
MESSAGE_FILTER=""
DO_GENERATE=0
AUTO_MINIMAL=1
MINIMAL_THRESHOLD=5
EXTRA_GTEST_ARGS=()

usage() {
    cat <<'EOF'
Usage:
  ./build_and_run_roundtrip_tests.sh [options] [-- <gtest args>]

Options:
  --fix-version <FIXx.y>   FIX version folder under output (default: FIX4.4)
  --xml <path>             XML template to generate from (default: templates/FIX44.xml)
  --message-filter <Name>  Generate only one FIX message and its dependencies
  --generate               Run ./run.sh --tests before compiling
    --no-auto-minimal        Always compile all generated .cpp files
  --help                   Show this help

Examples:
  ./build_and_run_roundtrip_tests.sh --generate --xml templates/FIX44.xml
  ./build_and_run_roundtrip_tests.sh --fix-version FIX4.4 -- --gtest_filter=RoundTrip_FIX4_4.Heartbeat
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
        --no-auto-minimal)
            AUTO_MINIMAL=0
            shift
            ;;
        --help)
            usage
            exit 0
            ;;
        --)
            shift
            while [[ $# -gt 0 ]]; do
                EXTRA_GTEST_ARGS+=("$1")
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
    echo "[run] regenerating code + roundtrip artifacts from ${XML_FILE}"
    RUN_ARGS=(--tests "$XML_FILE")
    if [[ -n "$MESSAGE_FILTER" ]]; then
        RUN_ARGS=(--tests --message-filter "$MESSAGE_FILTER" "$XML_FILE")
    fi
    ./run.sh "${RUN_ARGS[@]}"
fi

ROUNDTRIP_CPP="output/tests/${FIX_VERSION}/roundtrip.cpp"
if [[ ! -f "$ROUNDTRIP_CPP" ]]; then
    echo "[error] missing generated roundtrip file: output/tests/${FIX_VERSION}/roundtrip.cpp" >&2
    echo "[hint] run: ./run.sh --tests ${XML_FILE}" >&2
    exit 3
fi

if ! ls /usr/include/gtest/gtest.h >/dev/null 2>&1; then
    echo "[error] gtest headers not found in /usr/include/gtest" >&2
    exit 4
fi

extract_message_cpp_files() {
    local roundtrip_cpp="$1"
    local fix_version="$2"
    sed -n 's/.*runRoundTripFor<[^>]*::messages::\([A-Za-z0-9_][A-Za-z0-9_]*\)>.*/\1/p' "$roundtrip_cpp" \
        | sort -u \
        | sed "s#^#output/cpp/${fix_version}/messages/#; s#\$#.cpp#"
}

TEST_COUNT=$(grep -c '^TEST(' "$ROUNDTRIP_CPP" || true)
CPP_FILES=()
GTEST_FILTER=""
FILTER_MSG=""
ROUNDTRIP_BUILD_CPP="$ROUNDTRIP_CPP"

for arg in "${EXTRA_GTEST_ARGS[@]}"; do
    if [[ "$arg" == --gtest_filter=* ]]; then
        GTEST_FILTER="${arg#--gtest_filter=}"
        break
    fi
done

if [[ -n "$GTEST_FILTER" && "$GTEST_FILTER" != *"*"* && "$GTEST_FILTER" != *":"* && "$GTEST_FILTER" != *"-"* && "$GTEST_FILTER" == *"."* ]]; then
    FILTER_MSG="${GTEST_FILTER##*.}"
fi

if [[ -n "$FILTER_MSG" ]]; then
    candidate="output/cpp/${FIX_VERSION}/messages/${FILTER_MSG}.cpp"
    if [[ -f "$candidate" ]]; then
        FILTERED_CPP="output/tests/${FIX_VERSION}/roundtrip_filtered_${FILTER_MSG}.cpp"
        {
            awk 'BEGIN{keep=1} /^TEST\(/{keep=0} keep{print}' "$ROUNDTRIP_CPP"
            awk -v t="$FILTER_MSG" '
                $0 ~ "^TEST\\([^,]+,[[:space:]]*" t "\\)" { in_test=1 }
                in_test { print }
                in_test && /^}/ { exit }
            ' "$ROUNDTRIP_CPP"
        } > "$FILTERED_CPP"

        if [[ -s "$FILTERED_CPP" ]]; then
            ROUNDTRIP_BUILD_CPP="$FILTERED_CPP"
        fi

        echo "[build] gtest-filter mode: compiling message unit ${FILTER_MSG}"
        CPP_FILES+=("$candidate")
        while IFS= read -r f; do
            [[ -n "$f" ]] && [[ -f "$f" ]] && CPP_FILES+=("$f")
        done < <(find "output/cpp/${FIX_VERSION}/components" -name '*.cpp' | sort)
        [[ -f "output/cpp/${FIX_VERSION}/messages/Header.cpp" ]] && CPP_FILES+=("output/cpp/${FIX_VERSION}/messages/Header.cpp")
        [[ -f "output/cpp/${FIX_VERSION}/messages/Trailer.cpp" ]] && CPP_FILES+=("output/cpp/${FIX_VERSION}/messages/Trailer.cpp")
    fi
fi

if [[ ${#CPP_FILES[@]} -eq 0 && "$AUTO_MINIMAL" -eq 1 && "$TEST_COUNT" -gt 0 && "$TEST_COUNT" -le "$MINIMAL_THRESHOLD" ]]; then
    echo "[build] auto-minimal mode: ${TEST_COUNT} test(s), compiling only referenced message units"
    while IFS= read -r f; do
        [[ -n "$f" ]] && [[ -f "$f" ]] && CPP_FILES+=("$f")
    done < <(extract_message_cpp_files "$ROUNDTRIP_CPP" "$FIX_VERSION")

    while IFS= read -r f; do
        [[ -n "$f" ]] && [[ -f "$f" ]] && CPP_FILES+=("$f")
    done < <(find "output/cpp/${FIX_VERSION}/components" -name '*.cpp' | sort)

    # Header/Trailer are shared by message implementations.
    [[ -f "output/cpp/${FIX_VERSION}/messages/Header.cpp" ]] && CPP_FILES+=("output/cpp/${FIX_VERSION}/messages/Header.cpp")
    [[ -f "output/cpp/${FIX_VERSION}/messages/Trailer.cpp" ]] && CPP_FILES+=("output/cpp/${FIX_VERSION}/messages/Trailer.cpp")
fi

if [[ ${#CPP_FILES[@]} -eq 0 ]]; then
    mapfile -t CPP_FILES < <(find "output/cpp/${FIX_VERSION}" -name '*.cpp' | sort)
fi

if [[ ${#CPP_FILES[@]} -eq 0 ]]; then
    echo "[error] no generated cpp files found under output/cpp/${FIX_VERSION}" >&2
    exit 5
fi

BIN="output/tests/${FIX_VERSION}/roundtrip_tests_bin"
mkdir -p "$(dirname "$BIN")"
FILTERED_CPP=""

CXX="${CXX:-g++}"
CXXFLAGS=(-std=c++20 -O0 -g -fno-omit-frame-pointer -I"output/include" -I"output/include/${FIX_VERSION}" -I"../src/include" -I"../src")
LDFLAGS=(-lgtest_main -lgtest -lpthread)
if [[ "${ASAN:-0}" == "1" ]]; then
    CXXFLAGS+=(-fsanitize=address)
    LDFLAGS+=(-fsanitize=address)
fi
OBJ_DIR="output/tests/${FIX_VERSION}/obj"
mkdir -p "$OBJ_DIR"

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

ROUNDTRIP_OBJ="${OBJ_DIR}/roundtrip.o"
echo "[build] compiling test translation unit ${ROUNDTRIP_BUILD_CPP}"
"$CXX" "${CXXFLAGS[@]}" -c "$ROUNDTRIP_BUILD_CPP" -o "$ROUNDTRIP_OBJ"

OBJ_FILES=()
TOTAL_CPP=${#CPP_FILES[@]}
INDEX=0
for src in "${CPP_FILES[@]}"; do
    INDEX=$((INDEX + 1))
    obj="$(obj_for_src "$src")"
    compile_obj_if_needed "$src" "$obj" "$INDEX" "$TOTAL_CPP"
    OBJ_FILES+=("$obj")
done

echo "[build] compiling roundtrip binary for ${FIX_VERSION}"
"$CXX" \
    "$ROUNDTRIP_OBJ" \
    "${OBJ_FILES[@]}" \
    "${LDFLAGS[@]}" \
    -o "$BIN"

if [[ -n "$FILTERED_CPP" && -f "$FILTERED_CPP" ]]; then
    rm -f "$FILTERED_CPP"
fi

echo "[test] running ${BIN}"

# If user already requested a filter, keep default single-process behavior.
HAS_FILTER=0
for arg in "${EXTRA_GTEST_ARGS[@]}"; do
    if [[ "$arg" == --gtest_filter=* ]]; then
        HAS_FILTER=1
        break
    fi
done

if [[ "$HAS_FILTER" -eq 1 ]]; then
    "$BIN" --gtest_brief=1 "${EXTRA_GTEST_ARGS[@]}"
    exit $?
fi

echo "[test] isolated mode: running each test case in a separate process"
mapfile -t TEST_CASES < <(
    "$BIN" --gtest_list_tests \
    | awk '
        /^[A-Za-z0-9_].*\.$/ { suite=$1; next }
        /^[[:space:]]+[A-Za-z0-9_].*/ {
            test=$1
            gsub(/#.*/, "", test)
            gsub(/[[:space:]]+$/, "", test)
            if (suite != "" && test != "") print suite test
        }
    '
)

if [[ ${#TEST_CASES[@]} -eq 0 ]]; then
    echo "[error] no tests discovered in ${BIN}" >&2
    exit 6
fi

FAIL_COUNT=0
for tc in "${TEST_CASES[@]}"; do
    if ! "$BIN" --gtest_brief=1 --gtest_filter="$tc"; then
        echo "[fail] ${tc}"
        FAIL_COUNT=$((FAIL_COUNT + 1))
    fi
done

if [[ "$FAIL_COUNT" -gt 0 ]]; then
    echo "[test] ${FAIL_COUNT} failing test(s)"
    exit 1
fi

echo "[test] all tests passed"
