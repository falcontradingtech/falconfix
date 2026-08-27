# -*- coding: utf-8 -*-

# SPDX-License-Identifier: MIT
# Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo

"""
generator_codec.py
-------------------------------------------------------
Generate utility headers required by generated messages:
- utils/parser.h
- utils/serializer.h
- utils/tag_constants.h
- utils/all_messages.h
"""

from pathlib import Path

from code.common import (
    normalize_name,
    normalize_version_string,
    namespace_from_spec,
    open_generated_file,
    write_autogen_header,
    write_generated_line_header,
)


def _tag_le_u32(tag_num: int) -> int:
    raw = f"{tag_num}=".encode("ascii")
    value = 0
    for i, ch in enumerate(raw[:4]):
        value |= ch << (8 * i)
    return value


def _write_parser_header(path: Path, ns: str):
    with open_generated_file(path, encoding="utf-8") as fh:
        write_generated_line_header(fh)
        fh.write("#pragma once\n\n")
        fh.write("#include <charconv>\n")
        fh.write("#include <cstdint>\n")
        fh.write("#include <utils/string_utils.h>\n\n")
        fh.write(f"namespace {ns}::parser {{\n\n")
        fh.write("[[nodiscard]] inline int64_t parse_int64(const char *b, const char *e) noexcept {\n")
        fh.write("    int64_t v = 0;\n")
        fh.write("    auto r = std::from_chars(b, e, v);\n")
        fh.write("    if (r.ec != std::errc()) return 0;\n")
        fh.write("    return v;\n")
        fh.write("}\n\n")
        fh.write("[[nodiscard]] inline int32_t parse_int32(const char *b, const char *e) noexcept {\n")
        fh.write("    int32_t v = 0;\n")
        fh.write("    auto r = std::from_chars(b, e, v);\n")
        fh.write("    if (r.ec != std::errc()) return 0;\n")
        fh.write("    return v;\n")
        fh.write("}\n\n")
        fh.write("[[nodiscard]] inline double parse_double(const char *b, const char *e) noexcept {\n")
        fh.write("    double v = 0.0;\n")
        fh.write("    auto r = std::from_chars(b, e, v, std::chars_format::general);\n")
        fh.write("    if (r.ec != std::errc()) return 0.0;\n")
        fh.write("    return v;\n")
        fh.write("}\n\n")
        fh.write("[[nodiscard]] inline bool parse_bool(const char *b, const char *e) noexcept {\n")
        fh.write("    if (b >= e) return false;\n")
        fh.write("    return (*b == 'Y');\n")
        fh.write("}\n\n")
        fh.write("[[nodiscard]] inline char parse_char(const char *b, const char *e) noexcept {\n")
        fh.write("    if (b >= e) return '\\0';\n")
        fh.write("    return *b;\n")
        fh.write("}\n\n")
        fh.write("[[nodiscard]] inline int32_t parse_date32(const char *b, const char *e) noexcept {\n")
        fh.write("    if (e - b < 8) return 0;\n")
        fh.write("    int32_t v = 0;\n")
        fh.write("    for (int32_t i = 0; i < 8; i++) {\n")
        fh.write("        char c = b[i];\n")
        fh.write("        if (c < '0' || c > '9') return 0;\n")
        fh.write("        v = v * 10 + (c - '0');\n")
        fh.write("    }\n")
        fh.write("    return v;\n")
        fh.write("}\n\n")
        fh.write("[[nodiscard]] inline int32_t parse_timeonly32(const char *b, const char *e) noexcept {\n")
        fh.write("    if (e - b < 8) return 0;\n")
        fh.write("    if (b[2] != ':' || b[5] != ':') return 0;\n")
        fh.write("    auto d = [](char c)->int32_t { return (c >= '0' && c <= '9') ? c - '0' : -1; };\n")
        fh.write("    int32_t h = d(b[0]) * 10 + d(b[1]);\n")
        fh.write("    int32_t m = d(b[3]) * 10 + d(b[4]);\n")
        fh.write("    int32_t s = d(b[6]) * 10 + d(b[7]);\n")
        fh.write("    if (h < 0 || m < 0 || s < 0) return 0;\n")
        fh.write("    int32_t ms = 0;\n")
        fh.write("    const char *p = b + 8;\n")
        fh.write("    if (p < e && *p == '.') {\n")
        fh.write("        ++p;\n")
        fh.write("        int32_t factor = 100;\n")
        fh.write("        while (p < e && factor > 0) {\n")
        fh.write("            if (*p < '0' || *p > '9') break;\n")
        fh.write("            ms += (*p - '0') * factor;\n")
        fh.write("            factor /= 10;\n")
        fh.write("            ++p;\n")
        fh.write("        }\n")
        fh.write("    }\n")
        fh.write("    return h * 10000000 + m * 100000 + s * 1000 + ms;\n")
        fh.write("}\n\n")
        fh.write("[[nodiscard]] inline int64_t parse_timestamp64(const char *b, const char *e) noexcept {\n")
        fh.write("    int64_t v = 0;\n")
        fh.write("    auto r = std::from_chars(b, e, v);\n")
        fh.write("    if (r.ec != std::errc()) return 0;\n")
        fh.write("    return v;\n")
        fh.write("}\n\n")
        fh.write(f"}} // namespace {ns}::parser\n")


