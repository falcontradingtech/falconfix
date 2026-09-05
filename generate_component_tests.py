#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# SPDX-License-Identifier: MIT
# Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo

"""
generate_component_tests.py

Standalone script to generate comprehensive component tests for FIX versions.
Places generated tests in: tests/engine/<FIX_VERSION>/components/

Usage:
	python generate_component_tests.py --input-dir src/engine/include/FIX4.4 --output-dir tests/engine/FIX4.4
"""

import argparse
import sys
from pathlib import Path

# Add falcongen to path
sys.path.insert(0, str(Path(__file__).parent / 'falcongen'))

from code.generator_components_tests import generate_all_component_tests


def main():
	parser = argparse.ArgumentParser(
		description="Generate comprehensive component unit tests with 100% coverage"
	)
	parser.add_argument(
		"--input-dir",
		type=Path,
		required=True,
		help="Path to FIX version include directory (e.g., src/engine/include/FIX4.4)"
	)
	parser.add_argument(
		"--output-dir",
		type=Path,
		required=True,
		help="Path to output tests directory (e.g., tests/engine/FIX4.4)"
	)
	parser.add_argument(
		"--fix-version",
		type=str,
		default=None,
		help="FIX version label (auto-detected from input-dir if not provided)"
	)

	args = parser.parse_args()

	# Verify input directory exists
	if not args.input_dir.exists():
		print(f"Error: Input directory not found: {args.input_dir}", file=sys.stderr)
		return 1

	# Auto-detect FIX version from input directory path if not provided
	if args.fix_version is None:
		# Extract version from path like "src/engine/include/FIX4.3"
		for part in args.input_dir.parts:
			if part.startswith("FIX") and "." in part:
				args.fix_version = part
				break
		if args.fix_version is None:
			print(f"Error: Could not auto-detect FIX version from path: {args.input_dir}", file=sys.stderr)
			print("Please provide --fix-version explicitly (e.g., FIX4.3)", file=sys.stderr)
			return 1

	# Create output directory
	output_components_dir = args.output_dir / "components"
	output_components_dir.mkdir(parents=True, exist_ok=True)

	print(f"[info] Input:  {args.input_dir}")
	print(f"[info] Output: {output_components_dir}")
	print(f"[info] Generating component tests for {args.fix_version}...\n")

	# Generate tests
	stats = generate_all_component_tests(
		args.input_dir,
		output_components_dir,
		args.fix_version
	)

	# Print summary
	print(f"\n{'='*60}")
	print(f"[summary] Component Test Generation")
	print(f"{'='*60}")
	print(f"  Total components:    {stats['total']}")
	print(f"  Tests generated:     {stats['generated']}")
	print(f"  Skipped:             {stats['skipped']}")
	print(f"  Output directory:    {output_components_dir}")

	if stats['errors']:
		print(f"\n[errors] {len(stats['errors'])} issues encountered:")
		for error in stats['errors'][:5]:
			print(f"  - {error}")
		if len(stats['errors']) > 5:
			print(f"  ... and {len(stats['errors']) - 5} more")

	return 0 if stats['generated'] > 0 else 1


if __name__ == "__main__":
	sys.exit(main())
