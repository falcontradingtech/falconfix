# -*- coding: utf-8 -*-

# SPDX-License-Identifier: MIT
# Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo

"""
generator_messages.py
-------------------------------------------------------
Message/header/trailer generator.

Design goals:
- XML-driven generation (no fallback copy path)
- Stable item iterator shared with other generators
"""

from pathlib import Path
from typing import Dict

from code.common import (
    open_generated_file,
    write_autogen_header,
    namespace_from_spec,
    normalize_version_string,
    normalize_name,
    resolve_cpp_type,
    is_string_like,
    max_value_encode_bytes,
)


def _iter_message_items(obj: dict, all_components_map: dict):
    """
    Iterate fields/groups/components preserving XML declaration order.
    """
    groups_by = {normalize_name(g["name"]): g for g in obj.get("groups", [])}
    fields_by = {normalize_name(f["name"]): f for f in obj.get("fields", [])}

    raw_comps = obj.get("components", [])
    comps_by = {}
    for comp in raw_comps:
        if isinstance(comp, dict):
            cname = normalize_name(comp["name"])
            comps_by[cname] = comp
        else:
            cname = normalize_name(comp)
            resolved = all_components_map.get(cname)
            if resolved is not None:
                comps_by[cname] = resolved

    raw = obj.get("raw_children") or []

    if raw:
        for entry in raw:
            tag = (entry.get("tag") or "").lower()
            name = normalize_name(entry.get("name") or "")
            if not name:
                continue

            if tag == "field":
                payload = fields_by.get(name)
                if payload:
                    yield "field", name, payload

            elif tag == "group":
                payload = groups_by.get(name)
                if payload:
                    yield "group", name, payload

            elif tag == "component":
                payload = comps_by.get(name)
                if payload is None:
                    payload = all_components_map.get(name)
                if payload is not None:
                    yield "component", name, payload
        return

    for field in obj.get("fields", []):
        yield "field", normalize_name(field["name"]), field

    for group in obj.get("groups", []):
        yield "group", normalize_name(group["name"]), group

    for comp in obj.get("components", []):
        if isinstance(comp, dict):
            yield "component", normalize_name(comp["name"]), comp
        else:
            cname = normalize_name(comp)
            payload = all_components_map.get(cname) or {
                "name": cname,
                "fields": [],
                "groups": [],
                "components": [],
                "raw_children": [],
            }
            yield "component", cname, payload


def _collect_component_deps(obj: dict, all_components_map: dict) -> set[str]:
    deps: set[str] = set()

    for tag, name, payload in _iter_message_items(obj, all_components_map):
        if tag == "component":
            deps.add(name)
            if isinstance(payload, dict):
                deps.update(_collect_component_deps(payload, all_components_map))
        elif tag == "group" and isinstance(payload, dict):
            deps.update(_collect_component_deps(payload, all_components_map))

    return deps


def _build_context(spec: dict):
    ns = namespace_from_spec(spec)
    version = spec.get("fix_version") or spec.get("version")
    if not version:
        raise ValueError("spec is missing FIX version (expected 'fix_version' or 'version')")
    include_root = normalize_version_string(version)

    fields_catalog = spec.get("fields", [])
    field_type_map = {
        normalize_name(f["name"]): (f.get("type") or "STRING").upper()
        for f in fields_catalog
    }
    name_to_num = {
        normalize_name(f["name"]): f.get("number")
        for f in fields_catalog
        if f.get("name")
    }
    all_components_map = {
        normalize_name(c["name"] if isinstance(c, dict) else c): c
        for c in spec.get("components", [])
    }
    return ns, include_root, field_type_map, name_to_num, all_components_map


def _required_mask_expr(required_names: list[str]) -> str:
    if not required_names:
        return "0ULL"
    return " |\n        ".join([f"P_{name}" for name in required_names])


