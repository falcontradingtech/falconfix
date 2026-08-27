# -*- coding: utf-8 -*-

# SPDX-License-Identifier: MIT
# Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo

"""
paths.py - Path Management for Code Generation

Functions to manage directory structure.

NOTE: This module uses a global _INCLUDE_ROOT that is defined by
ensure_base_include_structure(). This is a legacy pattern que permite
compatibility with old code. New code should pass paths
as parameters instead of using globals.
"""

from pathlib import Path
from typing import Optional

from .strings import normalize_version_string, namespace_from_version

# ===== GLOBAL STATE (LEGACY - COMPATIBILITY LAYER) =====
# This global is defined por ensure_base_include_structure()
# New code should avoid this
_INCLUDE_ROOT: Optional[Path] = None


def ensure_base_include_structure(base_path: Path, spec_version: str) -> Path:
    """
    Creates standard directory structure and sets o global _INCLUDE_ROOT.

    Args:
        base_path: Root directory (ex: ./output)
        spec_version: FIX version (ex: "FIX4.4")

    Returns:
        Path of main directory created

    Side effects:
        Set global _INCLUDE_ROOT to support get_*_path() funcs

    Creates:
        base_path/include/FIX4.4/
            components/
            core/
            engine/
            messages/
            samples/
            utils/
    """
    global _INCLUDE_ROOT

    base = Path(base_path)
    version = normalize_version_string(spec_version)
    major = version[3]
    minor = version[5]

    # Use "include" subdirectory to match old behavior
    versioned_dir = base / "include" / f"FIX{major}.{minor}"

    # Store in global for compatibility
    _INCLUDE_ROOT = versioned_dir

    subdirs = [
        "components",
        "core",
        "engine",
        "messages",
        "proto",
        "samples",
        "session",
        "utils",
    ]

    versioned_dir.mkdir(parents=True, exist_ok=True)

    for subdir in subdirs:
        (versioned_dir / subdir).mkdir(exist_ok=True)

    return versioned_dir


def get_base_path() -> Path:
    """
    Returns the base path defined por ensure_base_include_structure().

    LEGACY: Uses global _INCLUDE_ROOT. New code should pass paths
    directly as parameters.
    """
    global _INCLUDE_ROOT
    if _INCLUDE_ROOT is None:
        raise RuntimeError("ensure_base_include_structure() was not called")
    return _INCLUDE_ROOT


def get_core_path() -> Path:
    """Returns path to core module (uses global _INCLUDE_ROOT)."""
    return get_base_path() / "core"


def get_components_path() -> Path:
    """Returns path to components module."""
    return get_base_path() / "components"

def get_messages_path() -> Path:
    """Returns path to messages module."""
    return get_base_path() / "messages"


def get_engine_path() -> Path:
    """Returns path to engine module."""
    return get_base_path() / "engine"


def get_serialize_path() -> Path:
    """Returns path to serialize module."""
    return get_base_path() / "serialize"


def get_parse_path() -> Path:
    """Returns path to parse module."""
    return get_base_path() / "parse"


def get_reset_path() -> Path:
    """Returns path to reset module."""
    return get_base_path() / "reset"


def get_samples_path() -> Path:
    """Returns path to samples module."""
    return get_base_path() / "samples"


def get_session_path() -> Path:
    """Returns path to session module."""
    return get_base_path() / "session"


def get_proto_path() -> Path:
    """Returns path to proto module."""
    return get_base_path() / "proto"


def get_utils_path() -> Path:
    """Returns path to utils module."""
    return get_base_path() / "utils"


def namespace_from_spec(spec_version) -> str:
    """
    Converte de FIX version para namespace C++.

    Accepts:
        - String: "FIX4.4" -> "fix44"
        - Dict: spec["version"] used
        - FixSpec object: spec.version used

    Example:
        "FIX4.4" -> "fix44"
        "FIX5.0" -> "fix50"
        {"version": "4.4"} -> "fix44"
        FixSpec(version="4.4") -> "fix44"
    """
    # If is a dict, extract key 'version'
    if isinstance(spec_version, dict):
        version = spec_version.get('version')
    # If is an object with version attribute, extract version
    elif hasattr(spec_version, 'version'):
        version = spec_version.version
    else:
        version = spec_version

    return namespace_from_version(version)
