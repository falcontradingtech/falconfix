#!/usr/bin/env python3
﻿# SPDX-License-Identifier: MIT
# Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
create_project.py

Creates a new FalconFIX project from a FIX XML specification.

This script:
1. Creates a new project directory
2. Copies the existing falconfix project structure (excluding generated code)
3. Runs falcon-gen.py to generate engine and test sources
"""

import argparse
import sys
import shutil
import subprocess
import os
from pathlib import Path
from typing import Optional, List


class ProjectCreator:
    """Handles the creation and validation of a new FalconFIX project."""

    # Directories to exclude when copying the base project
    EXCLUDED_DIRS = {
        "src/engine/cpp",
        "src/engine/include",
        "tests/engine",
    }

    # Directories to exclude from the entire project copy
    EXCLUDED_TOP_LEVEL_DIRS = {
        ".vs",
        ".git",
        "build",
        "__pycache__"
    }

    def __init__(
        self,
        project_name: str,
        output_dir: str,
        xml_file: str,
        fix_version: Optional[str] = None,
        message_filter: Optional[str] = None,
        string_size_template: bool = False,
        samples_template: bool = False,
    ):
        """Initialize the project creator.

        Args:
            project_name: Name of the new project
            output_dir: Directory where the project will be created
            xml_file: Path to the FIX XML specification
            fix_version: Optional FIX version override
            message_filter: Optional comma-separated list of messages to generate
            string_size_template: Whether to enable string size templates
            samples_template: Whether to enable sample templates
        """
        self.project_name = project_name
        self.output_dir = Path(output_dir).resolve()
        self.xml_file = Path(xml_file).resolve()
        self.fix_version = fix_version
        self.message_filter = message_filter
        self.string_size_template = string_size_template
        self.samples_template = samples_template

        # Paths
        self.script_dir = Path(__file__).parent.resolve()
        self.source_project = self.script_dir / "."
        self.project_dir = self.output_dir / project_name

    def validate_inputs(self) -> None:
        """Validate all input parameters."""
        if not self.project_name:
            self._error("project-name is required")

        if not self.output_dir:
            self._error("output-dir is required")

        if not self.xml_file:
            self._error("--xml-file is required")

        if not self.xml_file.exists():
            self._error(f"XML file not found: {self.xml_file}")

    def create_directories(self) -> None:
        """Create the output and project directories."""
        try:
            self.output_dir.mkdir(parents=True, exist_ok=True)
            self._log(f"Created output directory: {self.output_dir}")
        except Exception as e:
            self._error(f"Failed to create output directory: {e}")

        try:
            self.project_dir.mkdir(parents=True, exist_ok=True)
            self._log(f"Created project directory: {self.project_dir}")
        except Exception as e:
            self._error(f"Failed to create project directory: {e}")

    def copy_base_project(self) -> None:
        """Copy the base falconfix project, excluding generated directories."""
        self._log(f"Copying base project from: {self.source_project}")

        try:
            for item in self.source_project.iterdir():
                # Skip hidden directories and excluded top-level directories
                if item.name.startswith(".") or item.name in self.EXCLUDED_TOP_LEVEL_DIRS:
                    self._debug(f"Skipping: {item.name}")
                    continue

                # Skip files at the root except essential ones
                if item.is_file():
                    # Copy essential files only
                    if item.name in {"CMakeLists.txt", "README.md", ".gitignore"}:
                        shutil.copy2(item, self.project_dir / item.name)
                    continue

                # Copy directories recursively
                if item.is_dir():
                    target = self.project_dir / item.name
                    self._copy_tree_excluding(item, target)

            self._log("Base project copied successfully")
        except Exception as e:
            self._error(f"Failed to copy base project: {e}")

    def _copy_tree_excluding(self, src: Path, dst: Path) -> None:
        """Recursively copy a directory tree, excluding certain paths.

        Args:
            src: Source directory
            dst: Destination directory
        """
        dst.mkdir(parents=True, exist_ok=True)

        for item in src.iterdir():
            rel_path = item.relative_to(self.source_project)
            # Normalize path to use forward slashes for consistent comparison
            rel_path_str = rel_path.as_posix()

            # Check if this path should be excluded
            is_excluded = (
                rel_path_str in self.EXCLUDED_DIRS or
                any(rel_path_str.startswith(f"{excl}/") for excl in self.EXCLUDED_DIRS)
            )

            if is_excluded:
                self._debug(f"Excluding: {rel_path_str}")
                continue

            target = dst / item.name

            if item.is_dir():
                self._copy_tree_excluding(item, target)
            else:
                shutil.copy2(item, target)

    def run_falcon_gen(self) -> None:
        """Run falcon-gen.py to generate engine and test sources."""
        self._log("Running falcon-gen.py...")

        # Build the command
        cmd = [sys.executable, str(self.script_dir / "falcongen" / "falcon-gen.py")]
        cmd.extend(["--xml-file", str(self.xml_file)])
        cmd.extend(["--output-dir", str(self.project_dir)])

        if self.fix_version:
            cmd.extend(["--fix-version", self.fix_version])

        if self.message_filter:
            cmd.extend(["--message-filter", self.message_filter])

        if self.string_size_template:
            cmd.append("--string-size-template")

        if self.samples_template:
            cmd.append("--samples-template")

        try:
            # Set UTF-8 encoding for Python output
            env = os.environ.copy()
            env["PYTHONIOENCODING"] = "utf-8"

            # Run subprocess without capturing output - let it print directly
            result = subprocess.run(
                cmd,
                cwd=self.script_dir / "falcongen",
                check=False,
                env=env,
            )

            # Check if core generated files exist (code generation succeeded even if post-processing failed)
            include_dir = self.project_dir / "include"
            cpp_dir = self.project_dir / "cpp"
            has_generated_code = include_dir.exists() or cpp_dir.exists()

            if result.returncode != 0 and not has_generated_code:
                # Only fail if code generation didn't happen at all
                self._error(f"falcon-gen.py failed with exit code {result.returncode}")
            elif result.returncode != 0:
                # Code was generated but post-processing failed - warn but continue
                self._log("falcon-gen.py completed with warnings (post-processing issue), but code was generated successfully")
            else:
                self._log("falcon-gen.py completed successfully")
        except Exception as e:
            self._error(f"Failed to run falcon-gen.py: {e}")

    def _is_fix_version_dir(self, dir_name: str) -> bool:
        """Check if a directory name matches a FIX version pattern (e.g., FIX4.4, FIX5.0).

        Args:
            dir_name: Name of the directory to check

        Returns:
            True if the directory matches the FIX version pattern
        """
        # Match FIX versions like FIX4.4, FIX5.0, FIX5.0SP1, etc.
        return dir_name.startswith("FIX") and len(dir_name) > 3

    def relocate_generated_directories(self) -> None:
        """Relocate generated FIX directories to the correct FalconFIX project layout.

        This method moves:
        - project_dir/include/FIX* -> project_dir/src/engine/include/FIX*
        - project_dir/cpp/FIX* -> project_dir/src/engine/cpp/FIX*
        - project_dir/tests/FIX* -> project_dir/tests/engine/FIX*
        """
        self._log("Relocating generated directories to project layout...")

        try:
            include_src = self.project_dir / "include"
            cpp_src = self.project_dir / "cpp"
            tests_src = self.project_dir / "tests"

            # Verify that at least the include or cpp directory exists
            if not include_src.exists() and not cpp_src.exists():
                self._error("No generated directories found (include/ and cpp/ not found)")

            # Create destination directories
            src_engine_include_dst = self.project_dir / "src" / "engine" / "include"
            src_engine_cpp_dst = self.project_dir / "src" / "engine" / "cpp"
            tests_engine_dst = self.project_dir / "tests" / "engine"

            src_engine_include_dst.mkdir(parents=True, exist_ok=True)
            src_engine_cpp_dst.mkdir(parents=True, exist_ok=True)
            tests_engine_dst.mkdir(parents=True, exist_ok=True)

            # Move include directories (e.g., include/FIX4.4 -> src/engine/include/FIX4.4)
            if include_src.exists():
                for fix_dir in include_src.iterdir():
                    if fix_dir.is_dir() and self._is_fix_version_dir(fix_dir.name):
                        destination = src_engine_include_dst / fix_dir.name
                        if destination.exists():
                            shutil.rmtree(destination)
                        shutil.move(str(fix_dir), str(destination))
                        self._log(f"Moved include/{fix_dir.name} -> src/engine/include/{fix_dir.name}")

                # Remove empty include directory
                try:
                    if not any(include_src.iterdir()):
                        include_src.rmdir()
                except OSError:
                    pass

            # Move cpp directories (e.g., cpp/FIX4.4 -> src/engine/cpp/FIX4.4)
            if cpp_src.exists():
                for fix_dir in cpp_src.iterdir():
                    if fix_dir.is_dir() and self._is_fix_version_dir(fix_dir.name):
                        destination = src_engine_cpp_dst / fix_dir.name
                        if destination.exists():
                            shutil.rmtree(destination)
                        shutil.move(str(fix_dir), str(destination))
                        self._log(f"Moved cpp/{fix_dir.name} -> src/engine/cpp/{fix_dir.name}")

                # Remove empty cpp directory
                try:
                    if not any(cpp_src.iterdir()):
                        cpp_src.rmdir()
                except OSError:
                    pass

            # Move test directories (e.g., tests/FIX4.4 -> tests/engine/FIX4.4)
            if tests_src.exists():
                for fix_dir in tests_src.iterdir():
                    if fix_dir.is_dir() and self._is_fix_version_dir(fix_dir.name):
                        destination = tests_engine_dst / fix_dir.name
                        if destination.exists():
                            shutil.rmtree(destination)
                        shutil.move(str(fix_dir), str(destination))
                        self._log(f"Moved tests/{fix_dir.name} -> tests/engine/{fix_dir.name}")

            self._log("Generated directories relocated successfully")
        except Exception as e:
            self._error(f"Failed to relocate generated directories: {e}")

    def validate_project_structure(self) -> None:
        """Validate that the final project structure matches the expected FalconFIX layout."""
        self._log("Validating project structure...")

        try:
            # Check for src/engine/include and src/engine/cpp with at least one FIX version directory
            src_engine_include = self.project_dir / "src" / "engine" / "include"
            src_engine_cpp = self.project_dir / "src" / "engine" / "cpp"
            tests_engine = self.project_dir / "tests" / "engine"

            has_include = src_engine_include.exists() and any(src_engine_include.iterdir())
            has_cpp = src_engine_cpp.exists() and any(src_engine_cpp.iterdir())
            has_tests = tests_engine.exists() and any(tests_engine.iterdir())

            if not has_include:
                self._error("Generated include directory not found at src/engine/include/")

            if not has_cpp:
                self._error("Generated cpp directory not found at src/engine/cpp/")

            if not has_tests:
                self._error("Generated test directory not found at tests/engine/")

            # Log what was found
            include_versions = [d.name for d in src_engine_include.iterdir() if d.is_dir()]
            cpp_versions = [d.name for d in src_engine_cpp.iterdir() if d.is_dir()]
            test_versions = [d.name for d in tests_engine.iterdir() if d.is_dir()]

            self._log(f"Found FIX versions in include: {', '.join(include_versions)}")
            self._log(f"Found FIX versions in cpp: {', '.join(cpp_versions)}")
            self._log(f"Found FIX versions in tests: {', '.join(test_versions)}")

            self._log("Project structure validation completed successfully")
        except Exception as e:
            self._error(f"Project structure validation failed: {e}")

    def copy_root_files(self) -> None:
        """Copy FalconFIX root files to the new project root."""

        root_files = [
            "vcpkg.json",
            ".clangd",
            ".clang-format",
            ".gitignore",
            "build.sh",
            "CMakeLists.txt",
            "CMakeSettings.json",
            "compile_commands.json",
            "create_project.py",
            "README-BUILD.md",
        ]

        self._log("Copying root files...")

        for file_name in root_files:
            src = self.source_project / file_name

            if not src.exists():
                self._debug(f"Skipping missing file: {file_name}")
                continue

            dst = self.project_dir / file_name
            shutil.copy2(src, dst)

        self._log("Root files copied successfully")


    def create_fix_cmakes(self) -> None:
        """Create FIX-version specific CMakeLists files."""

        cpp_root = (
            self.project_dir /
            "src" /
            "engine" /
            "cpp"
        )

        fix_dirs = [
            d.name
            for d in cpp_root.iterdir()
            if d.is_dir() and self._is_fix_version_dir(d.name)
        ]

        if len(fix_dirs) != 1:
            self._error(
                f"Expected exactly one FIX version directory under "
                f"{cpp_root}, found {len(fix_dirs)}"
            )

        fix_version = fix_dirs[0]

        #
        # src/engine/cpp/FIX_VERSION/CMakeLists.txt
        #
        engine_cmake_dir = cpp_root / fix_version

        engine_cmake_content = """add_lib_custom(falconfix_core STATIC spdlog::spdlog fmt::fmt)

