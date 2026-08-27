# -*- coding: utf-8 -*-

# SPDX-License-Identifier: MIT
# Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo

"""
generator_fields.py — unified version
Generates:
  - core/field_id.h
  - core/fields.h
  - core/null_fields.h
No hardcoded types: everything derived from resolve_cpp_type() and classify_fix_type().
"""

from pathlib import Path
from code.common import (
    open_generated_file,
    write_autogen_header,
    namespace_from_spec,
    normalize_version_string,
    normalize_name,
    classify_fix_type,
    resolve_cpp_type,
    PRIMITIVE_INT,
    PRIMITIVE_FLOAT,
    TEMPORAL_TYPES,
    STRING_LIKE_TYPES,
)

# ------------------------------------------------------------
# null alignment
# ------------------------------------------------------------
def _align_defines(lines, type_col=10, name_col=15):
    """
    Format:
        <TYPE> <spaces> <NULL_NAME> <spaces> = <VALUE>;
    """
    out = []
    for T, N, V in lines:
        st = " " * max(1, type_col - len(T))
        sn = " " * max(1, name_col - len(N))
        out.append(f"{T}{st}{N}{sn} {V};")
    return "\n".join(out)


def _align(lines, type_col=45, name_col=45):
    """
    Format:
        <TYPE> <spaces> <NULL_NAME> <spaces> = <VALUE>;
    """
    out = []
    for T, N, V in lines:
        st = " " * max(1, type_col - len(T))
        sn = " " * max(1, name_col - len(N))
        out.append(f"{T}{st}{N}{sn}= {V};")
    return "\n".join(out)

# ------------------------------------------------------------
# type → NULL_<TYPE> name
# example:
#   UTCTimestamp → NULL_UTCTIMESTAMP
#   PRICE → NULL_PRICE
#   STRING<32> → NULL_STRING
# ------------------------------------------------------------
def _null_name_for_type(cpp_type: str) -> str:
    # STRING<32> → STRING
    if "<" in cpp_type:
        base = cpp_type.split("<", 1)[0]
        return f"NULL_{base.upper()}"

    return f"NULL_{cpp_type.upper()}"

# ------------------------------------------------------------
# Resolve sentinel for the type
# ------------------------------------------------------------
def _null_value_for_type(category: str) -> str:
    if category == "float":
        return "NULL_DOUBLE"
    if category == "int":
        return "NULL_INT32"
    if category == "bool":
        return "false"
    if category == "char":
        return "NULL_CHAR"
    if category == "timestamp":
        return "NULL_INT64"
    if category == "date":
        return "NULL_INT32"
    if category == "timeonly":
        return "NULL_INT32"
    if category == "string":
        return None  # will become "{}" of the type
    return None

