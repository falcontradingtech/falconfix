# SPDX-License-Identifier: MIT
# Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#!/usr/bin/env python3

from __future__ import annotations

import argparse
import json
import shutil
import subprocess
import sys
import xml.etree.ElementTree as ET
from datetime import datetime, timezone
from pathlib import Path


ROOT_DIR = Path(__file__).resolve().parent
DEFAULT_XML_TEMPLATE = ROOT_DIR / "falcongen" / "templates" / "FIX44.xml"
DEFAULT_OUTPUT_DIR = ROOT_DIR / "falconfix-lib"
BUILD_PRESET = "linux-release"
BUILD_DIR = ROOT_DIR / "build" / BUILD_PRESET

INTERNAL_LIBRARIES = [
    ("falconfix_core", BUILD_DIR / "src" / "engine" / "cpp" / "FIX4.4" / "libfalconfix_core.a"),
    ("falconfix_generated_protocols", BUILD_DIR / "src" / "engine" / "cpp" / "FIX4.4" / "libfalconfix_generated_protocols.a"),
    ("falconfix_config", BUILD_DIR / "src" / "cpp" / "config" / "libfalconfix_config.a"),
    ("falconfix_session", BUILD_DIR / "src" / "cpp" / "session" / "libfalconfix_session.a"),
    ("falconfix_logger", BUILD_DIR / "src" / "cpp" / "logger" / "libfalconfix_logger.a"),
    ("falconfix_connection", BUILD_DIR / "src" / "cpp" / "connection" / "libfalconfix_connection.a"),
    ("socket", BUILD_DIR / "src" / "cpp" / "socket" / "libsocket.a"),
]

FIX_VERSION_TO_MACRO = {
    "FIX4.0": "FALCONFIX_ENABLE_FIX40",
    "FIX4.1": "FALCONFIX_ENABLE_FIX41",
    "FIX4.2": "FALCONFIX_ENABLE_FIX42",
    "FIX4.3": "FALCONFIX_ENABLE_FIX43",
    "FIX4.4": "FALCONFIX_ENABLE_FIX44",
    "FIX5.0": "FALCONFIX_ENABLE_FIX50",
    "FIX5.0SP1": "FALCONFIX_ENABLE_FIX50SP1",
    "FIX5.0SP2": "FALCONFIX_ENABLE_FIX50SP2",
    "FIXT1.1": "FALCONFIX_ENABLE_FIXT11",
}


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Generate FalconFix protocol code, build release artifacts, and stage a deploy folder.",
    )
    parser.add_argument(
        "xml_template",
        nargs="?",
        default=str(DEFAULT_XML_TEMPLATE),
        help=f"Path to the FIX XML template. Default: {DEFAULT_XML_TEMPLATE}",
    )
    parser.add_argument(
        "output_dir",
        nargs="?",
        default=str(DEFAULT_OUTPUT_DIR),
        help=f"Deploy output directory. Default: {DEFAULT_OUTPUT_DIR}",
    )
    return parser.parse_args()


def run_command(command: list[str], cwd: Path) -> None:
    printable = " ".join(str(part) for part in command)
    print(f"[run] ({cwd}) {printable}", flush=True)
    subprocess.run(command, cwd=cwd, check=True)


def reset_directory(path: Path) -> None:
    if path.exists():
        shutil.rmtree(path)
    path.mkdir(parents=True, exist_ok=True)


def copy_tree_contents(source_dir: Path, destination_dir: Path) -> None:
    if not source_dir.exists():
        raise FileNotFoundError(f"Missing source directory: {source_dir}")

    destination_dir.mkdir(parents=True, exist_ok=True)
    for child in source_dir.iterdir():
        destination = destination_dir / child.name
        if child.is_dir():
            shutil.copytree(child, destination, dirs_exist_ok=True)
        else:
            shutil.copy2(child, destination)


def copy_selected_engine_headers(fix_version: str, destination_dir: Path) -> None:
    source_dir = ROOT_DIR / "src" / "engine" / "include" / fix_version
    if not source_dir.exists():
        raise FileNotFoundError(f"Missing generated headers for {fix_version}: {source_dir}")
    shutil.copytree(source_dir, destination_dir / fix_version, dirs_exist_ok=True)


def collect_enabled_macros(fix_version: str) -> list[str]:
    macro = FIX_VERSION_TO_MACRO.get(fix_version)
    return [macro] if macro else []


