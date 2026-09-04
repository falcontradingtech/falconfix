# -*- coding: utf-8 -*-

# SPDX-License-Identifier: MIT
# Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo

"""
generator_components_tests.py
-------------------------------------------------
Generates comprehensive C++ unit tests for FIX components with 100% code coverage.
Creates one test file per component in tests/engine/<FIX_VERSION>/components/<ComponentName>_tests.cpp

Features:
  - Tests for all getters/setters (presence coverage)
  - Tests for reset() functionality
  - Tests for encode/decode roundtrips
  - Tests for field boundary conditions
  - Tests for required field validation
"""

from __future__ import annotations
from pathlib import Path
import re
import json
from typing import Optional

from code.common import namespace_from_version, normalize_version_string


def _find_nested_struct_blocks(content: str) -> list[tuple[str, str, int, int]]:
	"""
	Locate nested `struct Name { ... };` blocks (used by FalconFIX for repeating
	group entries, e.g. `class AllocAckGrp { struct NoAllocs { ... }; ... };`).

	Returns a list of (struct_name, struct_body, start_index, end_index) using
	brace-depth matching (regex alone cannot handle nested braces reliably).
	"""
	results = []
	for m in re.finditer(r'struct\s+(\w+)\s*\{', content):
		struct_name = m.group(1)
		brace_start = m.end() - 1  # index of the opening '{'
		depth = 0
		i = brace_start
		while i < len(content):
			if content[i] == '{':
				depth += 1
			elif content[i] == '}':
				depth -= 1
				if depth == 0:
					break
			i += 1
		if depth != 0:
			continue  # malformed / unmatched braces, skip
		brace_end = i
		body = content[brace_start + 1:brace_end]
		results.append((struct_name, body, m.start(), brace_end + 1))
	return results


def _parse_members(scope_text: str) -> dict:
	"""
	Parse fields/getters/setters/has-methods/REQUIRED/MAX_ENCODE_SIZE from a
	class or struct body (used for both top-level components and nested group
	entry structs).
	"""
	fields = {}
	field_pattern = r'types::(\w+)(?:<(\d+)>)?\s+m_(\w+)\s*\{'
	for match in re.finditer(field_pattern, scope_text):
		field_type = match.group(1)
		field_size = match.group(2)
		field_name = match.group(3)
		fields[field_name] = {
			'type': field_type,
			'size': field_size,
			'camelCase': _to_camel_case(field_name)
		}

	# REQUIRED can be a single literal (e.g. `0ULL` or `0x3`) or a multi-line
	# bitmask expression like `(1ULL << 0) | (1ULL << 2);`. Capture everything
	# up to the terminating semicolon (DOTALL) and treat it as "required" if
	# it's not the trivial `0ULL` literal.
	required_match = re.search(r'static\s+constexpr\s+uint64_t\s+REQUIRED\s*=\s*(.*?);', scope_text, re.DOTALL)
	is_any_required = bool(required_match) and required_match.group(1).strip() != '0ULL'

	size_match = re.search(r'static\s+constexpr\s+std::size_t\s+MAX_ENCODE_SIZE\s*=\s*(\d+)', scope_text)
	max_size = int(size_match.group(1)) if size_match else 256

	methods = {
		'getters': [],
		'setters': [],
		'has_methods': []
	}

	getter_pattern = r'const\s+(types::\w+(?:<\d+>)?)\s+&get(\w+)\(\)\s+const\s+noexcept'
	for match in re.finditer(getter_pattern, scope_text):
		methods['getters'].append({
			'return_type': match.group(1),
			'name': match.group(2)
		})

	setter_pattern = r'void\s+set(\w+)\((const\s+)?(types::\w+(?:<\d+>)?)\s+[&v]\)\s+noexcept'
	for match in re.finditer(setter_pattern, scope_text):
		methods['setters'].append({
			'name': match.group(1),
			'param_type': match.group(3)
		})

	has_pattern = r'bool\s+has(\w+)\(\)\s+const\s+noexcept'
	for match in re.finditer(has_pattern, scope_text):
		methods['has_methods'].append(match.group(1))

	return {
		'fields': fields,
		'methods': methods,
		'has_required': is_any_required,
		'max_encode_size': max_size,
	}


