# -*- coding: utf-8 -*-

# SPDX-License-Identifier: MIT
# Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo

"""
generator_components.py
-------------------------------------------------------
Component header generator.

Design goals:
- XML-driven generation (no fallback copy path)
- Shared item iterator (_iter_component_items) compatible with generator_components_cpp
- Handles two component shapes:
    * Plain components: fields and/or sub-components, no groups
    * Repeating-group components: wraps exactly one (or more) groups, no direct fields
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
    classify_fix_type,
    max_value_encode_bytes,
    TEMPORAL_TIMESTAMP_TYPES,
    TEMPORAL_DATE_TYPES,
    TEMPORAL_TIME_TYPES,
)


def _iter_component_items(obj: dict, all_components_map: dict):
    """
    Iterate fields/groups/components preserving XML declaration order.
    Exported for use by generator_components_cpp if needed.
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
                payload = comps_by.get(name) or all_components_map.get(name)
                if payload:
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


def _required_mask_expr(required_names: list) -> str:
    if not required_names:
        return "0ULL"
    if len(required_names) == 1:
        return f"P_{required_names[0]}"
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


def _compute_fields_max_encode(field_items: list, name_to_num: dict) -> int:
    total = 0
    for name, fix_type, _cpp_type in field_items:
        tag_num = name_to_num.get(name, 0)
        if tag_num:
            total += len(str(tag_num)) + 1 + max_value_encode_bytes(fix_type, name) + 1
    return total if total > 0 else 32


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


def _collect_component_deps(items: list) -> list:
    """Collect unique sub-component names from items list, in order."""
    seen = set()
    deps = []
    for tag, name, _payload in items:
        if tag == "component" and name not in seen:
            seen.add(name)
            deps.append(name)
    return deps


