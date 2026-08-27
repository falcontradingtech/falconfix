# -*- coding: utf-8 -*-

# SPDX-License-Identifier: MIT
# Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo

"""
generator_string_sizes.py
--------------------------------------
Generates config/string_sizes.yaml based on:
 - Se XML existe  -> varre todos os campos STRING-like reais
 - If XML does not exist -> generates rich template based on heuristics

Uses:
   - xml_parser.parse()
   - common.normalize_name()
   - common_types.classify_fix_type()
"""

import yaml
from pathlib import Path

from code.common import normalize_name, classify_fix_type, open_generated_file, write_generated_line_header

# ---------------------------------------------------------------------
# Types that depend on size (STRING-like)
# ---------------------------------------------------------------------
STRING_TYPES = {
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
}

# =====================================================================
#  Case XML exists
# =====================================================================
def _generate_from_xml(xml_path: Path, yaml_path: Path):
    from code.xml_parser import parse

    spec = parse(xml_path)
    fields = spec.get("fields", [])

    out = {
        "default": 32,
        "categories": {
            "id": 32,
            "medium_id": 64,
            "large_id": 128,
        },
        "fields": {},
    }

    fields_map = out["fields"]

    for f in fields:
        name = normalize_name(f["name"])
        ftype = f.get("type", "").upper()

        category = classify_fix_type(ftype)

        # we want only STRING-like
        if category != "string":
            continue

        if ftype in STRING_TYPES:
            # heuristic: decides size based on name
            lname = name.lower()

            if "id" in lname and "cl" not in lname:
                size = 32
            elif "symbol" in lname:
                size = 64
            elif "security" in lname:
                size = 64
            elif "xml" in lname or "data" in lname:
                size = 128
            else:
                size = 32

            fields_map[name] = size

    with open_generated_file(yaml_path, encoding="utf-8") as fh:
        write_generated_line_header(fh, "#")
        fh.write(yaml.dump(out, sort_keys=False))

    print(f"[ok] Generated string_sizes.yaml using XML: {yaml_path}")
    return out


# =====================================================================
#  Case XML DOES NOT exist -> powerful template
# =====================================================================
def _generate_default(yaml_path: Path):
    """
    Generates detailed template even without XML.
    """
    out = {
        "default": 32,

        # General categories
        "categories": {
            "id": 32,
            "code": 32,
            "medium_id": 64,
            "large_id": 128,
            "xml": 128,
            "data": 128,
            "currency": 8,
            "country": 4,
            "exchange": 8,
        },

        # Pre-suggested fields
        "fields": {
            # Common heuristics in universal FIX
            "Symbol": 64,
            "SecurityID": 64,
            "SecurityIDSource": 8,
            "SecurityExchange": 16,
            "Issuer": 128,
            "EncodedIssuer": 256,
            "EncodedIssuerLen": 8,
            "Text": 128,
            "EncodedText": 128,
            "EncodedTextLen": 8,
            "Account": 32,
            "ClOrdID": 32,
            "OrderID": 32,
            "ExecID": 32,
            "TradeID": 32,
            "CheckSum": 4,
        }
    }

    with open_generated_file(yaml_path, encoding="utf-8") as fh:
        write_generated_line_header(fh, "#")
        fh.write(yaml.dump(out, sort_keys=False))

    print(f"[ok] Generated DEFAULT string_sizes.yaml (no XML found): {yaml_path}")
    return out

# =====================================================================
#  Main function
# =====================================================================
def generate_or_validate_string_sizes(xml_path: Path, yaml_path: Path):
    """
    xml *may* exist.
       - If exists -> use real FIX content
       - If does not exist -> generate general template, but detailed
    """
    yaml_path.parent.mkdir(parents=True, exist_ok=True)
    if yaml_path.exists():
        print(f"[ok] Using existing {yaml_path}")
        return

    if xml_path.exists():
        return _generate_from_xml(xml_path, yaml_path)
    else:
        return _generate_default(yaml_path)