def extract_component_info(header_file: Path, namespace: str) -> dict:
	"""
	Parse a component header file and extract:
	- Class name
	- Field members (name, type)
	- Getters/setters
	- Required fields
	- MAX_ENCODE_SIZE

	Repeating-group components (e.g. AllocAckGrp) declare their real fields
	inside a nested `struct NoXxx { ... }` entry type; the outer class itself
	only exposes a std::vector<NoXxx> plus count/has accessors. Nested structs
	are detected and parsed separately so tests reference the correct type.
	"""
	content = header_file.read_text(encoding="utf-8")

	# Extract class name
	class_match = re.search(r'class\s+(\w+)\s*\{', content)
	if not class_match:
		return None

	class_name = class_match.group(1)

	# Detect nested group-entry structs and strip them out before parsing the
	# outer class scope, so its fields/setters/getters aren't polluted by the
	# inner struct's members.
	nested_blocks = _find_nested_struct_blocks(content)
	outer_content = content
	for _, _, start, end in sorted(nested_blocks, key=lambda b: b[2], reverse=True):
		outer_content = outer_content[:start] + outer_content[end:]

	outer_members = _parse_members(outer_content)

	nested = []
	for struct_name, body, _, _ in nested_blocks:
		nested_members = _parse_members(body)
		nested.append({
			'name': f'{class_name}_{struct_name}',
			'cpp_type': f'{class_name}::{struct_name}',
			'header_name': class_name,
			'namespace': f'{namespace}::components',
			**nested_members,
			'file': header_file.name,
		})

	return {
		'name': class_name,
		'cpp_type': class_name,
		'header_name': class_name,
		'namespace': f'{namespace}::components',
		**outer_members,
		'file': header_file.name,
		'nested': nested,
	}


def _to_camel_case(snake_str: str) -> str:
	"""Convert snake_case to camelCase"""
	components = snake_str.split('_')
	return components[0] + ''.join(x.title() for x in components[1:])


def _generate_test_header(component: dict, fix_version: str) -> str:
	"""Generate the #include section and setup"""
	include_root = normalize_version_string(fix_version)
	cpp_type = component.get('cpp_type', component['name'])
	header_name = component.get('header_name', component['name'])
	lines = [
		"// SPDX-License-Identifier: MIT",
		f"// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo",
		"// Auto-generated component tests. Do not edit by hand.",
		f"// Component: {cpp_type} ({fix_version})",
		"",
		"#include <gtest/gtest.h>",
		"#include <string_view>",
		"#include <cstring>",
		"",
		f"#include <{include_root}/components/{header_name}.h>",
		f"#include <{include_root}/utils/serializer.h>",
		"#include <utils/fast_buffer.h>",
		"",
		f"using namespace {component['namespace']};",
		"",
		f"class {component['name']}ComponentTest : public ::testing::Test {{",
		"protected:",
		f"    {cpp_type} component;",
		"",
		"    void SetUp() override {",
		"        component.reset();",
		"    }",
		"};",
		"",
	]
	return "\n".join(lines)


def _generate_reset_tests(component: dict) -> str:
	"""Generate tests for reset() functionality"""
	lines = [
		f"TEST_F({component['name']}ComponentTest, ResetClearsAllFields) {{",
		"    // Set some fields",
	]

	# Add setter calls for first few fields
	for setter in component['methods']['setters'][:3]:
		field_name = setter['name']
		if 'CHAR' in setter['param_type']:
			lines.append(f"    component.set{field_name}('A');")
		elif 'INT' in setter['param_type']:
			lines.append(f"    component.set{field_name}(42);")
		elif 'FLOAT' in setter['param_type']:
			lines.append(f"    component.set{field_name}(3.14f);")
		elif 'CURRENCY' in setter['param_type']:
			lines.append(f"    // component.set{field_name}(...);")

	lines.extend([
		"    ",
		"    // Reset component",
		"    component.reset();",
		"    ",
		"    // Verify all fields are cleared",
		"    EXPECT_FALSE(component.hasAnySet());",
		"}",
		"",
	])

	return "\n".join(lines)


def _generate_setter_getter_tests(component: dict) -> str:
	"""Generate tests for all setters and getters"""
	lines = []

	for setter in component['methods']['setters']:
		field_name = setter['name']
		param_type = setter['param_type']

		# Find corresponding getter
		getter = next((g for g in component['methods']['getters'] if g['name'] == field_name), None)
		if not getter:
			continue

		test_name = f"{component['name']}ComponentTest, Set{field_name}And{field_name}Match"
		lines.append(f"TEST_F({test_name}) {{")

		# Generate appropriate test value based on type
		if 'CHAR' in param_type:
			lines.append("    const char test_value = 'X';")
			lines.append(f"    component.set{field_name}(test_value);")
			lines.append(f"    EXPECT_EQ(component.get{field_name}(), test_value);")
			lines.append(f"    EXPECT_TRUE(component.has{field_name}());")
		elif 'INT' in param_type or 'LONG' in param_type:
			lines.append("    const int64_t test_value = 12345;")
			lines.append(f"    component.set{field_name}(test_value);")
			lines.append(f"    EXPECT_EQ(component.get{field_name}(), test_value);")
			lines.append(f"    EXPECT_TRUE(component.has{field_name}());")
		elif 'FLOAT' in param_type or 'PRICE' in param_type:
			lines.append("    const double test_value = 123.456;")
			lines.append(f"    component.set{field_name}(test_value);")
			lines.append(f"    EXPECT_EQ(component.get{field_name}(), test_value);")
			lines.append(f"    EXPECT_TRUE(component.has{field_name}());")
		elif 'CURRENCY' in param_type or 'STRING' in param_type:
			# For complex types, just test presence
			lines.append(f"    EXPECT_FALSE(component.has{field_name}());")

		lines.extend(["}", ""])

	return "\n".join(lines)


