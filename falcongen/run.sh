#!/usr/bin/env bash
set -euo pipefail

#find . -type d -name "__pycache__" -exec rm -rf {} +

rm -rf __pycache__
rm -rf code/__pycache__

XML_FILE=""
RUN_TESTS=0
MESSAGE_FILTER=""
OUTPUT_DIR="output"
FULL_GENERATION=0

write_protocol_cmake() {
	local protocol_cpp_dir="$1"

	mkdir -p "$protocol_cpp_dir"
	cat > "${protocol_cpp_dir}/CMakeLists.txt" <<'EOF'
add_lib_custom(falconfix_core STATIC spdlog::spdlog fmt::fmt)

# ------------------------------------------------------------------------------
# FalconFix generated protocols
# ------------------------------------------------------------------------------

falconfix_add_generated_protocols(falconfix_core src/engine/include/session/session_types.h)


if(FALCONFIX_BUILD_TESTS)
    target_compile_definitions(falconfix_core PUBLIC FALCONFIX_ENABLE_TEST_API=1)
    message(STATUS "Building with FALCONFIX_ENABLE_TEST_API enabled")
endif()
EOF
}

while [[ $# -gt 0 ]]; do
	case "$1" in
		--tests|--test)
			RUN_TESTS=1
			shift
			;;
		--message-filter)
			MESSAGE_FILTER="$2"
			shift 2
			;;
		--output-dir)
			OUTPUT_DIR="$2"
			shift 2
			;;
		--all)
			FULL_GENERATION=1
			shift
			;;
		*)
			XML_FILE="$1"
			shift
			;;
	esac
done

if [[ -z "$XML_FILE" ]]; then
	shopt -s nullglob
	xml_candidates=(templates/*.xml)
	shopt -u nullglob
	if [[ ${#xml_candidates[@]} -eq 0 ]]; then
		echo "[error] no XML templates found under templates/" >&2
		exit 1
	fi
	XML_FILE="${xml_candidates[0]}"
fi

if [[ ! -f "$XML_FILE" ]]; then
	echo "[error] XML file not found: $XML_FILE" >&2
	exit 1
fi

if [[ "$FULL_GENERATION" -eq 0 && -z "$MESSAGE_FILTER" ]]; then
	echo "[error] blocked: full generation is disabled by default" >&2
	echo "        use --message-filter <MessageName> for roadmap validation" >&2
	echo "        or pass --all to explicitly generate everything" >&2
	exit 1
fi

FIX_VERSION="$(python3 - "$XML_FILE" <<'PY'
import sys
import xml.etree.ElementTree as ET

xml_path = sys.argv[1]
root = ET.parse(xml_path).getroot()
major = root.attrib.get("major")
minor = root.attrib.get("minor")
if not major or not minor:
	raise SystemExit("Unable to derive FIX version from XML root attributes 'major'/'minor'")
print(f"FIX{major}.{minor}")
PY
)"

echo "[run] generating code for ${FIX_VERSION} from ${XML_FILE}"
echo "[run] output-dir: ${OUTPUT_DIR}"
if [[ "$FULL_GENERATION" -eq 1 ]]; then
	echo "[run] mode: full (--all)"
else
	echo "[run] mode: filtered (--message-filter=${MESSAGE_FILTER})"
fi

mkdir -p "$OUTPUT_DIR"
rm -rf \
	"${OUTPUT_DIR}/include/${FIX_VERSION}" \
	"${OUTPUT_DIR}/cpp/${FIX_VERSION}" \
	"${OUTPUT_DIR}/tests/${FIX_VERSION}"

GEN_ARGS=(--xml-file="$XML_FILE" --output-dir="$OUTPUT_DIR")
if [[ -n "$MESSAGE_FILTER" ]]; then
	GEN_ARGS+=(--message-filter="$MESSAGE_FILTER")
fi
python3 falcon-gen.py "${GEN_ARGS[@]}"
write_protocol_cmake "${OUTPUT_DIR}/cpp/${FIX_VERSION}"

if [[ "$RUN_TESTS" -eq 1 ]]; then
	echo "[tests] validating generated roundtrip artifacts for ${FIX_VERSION}"
	ROUNDTRIP_FILE="${OUTPUT_DIR}/tests/${FIX_VERSION}/roundtrip.cpp"
	if [[ ! -f "$ROUNDTRIP_FILE" ]]; then
		echo "[error] Missing generated tests file: $ROUNDTRIP_FILE" >&2
		exit 2
	fi

	TEST_COUNT="$(grep -c '^TEST(' "$ROUNDTRIP_FILE")"
	if [[ "$TEST_COUNT" -le 0 ]]; then
		echo "[error] No TEST() cases found in: $ROUNDTRIP_FILE" >&2
		exit 2
	fi

	if ! grep -q "RoundTrip_" "$ROUNDTRIP_FILE"; then
		echo "[error] RoundTrip suite marker not found in: $ROUNDTRIP_FILE" >&2
		exit 2
	fi

	echo "[tests] ok: ${TEST_COUNT} roundtrip tests generated in ${ROUNDTRIP_FILE}"
fi

echo ""
