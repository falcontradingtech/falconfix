# -*- coding: utf-8 -*-

# SPDX-License-Identifier: MIT
# Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo

"""
generator_enums.py
-------------------------------------------------
Generates optimized FIX enums for HFT (no std::string),
with support for CHAR, MULTIPLEVALUESTRING, BOOLEAN and STRING-like.

New integration:
- Uses common_strings.normalize_name
- Uses common_paths.namespace_from_spec
- Uses common_code.write_autogen_header
- Writes directly to core/enums.h (without creating 'core/' again)
"""

from pathlib import Path
from datetime import datetime  # still useful if you want extra logging

from code.common import normalize_name, namespace_from_spec, open_generated_file, write_autogen_header


# ------------------------------------------------------------
# Safe identifier (resolves 100% of previous issues)
# ------------------------------------------------------------
def safe_enum_name(raw: str, index: int) -> str:
    """
    Converts an enum name (description) to a valid C++ identifier.
    If empty → replaces with V_<index>.
    """
    if raw is None:
        return f"V_{index}"

    name = raw.strip()
    if not name:
        return f"V_{index}"

    # safe characters
    name = (
        name.replace(" ", "_")
            .replace("-", "_")
            .replace("/", "_")
            .replace(".", "_")
            .replace("(", "_")
            .replace(")", "_")
            .replace(",", "_")
            .replace(";", "_")
            .replace(":", "_")
            .replace("&", "_")
            .replace("%", "_")
    )

    # if starts with digit → prefix
    if name[0].isdigit():
        name = "V_" + name

    # compact underscores
    while "__" in name:
        name = name.replace("__", "_")
    name = name.strip("_")
    if not name:
        return f"V_{index}"

    return name


def assert_unique_member_names(enum_name: str, values):
    """
    Generates member names for an enum and aborts on collision.
    """
    result = []
    first_seen = {}

    for idx, v in enumerate(values):
        name = safe_enum_name(v.get("description"), idx)
        if name in first_seen:
            prev_idx, prev_desc, prev_enum = first_seen[name]
            cur_desc = v.get("description", "")
            cur_enum = v.get("enum", "")
            raise ValueError(
                "Enum member collision in "
                f"{enum_name}: normalized member '{name}' is duplicated. "
                f"First value idx={prev_idx}, enum='{prev_enum}', description='{prev_desc}'. "
                f"Second value idx={idx}, enum='{cur_enum}', description='{cur_desc}'."
            )

        first_seen[name] = (idx, v.get("description", ""), v.get("enum", ""))
        result.append(name)

    return result


def _cxx_char_literal(ch: str) -> str:
    escaped = {
        "\\": "\\\\",
        "'": "\\'",
        "\n": "\\n",
        "\r": "\\r",
        "\t": "\\t",
        "\0": "\\0",
    }
    return "'" + escaped.get(ch, ch) + "'"


# ------------------------------------------------------------
# Emission of string-like enums
# ------------------------------------------------------------
def _emit_msg_type_parser(fh, values, member_names):
    by_len = {}
    for idx, v in enumerate(values):
        val = v.get("enum", "")
        if not val:
            continue
        by_len.setdefault(len(val), []).append((val, member_names[idx]))

    fh.write("[[nodiscard]] inline bool parse_MsgType(const char *begin,\n")
    fh.write("                                       const char *end,\n")
    fh.write("                                       MsgType &out) noexcept {\n")
    fh.write("    out = MsgType::UNKNOWN;\n\n")
    fh.write("    if (!begin || !end || begin >= end) {\n")
    fh.write("        return false;\n")
    fh.write("    }\n\n")
    fh.write("    const auto len = static_cast<std::size_t>(end - begin);\n\n")
    fh.write("    switch (len) {\n")

    for msg_len in sorted(by_len):
        entries = by_len[msg_len]
        fh.write(f"        case {msg_len}:\n")

        if msg_len == 1:
            fh.write("            switch (begin[0]) {\n")
            for val, desc in entries:
                fh.write(f"                case {_cxx_char_literal(val[0])}: out = MsgType::{desc}; return true;\n")
            fh.write("                default: return false;\n")
            fh.write("            }\n\n")
            continue

        for val, desc in entries:
            cond = " && ".join(
                f"begin[{i}] == {_cxx_char_literal(ch)}"
                for i, ch in enumerate(val)
            )
            fh.write(f"            if ({cond}) {{\n")
            fh.write(f"                out = MsgType::{desc};\n")
            fh.write("                return true;\n")
            fh.write("            }\n")
        fh.write("            break;\n\n")

    fh.write("        default:\n")
    fh.write("            break;\n")
    fh.write("    }\n\n")
    fh.write("    return false;\n")
    fh.write("}\n\n")