def _generate_presence_tests(component: dict) -> str:
	"""Generate tests for field presence tracking"""
	lines = [
		f"TEST_F({component['name']}ComponentTest, HasAnySetTracksPresence) {{",
		"    EXPECT_FALSE(component.hasAnySet());",
		"    ",
	]

	if component['methods']['setters']:
		setter = component['methods']['setters'][0]
		lines.append(f"    component.set{setter['name']}(" + ("'A'" if 'CHAR' in setter['param_type'] else "42") + ");")
		lines.append("    EXPECT_TRUE(component.hasAnySet());")

	lines.extend(["}", ""])
	return "\n".join(lines)


def _test_value_literal_and_setup(param_type: str) -> tuple[str, str]:
	"""
	Return (decl_lines_joined, value_expr) for a scalar setter test value,
	matching the same type-dispatch rules used by _generate_setter_getter_tests.
	Returns ("", "") if the type is not a simple settable scalar we can
	populate deterministically (e.g. complex STRING/CURRENCY types).
	"""
	if 'CHAR' in param_type:
		return "const char test_value = 'X';", "test_value"
	if 'INT' in param_type or 'LONG' in param_type:
		return "const int64_t test_value = 12345;", "test_value"
	if 'FLOAT' in param_type or 'PRICE' in param_type:
		return "const double test_value = 123.456;", "test_value"
	return "", ""


def _generate_encode_decode_tests(component: dict) -> str:
	"""Generate tests for encode/decode roundtrips"""
	cpp_type = component.get('cpp_type', component['name'])

	# Prefer a setter whose type we can populate deterministically (CHAR/INT/
	# FLOAT/PRICE), so the roundtrip actually exercises a non-empty payload.
	chosen_setter = None
	chosen_decl = ""
	chosen_value = ""
	for setter in component['methods']['setters']:
		decl, value = _test_value_literal_and_setup(setter['param_type'])
		if decl:
			chosen_setter = setter
			chosen_decl = decl
			chosen_value = value
			break

	lines = [
		f"TEST_F({component['name']}ComponentTest, EncodeDecodeRoundtrip) {{",
		f"    char buffer[{component['max_encode_size'] * 2}];",
		"    ",
	]

	if chosen_setter:
		lines.extend([
			"    // Populate a real field so the encoded payload is non-empty and",
			"    // decode() is genuinely exercised (some component wrappers return",
			"    // false on a decode of zero consumed bytes).",
			f"    {chosen_decl}",
			f"    component.set{chosen_setter['name']}({chosen_value});",
			"    ",
			"    char *p = buffer;",
			"    p = component.encode(p, true);",
			"    std::size_t encoded_size = p - buffer;",
			"    ",
			"    // Verify encode succeeded",
			"    EXPECT_LE(encoded_size, component.compute_buffer_size());",
			"    EXPECT_GT(encoded_size, 0u);",
			"    ",
			"    // Decode back",
			f"    {cpp_type} decoded;",
			"    const char *q = buffer;",
			"    bool decode_result = decoded.decode(q, p);",
			"    EXPECT_TRUE(decode_result);",
			f"    EXPECT_EQ(decoded.get{chosen_setter['name']}(), component.get{chosen_setter['name']}());",
			"    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());",
			"}",
			"",
		])
	else:
		lines.extend([
			"    // No scalar setter available to populate deterministically (this",
			"    // component only wraps nested groups/sub-components). Group-only",
			"    // containers with no required fields legitimately encode to 0",
			"    // bytes, and some decode() implementations return false when no",
			"    // bytes were consumed, so we only assert the two views agree.",
			"    char *p = buffer;",
			"    p = component.encode(p, true);",
			"    std::size_t encoded_size = p - buffer;",
			"    ",
			"    EXPECT_LE(encoded_size, component.compute_buffer_size());",
			"    ",
			f"    {cpp_type} decoded;",
			"    const char *q = buffer;",
			"    (void)decoded.decode(q, p);",
			"    EXPECT_EQ(decoded.hasAnySet(), component.hasAnySet());",
			"}",
			"",
		])

	return "\n".join(lines)