# ============================================================
# ENTRYPOINT PRINCIPAL
# ============================================================
def generate(core_dir: Path, spec: dict):
    fields = spec.get("fields") or []
    ns = namespace_from_spec(spec)
    version = spec.get("fix_version") or spec.get("version")
    if not version:
        raise ValueError("spec is missing FIX version (expected 'fix_version' or 'version')")
    include_root = normalize_version_string(version)

    # ========================================================
    # 1. field_id.h
    # ========================================================
    path_field_id = core_dir / "field_id.h"

    with open_generated_file(path_field_id, encoding="utf-8") as fh:
        write_autogen_header(fh, "FIX Field IDs")
        fh.write("#pragma once\n\n")
        fh.write("#include <cstdint>\n\n")

        fh.write(f"namespace {ns} {{\n")
        fh.write("namespace field_id {\n\n")
        fh.write("using Tag = int32_t;\n\n")

        for f in fields:
            name = normalize_name(f["name"])
            tag = f["number"]
            fh.write(f"constexpr Tag {name:<39} = {tag};\n")

        fh.write("\n} // namespace field_id\n")
        fh.write(f"}} // namespace {ns}\n")

    print(f"[ok] field_id.h → {path_field_id}")

    # ========================================================
    # 2. fields.h
    # ========================================================
    path_fields = core_dir / "fields.h"

    with open_generated_file(path_fields, encoding="utf-8") as fh:
        write_autogen_header(fh, "FIX Field Type Aliases")
        fh.write("#pragma once\n\n")
        fh.write(f"#include <{include_root}/core/types.h>\n")
        fh.write("\n")

        fh.write(f"namespace {ns} {{\n")
        fh.write("namespace fields {\n\n")

        fh.write("// Automatic aliases for each FIX field\n\n")

        for f in fields:
            name = normalize_name(f["name"])
            fix_type = f["type"]
            cpp = resolve_cpp_type(name, fix_type).upper()
            fh.write(f"using {name:<36} = {ns}::types::{cpp};\n")

        fh.write("\n} // namespace fields\n")
        fh.write(f"}} // namespace {ns}\n")

    print(f"[ok] fields.h → {path_fields}")

    # ========================================================
    # 3. null_fields.h
    # ========================================================
    path_nulls = core_dir / "null_fields.h"

    # --------------------------------------------------------
    # Build list of aliases from types.h (dynamic)
    # --------------------------------------------------------
    # --------------------------------------------------------
    # aliases from types.h (dynamic via categories)
    # --------------------------------------------------------
    alias_types = set()

    alias_types.add("BOOLEAN")
    alias_types.add("CHAR")

    alias_types.update(PRIMITIVE_INT)
    alias_types.update(PRIMITIVE_FLOAT)
    alias_types.update(TEMPORAL_TYPES)
    alias_types.update(STRING_LIKE_TYPES)

    alias_types = sorted(alias_types)

    with open_generated_file(path_nulls, encoding="utf-8") as fh:
        write_autogen_header(fh, "Null Fields")

        fh.write("#pragma once\n\n")
        fh.write("#include <cstdint>\n#include <limits>\n")
        fh.write(f"#include <{include_root}/core/types.h>\n")
        fh.write("\n")

        fh.write(f"namespace {ns} {{\n")
        fh.write("namespace fields {\n\n")

        # ----------------------------------------------------
        # Generic NULLs (numeric + char)
        # ----------------------------------------------------
        #generic_lines = [
        #    ("inline constexpr double",  "NULL_DOUBLE", "std::numeric_limits<double>::quiet_NaN()"),
        #    ("inline constexpr int64_t", "NULL_INT64",  "std::numeric_limits<int64_t>::min()"),
        #    ("inline constexpr int32_t", "NULL_INT32",  "std::numeric_limits<int32_t>::min()"),
        #    ("inline constexpr CHAR",    "NULL_CHAR",   "'\\0'"),
        #]
        generic_lines = [
            ("inline constexpr bool",   "DNULL_BOOLEAN", "false"),
            ("inline constexpr double", "DNULL_DOUBLE",  "std::numeric_limits<double>::quiet_NaN()"),
            ("inline constexpr int64_t", "DNULL_INT64",  "std::numeric_limits<int64_t>::min()"),
            ("inline constexpr int32_t", "DNULL_INT32",  "std::numeric_limits<int32_t>::min()"),
            ("inline constexpr char",   "DNULL_CHAR",    "'\\0'"),
        ]
        fh.write("// Generic sentinels\n")
        fh.write(_align(generic_lines, type_col=28, name_col=14))
        fh.write("\n\n")

        # ----------------------------------------------------
        # NULL per TYPE (BOOLEAN, INT, PRICE, UTCTIMESTAMP, ...)
        #   - STRING-like: no NULL per type (only per field)
        # ----------------------------------------------------
        per_type_lines = []

        for t in alias_types:
            cat = classify_fix_type(t)  # uses FIX type name (INT, PRICE, STRING...)

            # String-like → ignore here (we generate per field)
            if cat == "string":
                continue

            null_name = f"NULL_{t}"
            type_decl = f"inline constexpr {ns}::types::{t}"

            if cat == "bool":
                value = "DNULL_BOOLEAN"
            elif cat == "char":
                value = "DNULL_CHAR"
            elif cat == "float":
                value = "DNULL_DOUBLE"
            elif cat == "int":
                # all PRIMITIVE_INT + DAYOFMONTH use int64_t in their types.h
                value = "DNULL_INT64"
            elif cat == "timestamp":
                # UTCTIMESTAMP (int64_t)
                value = "DNULL_INT64"
            elif cat in ("date", "timeonly"):
                # UTCDATEONLY, UTCTIMEONLY, LOCALMKTDATE (int32_t)
                value = "DNULL_INT32"
            else:
                value = "{}"

            per_type_lines.append((type_decl, null_name, value))

        fh.write("// Per-type nulls\n")
        fh.write(_align(per_type_lines))
        fh.write("\n\n")

        # ----------------------------------------------------
        # NULL per FIELD — only STRING-like
        #   Ex: inline constexpr STRING<32> Symbol_NULL = STRING<32>{};
        # ----------------------------------------------------
        field_lines = []

        for f in fields:
            fname = normalize_name(f["name"])
            ftype = f["type"]

            cpp_type = resolve_cpp_type(fname, ftype)   # ex: STRING<32>, CURRENCY<3>, INT, PRICE...
            base = cpp_type.split("<", 1)[0]            # STRING, CURRENCY, INT, PRICE...

            cat = classify_fix_type(base)

            if cat != "string":
                # Only generate null per field for STRING-like
                continue

            value = f"{ns}::types::{cpp_type}{{}}"
            var_name = f"{fname}_NULL"
            type_decl = f"inline constexpr {ns}::types::{cpp_type}"

            field_lines.append((type_decl, var_name, value))

        fh.write("// Per-field nulls (only for string-like types)\n")
        fh.write(_align(field_lines))
        fh.write("\n\n")

        fh.write("} // namespace fields\n")
        fh.write(f"}} // namespace {ns}\n")

    print(f"[ok] null_fields.h written -> {path_nulls}")