def _emit_string_enum(fh, enum_name: str, values):
    """
    STRING, MULTIPLECHAR, other non-CHAR types.
    Generates enum + to_cstr() + parse().
    """
    member_names = assert_unique_member_names(enum_name, values)

    fh.write(f"enum class {enum_name} : uint8_t {{\n")

    for idx, _v in enumerate(values):
        desc = member_names[idx]
        fh.write(f"    {desc},\n")

    if enum_name == "MsgType":
        fh.write("    UNKNOWN = 255,\n")

    fh.write("};\n\n")

    # to_cstr()
    fh.write(f"[[nodiscard]] constexpr const char *to_cstr({enum_name} v) noexcept {{\n")
    fh.write("    switch (v) {\n")

    for idx, v in enumerate(values):
        desc = member_names[idx]
        val = v.get("enum", "")
        fh.write(f"        case {enum_name}::{desc:<32}: return \"{val}\";\n")

    if enum_name == "MsgType":
        fh.write(f"        case {enum_name}::UNKNOWN{'':<30}: return \"\";\n")

    fh.write("    }\n")
    fh.write("    return \"\";\n")
    fh.write("}\n\n")

    if enum_name == "MsgType":
        _emit_msg_type_parser(fh, values, member_names)
        return

    # parse()
    fh.write(f"[[nodiscard]] inline bool parse_{enum_name}(const char *s, {enum_name} &out) noexcept {{\n")
    fh.write("    if (!s) return false;\n")

    for idx, v in enumerate(values):
        desc = member_names[idx]
        val = v.get("enum", "")
        fh.write(f"    if (std::strcmp(s, \"{val}\") == 0) {{ out = {enum_name}::{desc}; return true; }}\n")

    fh.write("    return false;\n")
    fh.write("}\n\n")


# ------------------------------------------------------------
# Emission of MULTIPLEVALUESTRING bitmask
# ------------------------------------------------------------
def _emit_bitmask_enum(fh, enum_name: str, values):
    member_names = assert_unique_member_names(enum_name, values)

    fh.write(f"enum class {enum_name}Bit : uint8_t {{\n")
    for idx, _v in enumerate(values):
        desc = member_names[idx]
        fh.write(f"    {desc},\n")
    fh.write("};\n\n")

    fh.write(f"using {enum_name}Mask = uint64_t;\n\n")

    fh.write(f"constexpr {enum_name}Mask bit({enum_name}Bit b) noexcept {{ return 1ull << (uint8_t)b; }}\n")
    fh.write(f"constexpr {enum_name}Mask set({enum_name}Mask m, {enum_name}Bit b) noexcept {{ return m | bit(b); }}\n")
    fh.write(f"constexpr bool has({enum_name}Mask m, {enum_name}Bit b) noexcept {{ return (m & bit(b)) != 0; }}\n\n")

    # fast parser
    fh.write(f"[[nodiscard]] inline bool parse_{enum_name}(const char *s, {enum_name}Mask &out) noexcept {{\n")
    fh.write("    out = 0;\n")
    fh.write("    if (!s) return false;\n")
    fh.write("    const char *p = s;\n")
    fh.write("    while (*p) {\n")
    fh.write("        if (*p == ' ') { ++p; continue; }\n")
    fh.write("        switch (*p) {\n")

    for idx, v in enumerate(values):
        desc = member_names[idx]
        val = v.get("enum", "")
        if not val:
            continue
        fh.write(f"            case '{val[0]}': out |= bit({enum_name}Bit::{desc}); break;\n")

    fh.write("        }\n")
    fh.write("        ++p;\n")
    fh.write("    }\n")
    fh.write("    return true;\n")
    fh.write("}\n\n")

    # to_string
    fh.write(f"[[nodiscard]] inline const char *to_string_{enum_name}({enum_name}Mask m, char *buf, std::size_t cap) noexcept {{\n")
    fh.write("    std::size_t pos = 0;\n")
    fh.write("    auto append = [&](const char *t){ "
             "std::size_t len=std::strlen(t); "
             "if(pos+len+1<cap){ std::memcpy(buf+pos,t,len); pos+=len; buf[pos++]=' '; } };\n")

    for idx, v in enumerate(values):
        desc = member_names[idx]
        val = v.get("enum", "")
        if not val:
            continue
        fh.write(f"    if (has(m, {enum_name}Bit::{desc})) append(\"{val}\");\n")

    fh.write("    if (pos > 0) buf[pos-1] = '\\0'; else if (cap > 0) buf[0] = '\\0';\n")
    fh.write("    return buf;\n")
    fh.write("}\n\n")