# ------------------------------------------------------------------------------
# FalconFix generated protocols
# ------------------------------------------------------------------------------

falconfix_add_generated_protocols(falconfix_core src/engine/include/session/session_types.h)

if(FALCONFIX_BUILD_TESTS)
    target_compile_definitions(falconfix_core PUBLIC FALCONFIX_ENABLE_TEST_API=1)
    message(STATUS "Building with FALCONFIX_ENABLE_TEST_API enabled")
endif()
"""

        (engine_cmake_dir / "CMakeLists.txt").write_text(
            engine_cmake_content,
            encoding="utf-8"
        )

        #
        # tests/engine/FIX_VERSION/CMakeLists.txt
        #
        tests_engine_dir = (
            self.project_dir /
            "tests" /
            "engine" /
            fix_version
        )

        tests_engine_dir.mkdir(parents=True, exist_ok=True)

        fix_test_name = (
            fix_version
            .replace("FIX", "")
            .replace(".", "_")
        )

        tests_engine_content = (
            f'add_test_custom("engine_fix_{fix_test_name}_tests" '
            f'falconfix_config falconfix_session falconfix_core)\n'
        )

        (tests_engine_dir / "CMakeLists.txt").write_text(
            tests_engine_content,
            encoding="utf-8"
        )

        #
        # tests/CMakeLists.txt
        #
        tests_cmake = self.project_dir / "tests" / "CMakeLists.txt"

        tests_root_content = f"""enable_testing()

