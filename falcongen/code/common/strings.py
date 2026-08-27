# -*- coding: utf-8 -*-

# SPDX-License-Identifier: MIT
# Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo

"""
strings.py - String Manipulation and Naming Utilities

Functions for name normalization, FIX version parsing, string sizes, etc.
"""

from pathlib import Path
from typing import Optional, Dict
import yaml
import xml.etree.ElementTree as ET

# ===== GLOBAL STATE FOR STRING SIZES =====
# This global is compatibility with old code. 
# New code should pass string_size_map as parameter.
_STRING_SIZE_MAP: Dict[str, int] = {"default": 32}


def load_string_size_map(path: Path) -> None:
    """
    Loads config/string_sizes.yaml and populates global _STRING_SIZE_MAP.
    
    Expected file:
        default: 32
        categories:
            STRING: 32
            CURRENCY: 3
        fields:
            ClOrdID: 64
            Symbol: 8
    """
    global _STRING_SIZE_MAP
    
    path = Path(path)
    
    if not path.exists():
        _STRING_SIZE_MAP = {"default": 32}
        return
    
    data = yaml.safe_load(path.read_text(encoding="utf-8")) or {}
    
    default_sz = int(data.get("default", 32))
    merged = {"default": default_sz}
    
    # categories + fields
    for grp in ("categories", "fields"):
        block = data.get(grp, {}) or {}
        for k, v in block.items():
            if isinstance(v, int):
                merged[normalize_name(k)] = v
    
    _STRING_SIZE_MAP = merged


def get_string_size(field_name: str, fix_type: str) -> int:
    """
    Returns string size for a field.
    
    Uses global _STRING_SIZE_MAP that is populated by load_string_size_map().
    """
    fname = normalize_name(field_name)
    if fname in _STRING_SIZE_MAP:
        return _STRING_SIZE_MAP[fname]
    
    t = fix_type.upper()
    if t in _STRING_SIZE_MAP:
        return _STRING_SIZE_MAP[t]
    
    return _STRING_SIZE_MAP.get("default", 32)


def normalize_name(name: str) -> str:
    """Converts FIX names to C\+\+ identifiers."""
    if not isinstance(name, str):
        return str(name)
    return name.replace("-", "_").replace(" ", "_")


def to_upper(s: str) -> str:
    """Safe uppercase."""
    return s.upper() if isinstance(s, str) else str(s)


def to_lower(s: str) -> str:
    """Safe lowercase."""
    return s.lower() if isinstance(s, str) else str(s)


# ===== FIX VERSION HELPERS =====

def parse_fix_version_from_xml(xml_path, override: Optional[str] = None) -> str:
    """Extracts FIX version from XML file."""
    if override:
        return normalize_version_string(override)
    
    xml_file = Path(xml_path)
    tree = ET.parse(xml_file)
    root = tree.getroot()

    major = root.attrib.get("major")
    minor = root.attrib.get("minor")

    if not major or not minor:
        raise ValueError(f"XML {xml_file} missing FIX major/minor attributes")

    return f"FIX{major}.{minor}"


def normalize_version_string(version: str) -> str:
    """Normalizes FIX version string to standard format."""
    # If is an object with version attribute, extract from it
    if hasattr(version, 'version'):
        version = version.version
    
    if not isinstance(version, str):
        raise ValueError(f"Invalid FIX version: {version}")

    v = version.strip().upper()

    # FIX44 → FIX4.4
    if v.startswith("FIX") and len(v) >= 5 and "." not in v:
        return f"FIX{v[3]}.{v[4]}"

    # 4.4 → FIX4.4
    if "." in v:
        parts = v.split(".")
        if len(parts) == 2 and parts[0].isdigit() and parts[1].isdigit():
            return f"FIX{parts[0]}.{parts[1]}"

    # 44 → FIX4.4
    if v.isdigit() and len(v) == 2:
        return f"FIX{v[0]}.{v[1]}"

    # FIX4.4 → OK
    if v.startswith("FIX") and "." in v:
        return v

    raise ValueError(f"Unrecognized FIX version format: {version}")


def namespace_from_version(version: str) -> str:
    """Converts FIX versions to canonical namespace."""
    v = normalize_version_string(version)
    lower = v.lower()

    if lower.startswith("fixt"):
        suffix = "".join(ch for ch in lower[4:] if ch.isalnum())
        return f"fixt{suffix}"

    if lower.startswith("fix"):
        suffix = "".join(ch for ch in lower[3:] if ch.isalnum())
        return f"fix{suffix}"

    suffix = "".join(ch for ch in lower if ch.isalnum())
    return f"fix{suffix}"


def short_version(version: str) -> str:
    """Returns version without .FIX prefix"."""
    v = normalize_version_string(version)
    return v.replace("FIX", "")