# ------------------------------------------------------------
# CHAR (single-char only)
# ------------------------------------------------------------
def _emit_char_enum(fh, enum_name: str, values):
    """
    CHAR → enum class X : char { A='A', B='B', ... }
    """
    member_names = assert_unique_member_names(enum_name, values)

    fh.write(f"enum class {enum_name} : char {{\n")
    for idx, v in enumerate(values):
        desc = member_names[idx]

        enumv = v.get("enum", "")
        if not enumv:
            enumv = "_"  # total safety

        fh.write(f"    {desc:<40} = '{enumv}',\n")
    fh.write("};\n\n")


# ------------------------------------------------------------
# ENTRYPOINT
# ------------------------------------------------------------
def generate(core_dir: Path, spec: dict):
    """
    Generates include/.../core/enums.h
    - core_dir is already .../include/FIX44/core (falcon-gen handles this)
    """
    fields = spec.get("fields") or []
    ns = namespace_from_spec(spec)

    out_path = core_dir / "enums.h"

    with open_generated_file(out_path, encoding="utf-8") as fh:
        # standard FalconFIX header
        write_autogen_header(fh, "FIX Enums (HFT optimized)")

        fh.write("#pragma once\n\n")
        fh.write("#include <cstdint>\n")
        fh.write("#include <cstddef>\n")
        fh.write("#include <cstring>\n\n")

        fh.write("#ifdef _WIN32\n")
        fh.write("#pragma push_macro(\"ABSOLUTE\")\n")
        fh.write("#pragma push_macro(\"PERCENTAGE\")\n")
        fh.write("#pragma push_macro(\"PER_UNIT\")\n")
        fh.write("#pragma push_macro(\"CORRECTION\")\n")
        fh.write("#pragma push_macro(\"ERROR\")\n")
        fh.write("#pragma push_macro(\"DELETE\")\n")
        fh.write("#pragma push_macro(\"constant\")\n")
        fh.write("#undef ABSOLUTE\n")
        fh.write("#undef PERCENTAGE\n")
        fh.write("#undef PER_UNIT\n")
        fh.write("#undef CORRECTION\n")
        fh.write("#undef ERROR\n")
        fh.write("#undef DELETE\n")
        fh.write("#undef constant\n")
        fh.write("#endif\n\n")

        fh.write(f"namespace {ns} {{\n")
        fh.write("namespace enums {\n\n")

        # to ensure unique enum type names
        used_enum_type_names = set()

        for fdef in fields:
            values = fdef.get("values") or []
            if not values:
                continue

            raw_name = fdef.get("name") or ""
            if not raw_name:
                continue

            fname = normalize_name(raw_name)
            ftype = (fdef.get("type") or "STRING").upper()

            # ensures unique identifier for enum type
            base = fname
            while fname in used_enum_type_names:
                fname = base + "_DUP"
            used_enum_type_names.add(fname)

            # BOOLEAN → do not generate enum (Y/N)
            if ftype == "BOOLEAN":
                continue

            # CHAR
            if ftype == "CHAR":
                # rare case: CHAR with multi-char values → treat as string
                if any(len((v.get("enum") or "")) != 1 for v in values):
                    _emit_string_enum(fh, fname, values)
                else:
                    _emit_char_enum(fh, fname, values)
                continue

            # MULTIPLEVALUESTRING → bitmask
            if ftype == "MULTIPLEVALUESTRING":
                _emit_bitmask_enum(fh, fname, values)
                continue

            # fallback → string enum (STRING, etc.)
            _emit_string_enum(fh, fname, values)

        fh.write("} // namespace enums\n")
        fh.write(f"}} // namespace {ns}\n\n")

        fh.write("#ifdef _WIN32\n")
        fh.write("// Restore Windows macros\n")
        fh.write("#pragma pop_macro(\"constant\")\n")
        fh.write("#pragma pop_macro(\"DELETE\")\n")
        fh.write("#pragma pop_macro(\"ERROR\")\n")
        fh.write("#pragma pop_macro(\"CORRECTION\")\n")
        fh.write("#pragma pop_macro(\"PER_UNIT\")\n")
        fh.write("#pragma pop_macro(\"PERCENTAGE\")\n")
        fh.write("#pragma pop_macro(\"ABSOLUTE\")\n")
        fh.write("#endif\n")

    print(f"[ok] enums.h written -> {out_path}")