def _write_group_struct(fh, cname: str, gname: str, grp: dict,
                        ns: str, include_root: str,
                        field_type_map: Dict[str, str],
                        all_components_map: dict,
                        name_to_num: dict,
                        indent: str = "    "):
    """Write the nested struct body for a repeating group entry."""
    items = list(_iter_component_items(grp, all_components_map))

    field_items = []
    component_items = []
    required_names = []
    bit_index = 0
    bit_positions = {}

    for tag, name, payload in items:
        if tag == "field":
            fix_type = field_type_map.get(name, "STRING")
            cpp_type = resolve_cpp_type(name, fix_type)
            field_items.append((name, fix_type, cpp_type))
            if payload.get("required", "N") == "Y":
                required_names.append(name)
            bit_positions[name] = bit_index
            fh.write(f"{indent}static constexpr std::size_t P_{name:<28} = {bit_index};\n")
            bit_index += 1
        elif tag == "component":
            component_items.append(name)

    nwords = _presence_nwords(bit_index)
    if nwords == 1:
        if required_names:
            if len(required_names) == 1:
                fh.write(f"\n{indent}static constexpr uint64_t REQUIRED = (1ULL << {bit_positions[required_names[0]]});\n")
            else:
                expr = " |\n".join(
                    f"{indent}    (1ULL << {bit_positions[name]})" for name in required_names
                )
                fh.write(f"\n{indent}static constexpr uint64_t REQUIRED =\n{expr};\n")
        else:
            fh.write(f"\n{indent}static constexpr uint64_t REQUIRED = 0ULL;\n")
    else:
        fh.write(f"\n{indent}static constexpr std::size_t NWORDS = {nwords};\n")
        fh.write(f"{indent}static constexpr std::array<uint64_t, NWORDS> REQUIRED{{{{\n")
        fh.write(_required_words_expr(required_names, bit_positions, nwords, indent + "    "))
        fh.write(f"\n{indent}}}}};\n")
        fh.write(f"{indent}static constexpr std::size_t _presence_word(std::size_t bit) noexcept {{ return bit / 64; }}\n")
        fh.write(f"{indent}static constexpr uint64_t _presence_mask(std::size_t bit) noexcept {{ return 1ULL << (bit % 64); }}\n")

    max_encode = _compute_fields_max_encode(field_items, name_to_num)
    fh.write(f"{indent}static constexpr std::size_t MAX_ENCODE_SIZE = {max_encode};\n\n")

    # Member declarations in raw_children order
    for tag, name, payload in items:
        if tag == "field":
            fix_type = field_type_map.get(name, "STRING")
            cpp_type = resolve_cpp_type(name, fix_type)
            init = _field_init(name, fix_type)
            fh.write(f"{indent}types::{cpp_type} m_{name}{{{init}}};\n")
        elif tag == "component":
            fh.write(f"{indent}{name} m_{name}{{}};\n")

    if nwords == 1:
        fh.write(f"\n{indent}uint64_t m_presence{{0}};\n\n")
    else:
        fh.write(f"\n{indent}std::array<uint64_t, NWORDS> m_presence{{}};\n\n")
    fh.write(f"{indent[:-4]}public:\n")

    fh.write(f"{indent}// lifecycle\n")
    fh.write(f"{indent}void reset() noexcept;\n\n")
    fh.write(f"{indent}// validation\n")
    fh.write(f"{indent}bool checkRequired() const noexcept;\n\n")
    fh.write(f"{indent}// decode\n")
    fh.write(f"{indent}bool decode(const char *&p, const char *end) noexcept;\n")
    fh.write(f"{indent}bool decode(const char *&p, const char *end, int32_t stop_tag, bool stop_on_repetition) noexcept;\n\n")
    fh.write(f"{indent}// encode\n")
    fh.write(f"{indent}char *encode(char *p, bool includeMilliseconds = true) const noexcept;\n")
    fh.write(f"{indent}bool setField(int32_t tag, const char *begin, const char *end) noexcept;\n")
    fh.write(f"{indent}inline bool setField(int32_t tag, std::string_view value) noexcept {{ return setField(tag, value.data(), value.data() + value.size()); }}\n")
    if component_items:
        fh.write(f"{indent}std::size_t compute_buffer_size() const noexcept {{\n")
        fh.write(f"{indent}    return MAX_ENCODE_SIZE\n")
        for idx, name in enumerate(component_items):
            suffix = ";" if idx == len(component_items) - 1 else ""
            fh.write(f"{indent}        + m_{name}.compute_buffer_size(){suffix}\n")
        fh.write(f"{indent}}}\n\n")
    else:
        fh.write(f"{indent}static constexpr std::size_t compute_buffer_size() noexcept {{ return MAX_ENCODE_SIZE; }}\n\n")

    fh.write(f"{indent}// accessors\n")
    for name in component_items:
        fh.write(f"{indent}{name} &getRef{name}() noexcept {{ return m_{name}; }}\n")
        fh.write(f"{indent}const {name} &getRef{name}() const noexcept {{ return m_{name}; }}\n")
    for name, fix_type, cpp_type in field_items:
        fh.write(f"{indent}const types::{cpp_type} &get{name}() const noexcept {{ return m_{name}; }}\n")

    fh.write(f"\n{indent}// mutators\n")
    for name, fix_type, cpp_type in field_items:
        if is_string_like(fix_type):
            if nwords == 1:
                fh.write(f"{indent}void set{name}(const types::{cpp_type} &v) noexcept {{ m_{name} = v; m_presence |= (1ULL << P_{name}); }}\n")
            else:
                fh.write(f"{indent}void set{name}(const types::{cpp_type} &v) noexcept {{ m_{name} = v; m_presence[_presence_word(P_{name})] |= _presence_mask(P_{name}); }}\n")
        else:
            if nwords == 1:
                fh.write(f"{indent}void set{name}(types::{cpp_type} v) noexcept {{ m_{name} = v; m_presence |= (1ULL << P_{name}); }}\n")
            else:
                fh.write(f"{indent}void set{name}(types::{cpp_type} v) noexcept {{ m_{name} = v; m_presence[_presence_word(P_{name})] |= _presence_mask(P_{name}); }}\n")

    fh.write(f"\n{indent}bool has{gname[0].upper()}{gname[1:]}() const noexcept {{ return (m_presence & P_{list(field_items)[0][0] if field_items else gname}) != 0; }}\n" if False else "")
    fh.write(f"\n{indent}// presence\n")
    _write_has_any_set(fh, indent, nwords, component_items)
    for name, _fix_type, _cpp_type in field_items:
        if nwords == 1:
            fh.write(f"{indent}bool has{name}() const noexcept {{ return (m_presence & (1ULL << P_{name})) != 0; }}\n")
        else:
            fh.write(f"{indent}bool has{name}() const noexcept {{ return (m_presence[_presence_word(P_{name})] & _presence_mask(P_{name})) != 0; }}\n")