include_directories(${{CMAKE_CURRENT_SOURCE_DIR}})

add_subdirectory(app)
add_subdirectory(config)

add_subdirectory(engine/{fix_version})

add_subdirectory(multiversion)
add_subdirectory(session)
add_subdirectory(socket)
add_subdirectory(store)

add_test_custom("all_tests" falconfix_connection falconfix_logger socket falconfix_config falconfix_session falconfix_core)

add_custom_command(TARGET all_tests POST_BUILD
COMMAND ${{CMAKE_COMMAND}} -E copy_if_different
    ${{CMAKE_SOURCE_DIR}}/tests/socket/certs/server.crt
    $<TARGET_FILE_DIR:all_tests>

COMMAND ${{CMAKE_COMMAND}} -E copy_if_different
    ${{CMAKE_SOURCE_DIR}}/tests/socket/certs/server.key
    $<TARGET_FILE_DIR:all_tests>
)
"""

        tests_cmake.write_text(
            tests_root_content,
            encoding="utf-8"
        )

        self._log(f"Created FIX-specific CMake files for {fix_version}")

    def create(self) -> None:
        """Execute the complete project creation workflow."""

        if self.project_dir.exists():
            self._error(
                f"Unable to create project '{self.project_name}'. "
                f"Project already exists: {self.project_dir}"
            )


        self._log("Starting project creation...")
        self.validate_inputs()
        self.create_directories()
        self.copy_base_project()
        self.run_falcon_gen()
        self.relocate_generated_directories()
        self.copy_root_files()
        self.create_fix_cmakes()
        self.validate_project_structure()
        self._log(f"Project created successfully at: {self.project_dir}")

    @staticmethod
    def _log(message: str) -> None:
        """Log an informational message."""
        print(f"[info] {message}")

    @staticmethod
    def _debug(message: str) -> None:
        """Log a debug message."""
        # Uncomment to enable debug output
        # print(f"[debug] {message}")

    @staticmethod
    def _error(message: str) -> None:
        """Log an error message and exit."""
        print(f"[error] {message}", file=sys.stderr)
        sys.exit(1)


def parse_args() -> argparse.Namespace:
    """Parse command-line arguments."""
    parser = argparse.ArgumentParser(
        prog="create_project.py",
        description="Create a new FalconFIX project from a FIX XML specification",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  python create_project.py MyFix ./generated --xml-file spec/FIX44.xml
  python create_project.py MyFix ./generated --xml-file spec/FIX44.xml --fix-version FIX44
  python create_project.py MyFix ./generated --xml-file spec/FIX44.xml --message-filter "NewOrderSingle,ExecutionReport"
        """,
    )

    # Positional arguments
    parser.add_argument(
        "project_name",
        help="Name of the new project",
    )
    parser.add_argument(
        "output_dir",
        help="Directory where the new project will be created",
    )

    # Required arguments
    parser.add_argument(
        "--xml-file",
        required=True,
        help="FIX XML specification file",
    )

    # Optional arguments
    parser.add_argument(
        "--fix-version",
        help="FIX version (e.g., FIX44)",
    )
    parser.add_argument(
        "--message-filter",
        help="Comma-separated list of messages to generate",
    )
    parser.add_argument(
        "--string-size-template",
        action="store_true",
        help="Enable string size templates",
    )
    parser.add_argument(
        "--samples-template",
        action="store_true",
        help="Enable sample templates",
    )

    return parser.parse_args()


def main() -> int:
    """Main entry point."""
    try:
        args = parse_args()
        creator = ProjectCreator(
            project_name=args.project_name,
            output_dir=args.output_dir,
            xml_file=args.xml_file,
            fix_version=args.fix_version,
            message_filter=args.message_filter,
            string_size_template=args.string_size_template,
            samples_template=args.samples_template,
        )
        creator.create()
        return 0
    except Exception as e:
        print(f"[error] Unexpected error: {e}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    sys.exit(main())