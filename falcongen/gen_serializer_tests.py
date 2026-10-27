# -*- coding: utf-8 -*-
# SPDX-License-Identifier: MIT
# Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo

"""
Standalone helper to (re)generate, directly into the real repo tree:
  - tests/engine/<FIX_VERSION>/utils/serializer_tests.cpp
  - tests/engine/<FIX_VERSION>/utils/parser_tests.cpp
  - tests/engine/<FIX_VERSION>/utils/datetime_tests.cpp
  - tests/engine/<FIX_VERSION>/CMakeLists.txt            (add_subdirectory only)
  - tests/engine/<FIX_VERSION>/components/CMakeLists.txt (add_test_custom)
  - tests/engine/<FIX_VERSION>/utils/CMakeLists.txt      (add_test_custom)

for FIX4.2, FIX4.3 and FIX4.4.
"""

from pathlib import Path

from code.generator_components_tests import (
	_generate_serializer_tests_content,
	_generate_parser_tests_content,
	_generate_datetime_tests_content,
	ensure_engine_version_cmakelists,
)

REPO_ROOT = Path(__file__).resolve().parent.parent
VERSIONS = ["FIX4.2", "FIX4.3", "FIX4.4"]

for version in VERSIONS:
	engine_dir = REPO_ROOT / "tests" / "engine"

	out_dir = engine_dir / version / "utils"
	out_dir.mkdir(parents=True, exist_ok=True)

	serializer_file = out_dir / "serializer_tests.cpp"
	serializer_file.write_text(_generate_serializer_tests_content(version), encoding="utf-8")
	print(f"[ok] {serializer_file}")

	parser_file = out_dir / "parser_tests.cpp"
	parser_file.write_text(_generate_parser_tests_content(version), encoding="utf-8")
	print(f"[ok] {parser_file}")

	datetime_file = out_dir / "datetime_tests.cpp"
	datetime_file.write_text(_generate_datetime_tests_content(version), encoding="utf-8")
	print(f"[ok] {datetime_file}")

	ensure_engine_version_cmakelists(engine_dir / version, version)