def _write_component_header(
    out_path: Path,
    cname: str,
    comp: dict,
    ns: str,
    include_root: str,
    field_type_map: Dict[str, str],
    all_components_map: dict,
    name_to_num: dict,
):
    items = list(_iter_component_items(comp, all_components_map))
    has_groups = any(t == "group" for t, _, _ in items)

    with open_generated_file(out_path, encoding="utf-8") as fh:
        write_autogen_header(fh, f"Component {cname}")
        fh.write("#pragma once\n\n")

        fh.write("#include <array>\n")
        if has_groups:
            fh.write("#include <cstddef>\n")
        fh.write("#include <cstdint>\n")
        fh.write("#include <string_view>\n")
        if has_groups:
            fh.write("#include <vector>\n")
        fh.write("\n")
        if not has_groups:
            fh.write("#include <utils/error_codes.h>\n")
        fh.write(f"#include <{include_root}/core/field_id.h>\n")
        fh.write(f"#include <{include_root}/core/null_fields.h>\n")
        fh.write(f"#include <{include_root}/core/types.h>\n")

        # Collect component deps: for plain components from top-level items,
        # for group components from all group items
        comp_deps = set()
        if has_groups:
            for tag, name, payload in items:
                if tag == "group":
                    for gtag, gname, _gpayload in _iter_component_items(payload, all_components_map):
                        if gtag == "component":
                            comp_deps.add(gname)
        else:
            for tag, name, _payload in items:
                if tag == "component":
                    comp_deps.add(name)

        if comp_deps:
            fh.write("\n")
            for dep in sorted(comp_deps):
                fh.write(f"#include <{include_root}/components/{dep}.h>\n")

        fh.write(f"\nnamespace {ns}::components {{\n\n")

        if has_groups:
            # -------------------------------------------------------
            # Repeating-group component: public struct(s), then outer
            # -------------------------------------------------------
            fh.write(f"class {cname} {{\n")
            fh.write("public:\n")

            group_items = [(name, payload) for tag, name, payload in items if tag == "group"]

            for gname, grp in group_items:
                fh.write(f"    struct {gname} {{\n")
                _write_group_struct(fh, cname, gname, grp, ns, include_root,
                                    field_type_map, all_components_map, name_to_num,
                                    indent="        ")
                fh.write("    };\n\n")

            fh.write("private:\n")
            bit_index = 0
            for gname, _grp in group_items:
                fh.write(f"    static constexpr std::size_t P_{gname:<28} = {bit_index};\n")
                bit_index += 1
            nwords = _presence_nwords(bit_index)

            outer_max_encode = 0
            for gname, _grp in group_items:
                tag_num = name_to_num.get(gname, 0)
                if tag_num:
                    outer_max_encode += len(str(tag_num)) + 1 + max_value_encode_bytes("INT", gname) + 1

            if nwords == 1:
                fh.write("    static constexpr uint64_t REQUIRED = 0ULL;\n")
            else:
                fh.write(f"    static constexpr std::size_t NWORDS = {nwords};\n")
                fh.write("    static constexpr std::array<uint64_t, NWORDS> REQUIRED{{0ULL}};\n")
            fh.write(f"    static constexpr std::size_t MAX_ENCODE_SIZE = {max(outer_max_encode, 10)};\n\n")

            for gname, _grp in group_items:
                fh.write(f"    std::vector<{gname}> m_{gname}Group{{}};\n")
            if nwords == 1:
                fh.write("    uint64_t m_presence{0};\n\n")
            else:
                fh.write("    std::array<uint64_t, NWORDS> m_presence{};\n\n")

            fh.write("public:\n")
            fh.write("    // lifecycle\n")
            fh.write("    void reset() noexcept;\n\n")
            fh.write("    // validation\n")
            fh.write("    bool checkRequired() const noexcept;\n\n")
            fh.write("    // decode\n")
            fh.write("    bool decode(const char *&p, const char *end) noexcept;\n")
            fh.write("    bool decode(const char *&p, const char *end, int32_t stop_tag, bool stop_on_repetition) noexcept;\n\n")
            fh.write("    // encode\n")
            fh.write("    char *encode(char *p, bool includeMilliseconds = true) const noexcept;\n")
            fh.write("    bool setField(int32_t tag, const char *begin, const char *end) noexcept;\n")
            fh.write("    inline bool setField(int32_t tag, std::string_view value) noexcept { return setField(tag, value.data(), value.data() + value.size()); }\n")
            fh.write("    std::size_t compute_buffer_size() const noexcept {\n")
            fh.write("        std::size_t total = MAX_ENCODE_SIZE;\n")
            for gname, _grp in group_items:
                fh.write(f"        for (const auto &entry : m_{gname}Group) {{ total += entry.compute_buffer_size(); }}\n")
            fh.write("        return total;\n")
            fh.write("    }\n\n")

            fh.write("    // accessors\n")
            for gname, _grp in group_items:
                fh.write(f"    std::vector<{gname}> &getRef{gname}Group() noexcept {{ return m_{gname}Group; }}\n")
                fh.write(f"    const std::vector<{gname}> &getRef{gname}Group() const noexcept {{ return m_{gname}Group; }}\n")
                fh.write(f"    std::size_t get{gname}() const noexcept {{ return m_{gname}Group.size(); }}\n")

            fh.write("\n    // mutators\n\n")

            fh.write("    // presence\n")
            _write_has_any_set(fh, "    ", nwords, [], [gname for gname, _grp in group_items])
            for gname, _grp in group_items:
                fh.write(f"    bool has{gname}() const noexcept {{ return !m_{gname}Group.empty(); }}\n")

        else:
            # -------------------------------------------------------
            # Plain/compound component: private-by-default layout
            # -------------------------------------------------------
            fh.write(f"class {cname} {{\n")

            field_items = []
            component_items = []
            required_names = []
            bit_index = 0
            bit_positions = {}

            for tag, name, payload in items:
                if tag == "field":
                    fix_type = field_type_map.get(name, "STRING")
                    cpp_type = resolve_cpp_type(name, fix_type)
                    field_items.append((name, fix_type, cpp_type))
                    bit_positions[name] = bit_index
                    fh.write(f"    static constexpr std::size_t P_{name:<28} = {bit_index};\n")
                    if payload.get("required", "N") == "Y":
                        required_names.append(name)
                    bit_index += 1
                elif tag == "component":
                    component_items.append(name)

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

            max_encode = _compute_fields_max_encode(field_items, name_to_num)
            fh.write(f"    static constexpr std::size_t MAX_ENCODE_SIZE = {max_encode};\n\n")

            # Member declarations in raw_children order
            for tag, name, payload in items:
                if tag == "field":
                    fix_type = field_type_map.get(name, "STRING")
                    cpp_type = resolve_cpp_type(name, fix_type)
                    init = _field_init(name, fix_type)
                    fh.write(f"    types::{cpp_type} m_{name}{{{init}}};\n")
                elif tag == "component":
                    fh.write(f"    {name} m_{name}{{}};\n")

            if nwords == 1:
                fh.write("\n    uint64_t m_presence{0};\n\n")
            else:
                fh.write("\n    std::array<uint64_t, NWORDS> m_presence{};\n\n")
            fh.write("public:\n")
            fh.write("    // lifecycle\n")
            fh.write("    void reset() noexcept;\n\n")
            fh.write("    // validation\n")
            fh.write("    bool checkRequired() const noexcept;\n\n")
            fh.write("    // decode\n")
            fh.write("    bool decode(const char *&p, const char *end) noexcept;\n")
            fh.write("    bool decode(const char *&p, const char *end, int32_t stop_tag, bool stop_on_repetition) noexcept;\n\n")
            fh.write("    // encode\n")
            fh.write("    char *encode(char *p, bool includeMilliseconds = true) const noexcept;\n")
            fh.write("    bool setField(int32_t tag, const char *begin, const char *end) noexcept;\n")
            fh.write("    inline bool setField(int32_t tag, std::string_view value) noexcept { return setField(tag, value.data(), value.data() + value.size()); }\n")

            # compute_buffer_size: static if no sub-components, dynamic otherwise
            if component_items:
                fh.write("    std::size_t compute_buffer_size() const noexcept {\n")
                fh.write("        return MAX_ENCODE_SIZE\n")
                for i, comp_name in enumerate(component_items):
                    if i < len(component_items) - 1:
                        fh.write(f"            + m_{comp_name}.compute_buffer_size()\n")
                    else:
                        fh.write(f"            + m_{comp_name}.compute_buffer_size();\n")
                fh.write("    }\n\n")
            else:
                fh.write("    static constexpr std::size_t compute_buffer_size() noexcept { return MAX_ENCODE_SIZE; }\n\n")

            # Accessors in raw_children order (interleaved components and fields)
            fh.write("    // accessors\n")
            for tag, name, _payload in items:
                if tag == "component":
                    fh.write(f"    {name} &getRef{name}() noexcept {{ return m_{name}; }}\n")
                    fh.write(f"    const {name} &getRef{name}() const noexcept {{ return m_{name}; }}\n")
                elif tag == "field":
                    fix_type = field_type_map.get(name, "STRING")
                    cpp_type = resolve_cpp_type(name, fix_type)
                    fh.write(f"    const types::{cpp_type} &get{name}() const noexcept {{ return m_{name}; }}\n")

            fh.write("\n    // mutators\n")
            for name, fix_type, cpp_type in field_items:
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

            fh.write("\n    // presence\n")
            _write_has_any_set(fh, "    ", nwords, component_items)
            for name, _fix_type, _cpp_type in field_items:
                if nwords == 1:
                    fh.write(f"    bool has{name}() const noexcept {{ return (m_presence & (1ULL << P_{name})) != 0; }}\n")
                else:
                    fh.write(f"    bool has{name}() const noexcept {{ return (m_presence[_presence_word(P_{name})] & _presence_mask(P_{name})) != 0; }}\n")

        fh.write("};\n\n")
        fh.write(f"}} // namespace {ns}::components\n")


def generate(components_dir: Path, spec: dict):
    """
    Generate components/*.h for each component defined in the FIX spec.
    """
    ns, include_root, field_type_map, name_to_num, all_components_map = _build_context(spec)
    components_dir.mkdir(parents=True, exist_ok=True)

    for comp in spec.get("components", []):
        cname = normalize_name(comp.get("name") or "")
        if not cname:
            continue
        out_path = components_dir / f"{cname}.h"
        _write_component_header(
            out_path, cname, comp,
            ns, include_root, field_type_map, all_components_map, name_to_num,
        )