def write_deploy_cmake(output_dir: Path, enabled_macros: list[str]) -> None:
    cmake_dir = output_dir / "cmake"
    cmake_dir.mkdir(parents=True, exist_ok=True)
    macro_block = "\n".join(f'        "{macro}"' for macro in enabled_macros)

    content = f"""include_guard(GLOBAL)

set(_falconfix_root "${{CMAKE_CURRENT_LIST_DIR}}/..")

find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost REQUIRED)
find_package(Threads REQUIRED)

function(_falconfix_import_static target_name file_name)
    if(TARGET ${{target_name}})
        return()
    endif()

    add_library(${{target_name}} STATIC IMPORTED GLOBAL)
    set_target_properties(${{target_name}} PROPERTIES
        IMPORTED_LOCATION "${{_falconfix_root}}/libs/${{file_name}}"
    )
endfunction()

_falconfix_import_static(FalconFix::socket libsocket.a)
_falconfix_import_static(FalconFix::falconfix_connection libfalconfix_connection.a)
_falconfix_import_static(FalconFix::falconfix_logger libfalconfix_logger.a)
_falconfix_import_static(FalconFix::falconfix_session libfalconfix_session.a)
_falconfix_import_static(FalconFix::falconfix_config libfalconfix_config.a)
_falconfix_import_static(FalconFix::falconfix_generated_protocols libfalconfix_generated_protocols.a)
_falconfix_import_static(FalconFix::falconfix_core libfalconfix_core.a)

if(NOT TARGET FalconFix::falconfix)
    add_library(FalconFix::falconfix INTERFACE IMPORTED GLOBAL)
    set_target_properties(FalconFix::falconfix PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${{_falconfix_root}}/include"
        INTERFACE_COMPILE_DEFINITIONS
{macro_block if macro_block else '        ""'}
        INTERFACE_LINK_LIBRARIES "FalconFix::falconfix_core;FalconFix::falconfix_generated_protocols;FalconFix::falconfix_config;FalconFix::falconfix_session;FalconFix::falconfix_logger;FalconFix::falconfix_connection;FalconFix::socket;fmt::fmt;spdlog::spdlog;OpenSSL::SSL;OpenSSL::Crypto;Boost::boost;Threads::Threads"
    )
endif()
"""

    (cmake_dir / "FalconFixDeploy.cmake").write_text(content, encoding="utf-8")


def parse_fix_version(xml_template: Path) -> str:
    root = ET.parse(xml_template).getroot()
    major = root.attrib.get("major")
    minor = root.attrib.get("minor")
    servicepack = root.attrib.get("servicepack")
    type_attr = root.attrib.get("type")

    if not major or not minor:
        raise ValueError(f"Could not derive FIX version from XML: {xml_template}")

    if type_attr == "FIXT":
        return f"FIXT{major}.{minor}"

    if servicepack and servicepack != "0":
        return f"FIX{major}.{minor}SP{servicepack}"

    return f"FIX{major}.{minor}"


def write_manifest(output_dir: Path, xml_template: Path, fix_version: str, enabled_macros: list[str]) -> None:
    manifest = {
        "generated_at_utc": datetime.now(timezone.utc).isoformat(),
        "xml_template": str(xml_template),
        "fix_version": fix_version,
        "build_preset": BUILD_PRESET,
        "libraries": [file_path.name for _, file_path in INTERNAL_LIBRARIES],
        "compile_definitions": enabled_macros,
        "include_roots": [
            "include",
        ],
        "cmake_helper": "cmake/FalconFixDeploy.cmake",
    }
    (output_dir / "manifest.json").write_text(
        json.dumps(manifest, indent=2),
        encoding="utf-8",
    )


def main() -> int:
    args = parse_args()
    xml_template = Path(args.xml_template).expanduser().resolve()
    output_dir = Path(args.output_dir).expanduser().resolve()

    if not xml_template.is_file():
        print(f"[error] XML template not found: {xml_template}", file=sys.stderr)
        return 1

    if output_dir == ROOT_DIR:
        print("[error] output directory cannot be the project root.", file=sys.stderr)
        return 1

    fix_version = parse_fix_version(xml_template)
    include_dir = output_dir / "include"
    libs_dir = output_dir / "libs"

    print(f"[info] xml template : {xml_template}", flush=True)
    print(f"[info] fix version  : {fix_version}", flush=True)
    print(f"[info] deploy dir   : {output_dir}", flush=True)

    if fix_version != "FIX4.4":
        print(
            "[warn] The current CMake setup still anchors falconfix_core in src/engine/cpp/FIX4.4. "
            "Other templates still feed falconfix_generated_protocols, but making that CMake target version-aware "
            "would leave this flow more robust.",
            flush=True,
        )

    run_command(
        [
            "bash",
            "run.sh",
            "--all",
            "--output-dir",
            "../src/engine",
            str(xml_template),
        ],
        cwd=ROOT_DIR / "falcongen",
    )

    run_command(
        ["bash", "build.sh", "--preset", BUILD_PRESET],
        cwd=ROOT_DIR,
    )

    reset_directory(output_dir)
    include_dir.mkdir(parents=True, exist_ok=True)
    libs_dir.mkdir(parents=True, exist_ok=True)

    copy_tree_contents(ROOT_DIR / "src" / "include", include_dir)
    copy_selected_engine_headers(fix_version, include_dir)

    for _, library_path in INTERNAL_LIBRARIES:
        if not library_path.is_file():
            raise FileNotFoundError(f"Expected build artifact not found: {library_path}")
        shutil.copy2(library_path, libs_dir / library_path.name)

    enabled_macros = collect_enabled_macros(fix_version)
    write_deploy_cmake(output_dir, enabled_macros)
    write_manifest(output_dir, xml_template, fix_version, enabled_macros)

    print("[ok] deploy package created successfully", flush=True)
    print(f"[ok] include dir: {include_dir}", flush=True)
    print(f"[ok] libs dir   : {libs_dir}", flush=True)
    print(f"[ok] cmake file : {output_dir / 'cmake' / 'FalconFixDeploy.cmake'}", flush=True)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