def _write_serializer_header(path: Path, ns: str, include_root: str):
    with open_generated_file(path, encoding="utf-8") as fh:
        write_generated_line_header(fh)
        fh.write("#pragma once\n\n")
        fh.write("#include <array>\n")
        fh.write("#include <cstdint>\n")
        fh.write(f"#include <{include_root}/core/types.h>\n")
        fh.write(f"using namespace {ns}::types;\n\n")
        fh.write(f"namespace {ns}::serialize {{\n\n")
        fh.write("constexpr char SOH = '\\x01';\n\n")
        fh.write("inline void write_utctimeonly(char *&p, UTCTIMEONLY value, bool includeMilliseconds = true) noexcept {\n")
        fh.write("    const int32_t h = static_cast<int32_t>(value / 10000000);\n")
        fh.write("    const int32_t m = static_cast<int32_t>((value / 100000) % 100);\n")
        fh.write("    const int32_t s = static_cast<int32_t>((value / 1000) % 100);\n")
        fh.write("    const int32_t ms = static_cast<int32_t>(value % 1000);\n")
        fh.write("    p[0] = static_cast<char>('0' + ((h / 10) % 10));\n")
        fh.write("    p[1] = static_cast<char>('0' + (h % 10));\n")
        fh.write("    p[2] = ':';\n")
        fh.write("    p[3] = static_cast<char>('0' + ((m / 10) % 10));\n")
        fh.write("    p[4] = static_cast<char>('0' + (m % 10));\n")
        fh.write("    p[5] = ':';\n")
        fh.write("    p[6] = static_cast<char>('0' + ((s / 10) % 10));\n")
        fh.write("    p[7] = static_cast<char>('0' + (s % 10));\n")
        fh.write("    if (includeMilliseconds) {\n")
        fh.write("        p[8] = '.';\n")
        fh.write("        p[9] = static_cast<char>('0' + ((ms / 100) % 10));\n")
        fh.write("        p[10] = static_cast<char>('0' + ((ms / 10) % 10));\n")
        fh.write("        p[11] = static_cast<char>('0' + (ms % 10));\n")
        fh.write("        p += 12;\n")
        fh.write("        return;\n")
        fh.write("    }\n")
        fh.write("    p += 8;\n")
        fh.write("}\n\n")
        fh.write("template<std::size_t N>\n")
        fh.write("inline void set_presence(std::array<uint64_t, N>& bitmap, std::size_t index) noexcept {\n")
        fh.write("    const std::size_t word = index / 64;\n")
        fh.write("    const std::size_t bit = index % 64;\n")
        fh.write("    bitmap[word] |= (uint64_t(1) << bit);\n")
        fh.write("}\n\n")
        fh.write("template<std::size_t N>\n")
        fh.write("inline void clear_presence(std::array<uint64_t, N>& bitmap, std::size_t index) noexcept {\n")
        fh.write("    const std::size_t word = index / 64;\n")
        fh.write("    const std::size_t bit = index % 64;\n")
        fh.write("    bitmap[word] &= ~(uint64_t(1) << bit);\n")
        fh.write("}\n\n")
        fh.write("template<std::size_t N>\n")
        fh.write("inline bool is_present(const std::array<uint64_t, N>& bitmap, std::size_t index) noexcept {\n")
        fh.write("    const std::size_t word = index / 64;\n")
        fh.write("    const std::size_t bit = index % 64;\n")
        fh.write("    return (bitmap[word] >> bit) & 1;\n")
        fh.write("}\n\n")
        fh.write(f"}} // namespace {ns}::serialize\n")


def _write_tag_constants(path: Path, ns: str, spec: dict):
    fields = spec.get("fields", [])
    with open_generated_file(path, encoding="utf-8") as fh:
        write_autogen_header(fh, "Pre-encoded FIX tag constants")
        fh.write("#pragma once\n\n")
        fh.write("#include <cstdint>\n\n")
        fh.write(f"namespace {ns}::tag_const {{\n\n")
        for f in fields:
            name = normalize_name(f.get("name", ""))
            number = f.get("number")
            if not name or number is None:
                continue
            le = _tag_le_u32(int(number))
            fh.write(f"inline constexpr uint32_t {name:<36} = 0x{le:08x}u; // {number}=\n")
        fh.write(f"\n}} // namespace {ns}::tag_const\n")


def _write_all_messages(path: Path, include_root: str, spec: dict):
    with open_generated_file(path, encoding="utf-8") as fh:
        write_generated_line_header(fh)
        fh.write("#pragma once\n\n")
        for msg in spec.get("messages", []):
            mname = normalize_name(msg.get("name", ""))
            if not mname:
                continue
            fh.write(f"#include <{include_root}/messages/{mname}.h>\n")


def generate(codec_dir: Path, utils_dir: Path, spec: dict):
    """
    Keep signature compatibility with previous pipeline.
    codec_dir is currently unused; required outputs are under utils_dir.
    """
    _ = codec_dir
    ns = namespace_from_spec(spec)
    version = spec.get("fix_version") or spec.get("version")
    if not version:
        raise ValueError("spec is missing FIX version (expected 'fix_version' or 'version')")
    include_root = normalize_version_string(version)

    utils_dir.mkdir(parents=True, exist_ok=True)
    _write_parser_header(utils_dir / "parser.h", ns)
    _write_serializer_header(utils_dir / "serializer.h", ns, include_root)
    _write_tag_constants(utils_dir / "tag_constants.h", ns, spec)
    _write_all_messages(utils_dir / "all_messages.h", include_root, spec)
