# -*- coding: utf-8 -*-

# SPDX-License-Identifier: MIT
# Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo

"""
types.py - FIX Protocol Type Classification and Constants

Centralizes FIX type categorization and their properties.
"""

from typing import Set, Dict

# ===== CHARACTER TYPES =====
PRIMITIVE_CHAR: Set[str] = {
    "CHAR"
}

# ===== BOOLEAN TYPES =====
PRIMITIVE_BOOLEAN: Set[str] = {
    "BOOLEAN"
}

# ===== INTEGER TYPES =====
PRIMITIVE_INT: Set[str] = {
    "INT",
    "SEQNUM",
    "NUMINGROUP",
    "LENGTH",
    "DAYOFMONTH"
}

# ===== FLOAT TYPES =====
PRIMITIVE_FLOAT: Set[str] = {
    "PRICE",
    "QTY",
    "AMT",
    "FLOAT",
    "PRICEOFFSET",
    "PERCENTAGE"
}

# ===== STRING TYPES =====
STRING_LIKE_TYPES: Set[str] = {
    "STRING",
    "MULTIPLEVALUESTRING",
    "MULTIPLESTRINGVALUE",
    "MULTIPLECHARVALUE",
    "EXCHANGE",
    "COUNTRY",
    "CURRENCY",
    "LANGUAGE",
    "XMLDATA",
    "DATA",
    "MONTHYEAR",
    "UTCDATE",
}

# ===== INTEGER-LIKE TYPES =====
INT_LIKE_TYPES: Set[str] = {
    "INT",
    "DAYOFMONTH",
    "LENGTH",
    "SEQNUM",
    "NUMINGROUP",
}

# ===== FLOAT-LIKE TYPES =====
FLOAT_LIKE_TYPES: Set[str] = {
    "AMT",
    "QTY",
    "PRICE",
    "PRICEOFFSET",
    "FLOAT",
    "PERCENTAGE",
}

# ===== TEMPORAL TYPES =====
TEMPORAL_TYPES: Set[str] = {
    "UTCTIMESTAMP",
    "UTCDATEONLY",
    "UTCTIMEONLY",
    "LOCALMKTDATE",
    "TZTIMEONLY",
}

TEMPORAL_DATE_TYPES: Set[str] = {
    "UTCDATEONLY",
    "LOCALMKTDATE",
}

TEMPORAL_TIME_TYPES: Set[str] = {
    "UTCTIMEONLY",
    "TZTIMEONLY",
}

TEMPORAL_TIMESTAMP_TYPES: Set[str] = {
    "UTCTIMESTAMP",
}


# ===== TYPE CLASSIFICATION =====

def is_string_like(fix_type: str) -> bool:
    """Check if FIX type is string-like."""
    return (fix_type or "").upper() in STRING_LIKE_TYPES


def classify_fix_type(ftype: str) -> str:
    """
    Classifica tipo FIX em uma categoria.

    Retorna: "string", "int", "float", "bool", "char", "timestamp", "date", "timeonly"
    """
    t = ftype.upper()

    if t in STRING_LIKE_TYPES:
        return "string"

    if t in PRIMITIVE_INT:
        return "int"

    if t in PRIMITIVE_FLOAT:
        return "float"

    if t == "BOOLEAN":
        return "bool"

    if t == "CHAR":
        return "char"

    if t in TEMPORAL_TYPES:
        if t == "UTCTIMESTAMP":
            return "timestamp"
        if t == "UTCDATEONLY" or t == "LOCALMKTDATE":
            return "date"
        if t == "UTCTIMEONLY":
            return "timeonly"

    return "string"


def resolve_cpp_type(field_name: str, fix_type: str, string_size_map: Dict[str, int] = None) -> str:
    """
    Resolve the C++ type based on classify_fix_type() and string_size_map.

    Args:
        field_name: FIX field name
        fix_type: FIX type (e.g., "STRING", "INT", "PRICE")
        string_size_map: String size mapping (field -> size or type -> size)

    Returns:
        C++ type (e.g., "STRING<32>", "INT", "FLOAT")
    """
    if string_size_map is None:
        string_size_map = {"default": 32}

    t = fix_type.upper()
    category = classify_fix_type(t)

    # STRING-LIKE: STRING<N>, CURRENCY<N>, EXCHANGE<N>, etc.
    if category == "string":
        from .strings import normalize_name
        fname = normalize_name(field_name)

        # Lookup by field name first
        if fname in string_size_map:
            sz = string_size_map[fname]
        # Then by type
        elif t in string_size_map:
            sz = string_size_map[t]
        # Fallback
        else:
            sz = string_size_map.get("default", 32)

        return f"{t}<{sz}>"

    # INTEGERS
    if category == "int":
        return t

    # FLOAT
    if category == "float":
        return "FLOAT"

    # BOOLEAN
    if category == "bool":
        return "BOOLEAN"

    # CHAR
    if category == "char":
        return "CHAR"

    # TEMPORAL
    if category == "timestamp":
        return "UTCTIMESTAMP"
    if category == "date":
        return "UTCDATEONLY"
    if category == "timeonly":
        return "UTCTIMEONLY"

    # Fallback
    return "STRING<32>"


def max_value_encode_bytes(fix_type: str, field_name: str, string_size_map: Dict[str, int] | None = None) -> int:
    """
    Returns the maximum number of bytes for the encoded VALUE of a FIX field
    (excluding the tag number, '=', and SOH delimiter).

    Type mapping:
      INT/SEQNUM/LENGTH/NUMINGROUP/DAYOFMONTH (int64_t): 20 bytes
      FLOAT/PRICE/QTY/AMT/PRICEOFFSET/PERCENTAGE (double): 24 bytes
      BOOLEAN: 1 byte
      CHAR: 1 byte
      UTCDATEONLY/LOCALMKTDATE (YYYYMMDD as int32): 8 bytes
      UTCTIMEONLY (HHMMSSmmm as int32): 9 bytes
      UTCTIMESTAMP (YYYYMMDDHHMMSSmmm as int64): 17 bytes
      STRING-like<N>: N bytes
    """
    if string_size_map is None:
        string_size_map = {}
    t = (fix_type or "STRING").upper()
    if t in INT_LIKE_TYPES:
        return 20
    if t in FLOAT_LIKE_TYPES:
        return 24
    if t == "BOOLEAN":
        return 1
    if t == "CHAR":
        return 1
    if t in TEMPORAL_DATE_TYPES:
        return 8
    if t in TEMPORAL_TIME_TYPES:
        return 9
    if t in TEMPORAL_TIMESTAMP_TYPES:
        return 17
    # STRING-like: look up by field name, then type, then default
    from .strings import normalize_name
    fname = normalize_name(field_name)
    if fname in string_size_map:
        return int(string_size_map[fname])
    if t in string_size_map:
        return int(string_size_map[t])
    return int(string_size_map.get("default", 32))