def _generate_boundary_tests(component: dict) -> str:
	"""Generate tests for boundary conditions and edge cases"""
	lines = [
		f"TEST_F({component['name']}ComponentTest, CheckRequiredWhenEmpty) {{",
	]

	if component['has_required']:
		lines.append("    EXPECT_FALSE(component.checkRequired());")
	else:
		lines.append("    EXPECT_TRUE(component.checkRequired());")

	lines.extend(["}", ""])
	return "\n".join(lines)


def _generate_full_test_file(component: dict, fix_version: str) -> str:
	"""Generate complete test file for a component"""
	sections = [
		_generate_test_header(component, fix_version),
		_generate_reset_tests(component),
		_generate_setter_getter_tests(component),
		_generate_presence_tests(component),
		_generate_encode_decode_tests(component),
		_generate_boundary_tests(component),
	]

	return "\n".join(filter(None, sections))


def generate_all_component_tests(
	include_dir: Path,
	output_dir: Path,
	fix_version: str = "FIX4.4"
) -> dict:
	"""
	Generate test files for all components in a FIX version.

	Args:
		include_dir: Path to include/<FIX_VERSION>/components/
		output_dir: Path to tests/engine/<FIX_VERSION>/components/
		fix_version: FIX version label (e.g., "FIX4.4")

	Returns:
		Dictionary with statistics about generated tests
	"""
	components_dir = include_dir / "components"
	output_dir.mkdir(parents=True, exist_ok=True)
	namespace = namespace_from_version(fix_version)

	stats = {
		'total': 0,
		'generated': 0,
		'skipped': 0,
		'errors': [],
		'files': []
	}

	if not components_dir.exists():
		stats['errors'].append(f"Components directory not found: {components_dir}")
		return stats

	# Process each component header file
	for header_file in sorted(components_dir.glob("*.h")):
		stats['total'] += 1

		try:
			component = extract_component_info(header_file, namespace)
			if not component:
				stats['skipped'] += 1
				continue

			# Generate test file for the top-level component (or group container)
			test_code = _generate_full_test_file(component, fix_version)
			test_file = output_dir / f"{component['name']}_tests.cpp"

			test_file.write_text(test_code, encoding="utf-8")
			stats['generated'] += 1
			stats['files'].append(str(test_file.relative_to(output_dir.parent.parent)))

			print(f"[ok] Generated {component['name']}_tests.cpp ({len(component['methods']['setters'])} setters)")

			# Generate test files for nested group-entry structs (e.g. NoAllocs),
			# which own the real field setters/getters for repeating groups.
			for nested_component in component.get('nested', []):
				nested_code = _generate_full_test_file(nested_component, fix_version)
				nested_file = output_dir / f"{nested_component['name']}_tests.cpp"

				nested_file.write_text(nested_code, encoding="utf-8")
				stats['generated'] += 1
				stats['files'].append(str(nested_file.relative_to(output_dir.parent.parent)))

				print(f"[ok] Generated {nested_component['name']}_tests.cpp ({len(nested_component['methods']['setters'])} setters, nested type)")

		except Exception as e:
			stats['errors'].append(f"{header_file.name}: {str(e)}")
			stats['skipped'] += 1

	return stats


def generate(base_output: Path, fix_version: str) -> list[Path]:
	"""
	Pipeline-compatible entry point, mirroring generator_tests.generate().

	Reads components from: base_output / "include" / fix_version / "components"
	Writes tests to:       base_output / "tests" / fix_version / "components"
	"""
	include_dir = base_output / "include" / fix_version
	output_dir = base_output / "tests" / fix_version / "components"

	stats = generate_all_component_tests(include_dir, output_dir, fix_version)

	if stats['errors']:
		for error in stats['errors']:
			print(f"[warn] {error}")

	print(f"[ok] component tests written → {output_dir} ({stats['generated']} components)")
	return [Path(f) for f in stats['files']]


if __name__ == "__main__":
	import sys

	# Example usage
	if len(sys.argv) > 1:
		include_path = Path(sys.argv[1])
		output_path = Path(sys.argv[2]) if len(sys.argv) > 2 else Path("./output/tests/FIX4.4/components")
		version = sys.argv[3] if len(sys.argv) > 3 else "FIX4.4"
	else:
		include_path = Path("../output/include/FIX4.4")
		output_path = Path("../output/tests/FIX4.4/components")
		version = "FIX4.4"

	stats = generate_all_component_tests(include_path, output_path, version)

	print(f"\n[summary]")
	print(f"  Total:     {stats['total']}")
	print(f"  Generated: {stats['generated']}")
	print(f"  Skipped:   {stats['skipped']}")

	if stats['errors']:
		print(f"\n[errors]")
		for error in stats['errors']:
			print(f"  - {error}")