def _presence_nwords(bit_count: int) -> int:
    return max(1, (bit_count + 63) // 64)


def _required_words_expr(required_names: list[str], bit_positions: dict[str, int], nwords: int, indent: str) -> str:
    if not required_names:
        return ",\n".join([f"{indent}0ULL" for _ in range(nwords)])

    words: list[list[str]] = [[] for _ in range(nwords)]
    for name in required_names:
        bit = bit_positions[name]
        words[bit // 64].append(f"(1ULL << {bit % 64})")

    return ",\n".join(
        f"{indent}{' | '.join(parts) if parts else '0ULL'}"
        for parts in words
    )


def _field_init(name: str, fix_type: str) -> str:
    if is_string_like(fix_type):
        return f"fields::{name}_NULL"
    return f"fields::NULL_{fix_type}"


def _write_has_any_set(fh, indent: str, nwords: int, component_items: list[str], group_items: list[str] | None = None):
    group_items = group_items or []
    fh.write(f"{indent}bool hasAnySet() const noexcept {{\n")
    if nwords == 1:
        fh.write(f"{indent}    if (m_presence != 0) {{ return true; }}\n")
    else:
        fh.write(f"{indent}    for (std::size_t i = 0; i < NWORDS; ++i) {{\n")
        fh.write(f"{indent}        if (m_presence[i] != 0) {{ return true; }}\n")
        fh.write(f"{indent}    }}\n")
    for name in component_items:
        fh.write(f"{indent}    if (m_{name}.hasAnySet()) {{ return true; }}\n")
    for name in group_items:
        fh.write(f"{indent}    if (!m_{name}Group.empty()) {{ return true; }}\n")
    fh.write(f"{indent}    return false;\n")
    fh.write(f"{indent}}}\n")


def _write_group_struct_decl(
    fh,
    group_name: str,
    group_payload: dict,
    field_type_map: Dict[str, str],
    all_components_map: dict,
    name_to_num: dict,
    indent: str = "    ",
):
    items = list(_iter_message_items(group_payload, all_components_map))
    field_items = []
    component_items = []
    subgroup_items = []
    ordered_items = []
    required_names = []
    bit_positions = {}

    fh.write(f"{indent}struct {group_name} {{\n")

    bit_index = 0
    for tag, name, payload in items:
        if tag == "field":
            bit_positions[name] = bit_index
            fh.write(f"{indent}    static constexpr std::size_t P_{name:<28} = {bit_index};\n")
            fix_type = field_type_map.get(name, "STRING")
            cpp_type = resolve_cpp_type(name, fix_type)
            field_items.append((name, fix_type, cpp_type))
            if payload.get("required", "N") == "Y":
                required_names.append(name)
            bit_index += 1
            ordered_items.append((tag, name))
        elif tag == "component":
            component_items.append(name)
            ordered_items.append((tag, name))
        elif tag == "group":
            subgroup_items.append((name, payload))
            ordered_items.append((tag, name))

    nwords = _presence_nwords(bit_index)
    if nwords == 1:
        if required_names:
            if len(required_names) == 1:
                fh.write(f"\n{indent}    static constexpr uint64_t REQUIRED = (1ULL << {bit_positions[required_names[0]]});\n")
            else:
                expr = " |\n".join(
                    f"{indent}        (1ULL << {bit_positions[name]})" for name in required_names
                )
                fh.write(f"\n{indent}    static constexpr uint64_t REQUIRED =\n{expr};\n")
        else:
            fh.write(f"\n{indent}    static constexpr uint64_t REQUIRED = 0ULL;\n")
    else:
        fh.write(f"\n{indent}    static constexpr std::size_t NWORDS = {nwords};\n")
        fh.write(f"{indent}    static constexpr std::array<uint64_t, NWORDS> REQUIRED{{{{\n")
        fh.write(_required_words_expr(required_names, bit_positions, nwords, indent + "        "))
        fh.write(f"\n{indent}    }}}};\n")
        fh.write(f"{indent}    static constexpr std::size_t _presence_word(std::size_t bit) noexcept {{ return bit / 64; }}\n")
        fh.write(f"{indent}    static constexpr uint64_t _presence_mask(std::size_t bit) noexcept {{ return 1ULL << (bit % 64); }}\n")

    max_encode = 0
    for name, fix_type, _cpp_type in field_items:
        tag_num = name_to_num.get(name, 0)
        if tag_num:
            max_encode += len(str(tag_num)) + 1 + max_value_encode_bytes(fix_type, name) + 1
    fh.write(f"{indent}    static constexpr std::size_t MAX_ENCODE_SIZE = {max_encode if max_encode > 0 else 32};\n\n")

    for sub_name, sub_payload in subgroup_items:
        _write_group_struct_decl(
            fh,
            sub_name,
            sub_payload,
            field_type_map,
            all_components_map,
            name_to_num,
            indent=indent + "    ",
        )
        fh.write("\n")

    field_meta = {name: (fix_type, cpp_type) for name, fix_type, cpp_type in field_items}
    for tag, name in ordered_items:
        if tag == "field":
            fix_type, cpp_type = field_meta[name]
            fh.write(f"{indent}    types::{cpp_type} m_{name}{{{_field_init(name, fix_type)}}};\n")
        elif tag == "component":
            fh.write(f"{indent}    components::{name} m_{name}{{}};\n")
        elif tag == "group":
            fh.write(f"{indent}    std::vector<{name}> m_{name}Group{{}};\n")

    if nwords == 1:
        fh.write(f"\n{indent}    uint64_t m_presence{{0}};\n\n")
    else:
        fh.write(f"\n{indent}    std::array<uint64_t, NWORDS> m_presence{{}};\n\n")
    fh.write(f"{indent}public:\n")
    fh.write(f"{indent}    // lifecycle\n")
    fh.write(f"{indent}    void reset() noexcept;\n\n")
    fh.write(f"{indent}    // validation\n")
    fh.write(f"{indent}    bool checkRequired() const noexcept;\n\n")
    fh.write(f"{indent}    // decode\n")
    fh.write(f"{indent}    bool decode(const char *&p, const char *end) noexcept;\n\n")
    fh.write(f"{indent}    // encode\n")
    fh.write(f"{indent}    char *encode(char *p, bool includeMilliseconds = true) const noexcept;\n")
    fh.write(f"{indent}    bool setField(int32_t tag, const char *begin, const char *end) noexcept;\n")
    if component_items or subgroup_items:
        fh.write(f"{indent}    std::size_t compute_buffer_size() const noexcept {{\n")
        fh.write(f"{indent}        return MAX_ENCODE_SIZE")
        for name in component_items:
            fh.write(f"\n{indent}            + m_{name}.compute_buffer_size()")
        for name, _payload in subgroup_items:
            fh.write(
                f"\n{indent}            + ([&]() noexcept {{ "
                f"std::size_t total = 0; "
                f"for (const auto &item : m_{name}Group) {{ total += item.compute_buffer_size(); }} "
                f"return total; "
                f"}}())"
            )
        fh.write(";\n")
        fh.write(f"{indent}    }}\n\n")
    else:
        fh.write(f"{indent}    static constexpr std::size_t compute_buffer_size() noexcept {{ return MAX_ENCODE_SIZE; }}\n\n")

    fh.write(f"{indent}    // accessors\n")
    for tag, name in ordered_items:
        if tag == "field":
            _fix_type, cpp_type = field_meta[name]
            fh.write(f"{indent}    const types::{cpp_type} &get{name}() const noexcept {{ return m_{name}; }}\n")
        elif tag == "component":
            fh.write(f"{indent}    components::{name} &getRef{name}() noexcept {{ return m_{name}; }}\n")
            fh.write(f"{indent}    const components::{name} &getRef{name}() const noexcept {{ return m_{name}; }}\n")
        elif tag == "group":
            fh.write(f"{indent}    std::vector<{name}> &getRef{name}Group() noexcept {{ return m_{name}Group; }}\n")
            fh.write(f"{indent}    const std::vector<{name}> &getRef{name}Group() const noexcept {{ return m_{name}Group; }}\n")

    fh.write(f"\n{indent}    // mutators\n")
    for tag, name in ordered_items:
        if tag == "field":
            fix_type, cpp_type = field_meta[name]
            if is_string_like(fix_type):
                if nwords == 1:
                    fh.write(f"{indent}    void set{name}(const types::{cpp_type} &v) noexcept {{ m_{name} = v; m_presence |= (1ULL << P_{name}); }}\n")
                else:
                    fh.write(f"{indent}    void set{name}(const types::{cpp_type} &v) noexcept {{ m_{name} = v; m_presence[_presence_word(P_{name})] |= _presence_mask(P_{name}); }}\n")
            else:
                if nwords == 1:
                    fh.write(f"{indent}    void set{name}(types::{cpp_type} v) noexcept {{ m_{name} = v; m_presence |= (1ULL << P_{name}); }}\n")
                else:
                    fh.write(f"{indent}    void set{name}(types::{cpp_type} v) noexcept {{ m_{name} = v; m_presence[_presence_word(P_{name})] |= _presence_mask(P_{name}); }}\n")
        elif tag == "group":
            fh.write(f"{indent}    void add{name}Group(const {name} &item) {{ m_{name}Group.push_back(item); }}\n")
            fh.write(f"{indent}    void add{name}Group({name} &&item) noexcept {{ m_{name}Group.push_back(std::move(item)); }}\n")

    fh.write(f"\n{indent}    // presence\n")
    _write_has_any_set(fh, f"{indent}    ", nwords, component_items, [name for name, _payload in subgroup_items])
    for tag, name in ordered_items:
        if tag == "field":
            if nwords == 1:
                fh.write(f"{indent}    bool has{name}() const noexcept {{ return (m_presence & (1ULL << P_{name})) != 0; }}\n")
            else:
                fh.write(f"{indent}    bool has{name}() const noexcept {{ return (m_presence[_presence_word(P_{name})] & _presence_mask(P_{name})) != 0; }}\n")
        elif tag == "group":
            fh.write(f"{indent}    bool has{name}() const noexcept {{ return !m_{name}Group.empty(); }}\n")

    fh.write(f"{indent}}};\n")


def _write_message_header(
    out_path: Path,
    mname: str,
    msg: dict,
    ns: str,
    include_root: str,
    field_type_map: Dict[str, str],
    all_components_map: dict,
    name_to_num: dict,
    is_header_or_trailer: bool,
):
    with open_generated_file(out_path, encoding="utf-8") as fh:
        title = mname if is_header_or_trailer else f"Message {mname}"
        write_autogen_header(fh, title)
        fh.write("#pragma once\n\n")
        fh.write("#include <array>\n")
        fh.write("#include <cstdint>\n")
        if not is_header_or_trailer:
            fh.write("#include <string_view>\n")
            fh.write("#include <utils/fast_buffer.h>\n")
            fh.write("#include <utils/error_codes.h>\n")
        if any(tag == "group" for tag, _n, _p in _iter_message_items(msg, all_components_map)):
            fh.write("#include <utility>\n")
            fh.write("#include <vector>\n")
        fh.write(f"#include <{include_root}/core/types.h>\n")
        fh.write(f"#include <{include_root}/core/null_fields.h>\n")
        fh.write(f"#include <{include_root}/core/field_id.h>\n")

        if not is_header_or_trailer:
            fh.write(f"#include <{include_root}/engine/protocol.h>\n")
            fh.write(f"#include <{include_root}/messages/Header.h>\n")
            fh.write(f"#include <{include_root}/messages/Trailer.h>\n")

        component_deps = _collect_component_deps(msg, all_components_map)
        if component_deps:
            fh.write("\n")
            for dep in sorted(component_deps):
                fh.write(f"#include <{include_root}/components/{dep}.h>\n")

        fh.write(f"\nnamespace {ns}::messages {{\n\n")
        fh.write(f"class {mname} {{\n")

        field_items = []
        component_items = []
        group_items = []
        ordered_items = []
        required_names = []
        bit_index = 0
        bit_positions = {}

        for tag, name, payload in _iter_message_items(msg, all_components_map):
            if tag == "field":
                bit_positions[name] = bit_index
                fh.write(f"    static constexpr std::size_t P_{name:<28} = {bit_index};\n")
                fix_type = field_type_map.get(name, "STRING")
                cpp_type = resolve_cpp_type(name, fix_type)
                field_items.append((name, fix_type, cpp_type))
                if payload.get("required", "N") == "Y":
                    required_names.append(name)
                bit_index += 1
                ordered_items.append((tag, name))
            elif tag == "component":
                component_items.append(name)
                ordered_items.append((tag, name))
            elif tag == "group":
                group_items.append((name, payload))
                ordered_items.append((tag, name))

        if group_items:
            fh.write("\n")
            for name, payload in group_items:
                _write_group_struct_decl(
                    fh,
                    name,
                    payload,
                    field_type_map,
                    all_components_map,
                    name_to_num,
                    indent="    ",
                )
                fh.write("\n")

        nwords = _presence_nwords(bit_index)
        if nwords == 1:
            if required_names:
                if len(required_names) == 1:
                    fh.write(f"\n    static constexpr uint64_t REQUIRED = (1ULL << {bit_positions[required_names[0]]});\n")
                else:
                    expr = " |\n".join(
                        f"        (1ULL << {bit_positions[name]})" for name in required_names
                    )
                    fh.write(f"\n    static constexpr uint64_t REQUIRED =\n{expr};\n")
            else:
                fh.write("\n    static constexpr uint64_t REQUIRED = 0ULL;\n")
        else:
            fh.write(f"\n    static constexpr std::size_t NWORDS = {nwords};\n")
            fh.write("    static constexpr std::array<uint64_t, NWORDS> REQUIRED{{\n")
            fh.write(_required_words_expr(required_names, bit_positions, nwords, "        "))
            fh.write("\n    }};\n")
            fh.write("    static constexpr std::size_t _presence_word(std::size_t bit) noexcept { return bit / 64; }\n")
            fh.write("    static constexpr uint64_t _presence_mask(std::size_t bit) noexcept { return 1ULL << (bit % 64); }\n")

        max_encode = 0
        for name, fix_type, _cpp_type in field_items:
            tag_num = name_to_num.get(name, 0)
            if tag_num:
                max_encode += len(str(tag_num)) + 1 + max_value_encode_bytes(fix_type, name) + 1
        fh.write(f"    static constexpr std::size_t MAX_ENCODE_SIZE = {max_encode if max_encode > 0 else 32};\n\n")

        if not is_header_or_trailer:
            fh.write("    Header m_Header{};\n")
            fh.write("    Trailer m_Trailer{};\n\n")

        field_meta = {name: (fix_type, cpp_type) for name, fix_type, cpp_type in field_items}
        for tag, name in ordered_items:
            if tag == "field":
                fix_type, cpp_type = field_meta[name]
                fh.write(f"    types::{cpp_type} m_{name}{{{_field_init(name, fix_type)}}};\n")
            elif tag == "component":
                fh.write(f"    components::{name} m_{name}{{}};\n")
            elif tag == "group":
                fh.write(f"    std::vector<{name}> m_{name}Group{{}};\n")

        if nwords == 1:
            fh.write("\n    uint64_t m_presence{0};\n\n")
        else:
            fh.write("\n    std::array<uint64_t, NWORDS> m_presence{};\n\n")
        fh.write("public:\n")

        if not is_header_or_trailer:
            msgtype = msg.get("msgtype", "")
            is_admin = (msg.get("msgcat", "app") == "admin")
            if msg.get("name") == "BusinessMessageReject":
                is_admin = True
            fh.write(f"    using Protocol = {ns}::Protocol;\n")
            fh.write("    static constexpr falconfix::ProtocolVersion PROTOCOL = Protocol::VERSION;\n")
            fh.write(f"    static constexpr std::string_view MSGTYPE = \"{msgtype}\";\n")
            fh.write(f"    static constexpr bool IS_ADMIN = {'true' if is_admin else 'false'};\n\n")

        fh.write("    // lifecycle\n")
        fh.write("    void reset() noexcept;\n\n")
        fh.write("    // validation\n")
        fh.write("    bool checkRequired() const noexcept;\n\n")

        fh.write("    // decode\n")
        if is_header_or_trailer:
            fh.write("    bool decode(const char *&p, const char *end) noexcept;\n\n")
        else:
            fh.write("    FFStatus setString(const char *&p, const char *end) noexcept;\n")
            fh.write("    FFStatus setString(std::string_view sv) noexcept {\n")
            fh.write("        const char *p = sv.data();\n")
            fh.write("        const char *end = p + sv.size();\n")
            fh.write("        return setString(p, end);\n")
            fh.write("    }\n\n")

        fh.write("    // encode\n")
        if is_header_or_trailer:
            fh.write("    char *encode(char *p, bool includeMilliseconds = true) const noexcept;\n")
        else:
            fh.write("    FFStatus toString(falconfix::FastBuffer &out, bool includeMilliseconds = true) const noexcept;\n")
        fh.write("    bool setField(int32_t tag, const char *begin, const char *end) noexcept;\n")
        if not is_header_or_trailer:
            fh.write("    bool setField(int32_t tag, std::string_view value) noexcept { return setField(tag, value.data(), value.data() + value.size()); }\n")
        if is_header_or_trailer:
            fh.write("    std::size_t compute_buffer_size() const noexcept {\n")
            fh.write("        std::size_t total = MAX_ENCODE_SIZE")
            if not group_items:
                fh.write(";\n")
            else:
                fh.write(";\n")
            for name, _payload in group_items:
                fh.write(f"        for (const auto &entry : m_{name}Group) {{ total += entry.compute_buffer_size(); }}\n")
            fh.write("        return total;\n")
            fh.write("    }\n\n")
        else:
            fh.write("    std::size_t compute_buffer_size() const noexcept {\n")
            fh.write("        std::size_t total = MAX_ENCODE_SIZE\n")
            fh.write("            + m_Header.compute_buffer_size()\n")
            fh.write("            + m_Trailer.compute_buffer_size();\n")
            for name, _payload in group_items:
                fh.write(f"        for (const auto &entry : m_{name}Group) {{ total += entry.compute_buffer_size(); }}\n")
            for name in component_items:
                fh.write(f"        total += m_{name}.compute_buffer_size();\n")
            fh.write("        return total;\n")
            fh.write("    }\n\n")

        fh.write("    // accessors\n")
        if not is_header_or_trailer:
            fh.write("    Header &getRefHeader() noexcept { return m_Header; }\n")
            fh.write("    const Header &getRefHeader() const noexcept { return m_Header; }\n")
            fh.write("    Trailer &getRefTrailer() noexcept { return m_Trailer; }\n")
            fh.write("    const Trailer &getRefTrailer() const noexcept { return m_Trailer; }\n\n")

        for tag, name in ordered_items:
            if tag == "field":
                _fix_type, cpp_type = field_meta[name]
                fh.write(f"    const types::{cpp_type} &get{name}() const noexcept {{ return m_{name}; }}\n")
            elif tag == "component":
                fh.write(f"    components::{name} &getRef{name}() noexcept {{ return m_{name}; }}\n")
                fh.write(f"    const components::{name} &getRef{name}() const noexcept {{ return m_{name}; }}\n")
            elif tag == "group":
                fh.write(f"    std::vector<{name}> &getRef{name}Group() noexcept {{ return m_{name}Group; }}\n")
                fh.write(f"    const std::vector<{name}> &getRef{name}Group() const noexcept {{ return m_{name}Group; }}\n")

        fh.write("\n    // mutators\n")
        for tag, name in ordered_items:
            if tag == "field":
                fix_type, cpp_type = field_meta[name]
                if is_string_like(fix_type):
                    if nwords == 1:
                        fh.write(f"    void set{name}(const types::{cpp_type} &v) noexcept {{ m_{name} = v; m_presence |= (1ULL << P_{name}); }}\n")
                    else:
                        fh.write(f"    void set{name}(const types::{cpp_type} &v) noexcept {{ m_{name} = v; m_presence[_presence_word(P_{name})] |= _presence_mask(P_{name}); }}\n")
                else:
                    if nwords == 1:
                        fh.write(f"    void set{name}(types::{cpp_type} v) noexcept {{ m_{name} = v; m_presence |= (1ULL << P_{name}); }}\n")
                    else:
                        fh.write(f"    void set{name}(types::{cpp_type} v) noexcept {{ m_{name} = v; m_presence[_presence_word(P_{name})] |= _presence_mask(P_{name}); }}\n")
            elif tag == "group":
                fh.write(f"    void add{name}Group(const {name} &item) {{ m_{name}Group.push_back(item); }}\n")
                fh.write(f"    void add{name}Group({name} &&item) noexcept {{ m_{name}Group.push_back(std::move(item)); }}\n")

        fh.write("\n    // presence\n")
        _write_has_any_set(fh, "    ", nwords, component_items, [name for name, _payload in group_items])
        for tag, name in ordered_items:
            if tag == "field":
                if nwords == 1:
                    fh.write(f"    bool has{name}() const noexcept {{ return (m_presence & (1ULL << P_{name})) != 0; }}\n")
                else:
                    fh.write(f"    bool has{name}() const noexcept {{ return (m_presence[_presence_word(P_{name})] & _presence_mask(P_{name})) != 0; }}\n")
            elif tag == "group":
                fh.write(f"    bool has{name}() const noexcept {{ return !m_{name}Group.empty(); }}\n")

        fh.write("};\n\n")
        fh.write(f"}} // namespace {ns}::messages\n")


def generate(messages_dir: Path, spec: dict):
    """
    Generate message headers.
    """
    messages_dir.mkdir(parents=True, exist_ok=True)

    ns, include_root, field_type_map, name_to_num, all_components_map = _build_context(spec)

    header = spec.get("header")
    trailer = spec.get("trailer")

    if header:
        _write_message_header(
            messages_dir / "Header.h",
            "Header",
            header,
            ns,
            include_root,
            field_type_map,
            all_components_map,
            name_to_num,
            is_header_or_trailer=True,
        )
        print("[ok] Generated Header.h")

    if trailer:
        _write_message_header(
            messages_dir / "Trailer.h",
            "Trailer",
            trailer,
            ns,
            include_root,
            field_type_map,
            all_components_map,
            name_to_num,
            is_header_or_trailer=True,
        )
        print("[ok] Generated Trailer.h")

    for msg in spec.get("messages", []):
        mname = normalize_name(msg.get("name") or "")
        if not mname:
            continue

        _write_message_header(
            messages_dir / f"{mname}.h",
            mname,
            msg,
            ns,
            include_root,
            field_type_map,
            all_components_map,
            name_to_num,
            is_header_or_trailer=False,
        )
        print(f"[ok] Generated {messages_dir / f'{mname}.h'}")
