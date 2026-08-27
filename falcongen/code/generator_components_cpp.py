# -*- coding: utf-8 -*-

# SPDX-License-Identifier: MIT
# Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo

"""
generator_components_cpp.py
-------------------------------------------------------
Generate components/*.cpp implementations from XML spec.
"""

from pathlib import Path
from typing import Dict

from code.common import (
    open_generated_file,
    write_autogen_header,
    namespace_from_spec,
    normalize_version_string,
    normalize_name,
    classify_fix_type,
    is_string_like,
    TEMPORAL_TIMESTAMP_TYPES,
    TEMPORAL_DATE_TYPES,
    TEMPORAL_TIME_TYPES,
)


def _iter_component_items(obj: dict, all_components_map: dict):
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


def _field_init(name: str, fix_type: str) -> str:
    if is_string_like(fix_type):
        return f"fields::{name}_NULL"
    return f"fields::NULL_{fix_type}"


def _emit_decode_set_line(name: str, fix_type: str, ns: str) -> str:
    t = (fix_type or "STRING").upper()
    category = classify_fix_type(t)
    if category == "string":
        return f"set{name}({{begin, len}});"
    if t == "BOOLEAN":
        return f"set{name}({ns}::parser::parse_bool(begin, end));"
    if t == "CHAR":
        return f"set{name}({ns}::parser::parse_char(begin, end));"
    if t in TEMPORAL_TIMESTAMP_TYPES:
        return f"set{name}(falconfix::atoi(begin, end));"
    if t in TEMPORAL_DATE_TYPES:
        return f"set{name}({ns}::parser::parse_date32(begin, end));"
    if t in TEMPORAL_TIME_TYPES:
        return f"set{name}({ns}::parser::parse_timeonly32(begin, end));"
    if category == "float":
        return f"set{name}({ns}::parser::parse_double(begin, end));"
    return f"set{name}(falconfix::atoi(begin, end));"


def _emit_encode_field_line(name: str, fix_type: str, ns: str, name_to_num: dict) -> str:
    tag_num = name_to_num.get(name)
    if not tag_num:
        return ""
    write_macro = f"WRITE_TAG{len(str(tag_num)) + 1}({ns}::tag_const::{name});"

    t = (fix_type or "STRING").upper()
    category = classify_fix_type(t)

    if category == "string":
        value_expr = f"std::memcpy(p, m_{name}.data(), m_{name}.size()); p += m_{name}.size();"
    elif t == "BOOLEAN":
        value_expr = f"*p++ = m_{name} ? 'Y' : 'N';"
    elif t == "CHAR":
        value_expr = f"*p++ = m_{name};"
    elif t in TEMPORAL_DATE_TYPES:
        value_expr = f"p += {ns}::datetime::format_UTCDateOnly(m_{name}, p);"
    elif t in TEMPORAL_TIME_TYPES:
        value_expr = f"{ns}::serialize::write_utctimeonly(p, m_{name}, includeMilliseconds);"
    elif t in TEMPORAL_TIMESTAMP_TYPES:
        value_expr = f"p += {ns}::datetime::format_UTCTimestamp_compact(m_{name}, p, includeMilliseconds);"
    elif category == "float":
        value_expr = f"p += falconfix::dtoa(m_{name}, p);"
    else:
        value_expr = f"p += falconfix::itoa(m_{name}, p);"

    return f"{write_macro} {value_expr} *p++ = {ns}::serialize::SOH;"


def _first_trigger_field(obj: dict, all_components_map: dict) -> str | None:
    for tag, name, payload in _iter_component_items(obj, all_components_map):
        if tag in ("field", "group"):
            return name
        if tag == "component":
            nested = _first_trigger_field(payload, all_components_map)
            if nested:
                return nested
    return None


def _first_trigger_item(obj: dict, all_components_map: dict) -> tuple[str, str] | None:
    for tag, name, payload in _iter_component_items(obj, all_components_map):
        if tag in ("field", "group"):
            return tag, name
        if tag == "component":
            nested = _first_trigger_item(payload, all_components_map)
            if nested:
                return nested
    return None


def _collect_group_decode_routes(obj: dict, all_components_map: dict) -> list[tuple[str, list[str]]]:
    routes: list[tuple[str, list[str]]] = []
    seen: set[str] = set()
    for tag, name, payload in _iter_component_items(obj, all_components_map):
        if tag == "group":
            if name not in seen:
                seen.add(name)
                routes.append((name, []))
        elif tag == "component":
            for trigger, chain in _collect_group_decode_routes(payload, all_components_map):
                if trigger not in seen:
                    seen.add(trigger)
                    routes.append((trigger, [name] + chain))
    return routes


def _emit_setfield_switch(fh, scope: str, field_items: list, ns: str):
    fh.write(f"bool {scope}::setField(int32_t tag, const char *begin, const char *end) noexcept {{\n")
    fh.write("    const std::size_t len = static_cast<std::size_t>(end - begin);\n\n")
    fh.write("    switch (tag) {\n")
    for name, fix_type in field_items:
        fh.write(f"    case field_id::{name}: {_emit_decode_set_line(name, fix_type, ns)} return true;\n")
    fh.write("    default: return false;\n")
    fh.write("    }\n")
    fh.write("}\n\n")


def _emit_plain_impl(fh, cname: str, comp: dict, ns: str, field_type_map: Dict[str, str],
                     name_to_num: dict, all_components_map: dict):
    items = list(_iter_component_items(comp, all_components_map))
    ordered_items = [(t, name) for t, name, _ in items]
    field_items = [(name, field_type_map.get(name, "STRING")) for t, name, _ in items if t == "field"]
    component_items = [name for t, name, _ in items if t == "component"]
    field_type_by_name = {name: fix_type for name, fix_type in field_items}
    nwords = max(1, (len(field_items) + 63) // 64)

    decode_triggers = []
    for t, name, payload in items:
        if t == "component":
            trigger_item = _first_trigger_item(payload, all_components_map)
            if trigger_item:
                trigger_tag, trigger_name = trigger_item
                if trigger_tag == "field":
                    decode_triggers.append((name, trigger_name, []))
                else:
                    for nested_trigger, chain in _collect_group_decode_routes(payload, all_components_map):
                        decode_triggers.append((name, nested_trigger, chain))

    fh.write(f"void {cname}::reset() noexcept {{\n")
    for tag, name in ordered_items:
        if tag == "field":
            fh.write(f"    m_{name} = {_field_init(name, field_type_by_name[name])};\n")
        elif tag == "component":
            fh.write(f"    m_{name}.reset();\n")
    fh.write("    m_presence.fill(0);\n" if nwords > 1 else "    m_presence = 0;\n")
    fh.write("}\n\n")

    fh.write(f"bool {cname}::checkRequired() const noexcept {{\n")
    if nwords > 1:
        fh.write("    for (std::size_t i = 0; i < NWORDS; ++i) {\n")
        fh.write("        if ((m_presence[i] & REQUIRED[i]) != REQUIRED[i]) {\n")
        fh.write("            return false;\n")
        fh.write("        }\n")
        fh.write("    }\n")
    else:
        fh.write("    if ((m_presence & REQUIRED) != REQUIRED) {\n")
        fh.write("        return false;\n")
        fh.write("    }\n")
    component_checks = [f"(!m_{name}.hasAnySet() || m_{name}.checkRequired())" for tag, name in ordered_items if tag == "component"]
    if component_checks:
        fh.write("\n")
        fh.write("    return " + " &&\n".join(component_checks[:1]))
        for expr in component_checks[1:]:
            fh.write(" &&\n")
            fh.write(f"            {expr}")
        fh.write(";\n")
    else:
        fh.write("    return true;\n")
    fh.write("}\n\n")

    fh.write(f"bool {cname}::decode(const char *&p, const char *end) noexcept {{\n")
    fh.write("    return decode(p, end, 0, false);\n")
    fh.write("}\n\n")

    fh.write(f"bool {cname}::decode(const char *&p, const char *end, int32_t stop_tag, bool stop_on_repetition) noexcept {{\n")
    fh.write("    bool consumed = false;\n")
    fh.write("    while (p < end) {\n")
    fh.write("        const char *tag_start = p;\n")
    fh.write("        int32_t tag = 0;\n")
    fh.write("        while (p < end && *p != '=') {\n")
    fh.write("            const unsigned d = static_cast<unsigned>(*p - '0');\n")
    fh.write("            if (d > 9) { return false; }\n")
    fh.write("            tag = tag * 10 + static_cast<int32_t>(d);\n")
    fh.write("            ++p;\n")
    fh.write("        }\n")
    fh.write("        if (p >= end) { return false; }\n")
    fh.write("        if (stop_on_repetition && consumed && tag == stop_tag) {\n")
    fh.write("            p = tag_start;\n")
    fh.write("            break;\n")
    fh.write("        }\n")
    if decode_triggers:
        fh.write("        switch (tag) {\n")
        for comp_name, trigger, chain in decode_triggers:
            expr = f"m_{comp_name}" + "".join(f".getRef{part}()" for part in chain)
            fh.write(f"        case field_id::{trigger}:\n")
            fh.write("            p = tag_start;\n")
            fh.write(f"            if (!{expr}.decode(p, end, stop_tag, stop_on_repetition)) {{ return false; }}\n")
            fh.write("            consumed = true;\n")
            fh.write("            continue;\n")
        fh.write("        default:\n")
        fh.write("            break;\n")
        fh.write("        }\n")
    fh.write("        ++p;\n")
    fh.write("        const char *valueBegin = p;\n")
    fh.write(f"        while (p < end && *p != {ns}::serialize::SOH) {{ ++p; }}\n")
    fh.write("        if (p >= end) { return false; }\n")
    fh.write("        const char *valueEnd = p;\n")
    fh.write("        ++p;\n")
    fh.write("        if (!setField(tag, valueBegin, valueEnd)) {\n")
    fh.write("            p = tag_start;\n")
    fh.write("            break;\n")
    fh.write("        }\n")
    fh.write("        consumed = true;\n")
    fh.write("    }\n")
    fh.write("    return true;\n")
    fh.write("}\n\n")

    fh.write(f"char *{cname}::encode(char *p, bool includeMilliseconds) const noexcept {{\n")
    for tag, name in ordered_items:
        if tag == "field":
            line = _emit_encode_field_line(name, field_type_by_name[name], ns, name_to_num)
            if line:
                fh.write(f"    if (has{name}()) {{ {line} }}\n")
        elif tag == "component":
            fh.write(f"    if ((p = m_{name}.encode(p, includeMilliseconds)) == nullptr) {{ return nullptr; }}\n")
    fh.write("    return p;\n")
    fh.write("}\n\n")

    _emit_setfield_switch(fh, cname, field_items, ns)


def _emit_group_entry_impl(fh, scope: str, grp: dict, ns: str, field_type_map: Dict[str, str],
                           name_to_num: dict, all_components_map: dict):
    items = list(_iter_component_items(grp, all_components_map))
    ordered_items = [(t, name) for t, name, _ in items]
    field_items = [(name, field_type_map.get(name, "STRING")) for t, name, _ in items if t == "field"]
    component_items = [name for t, name, _ in items if t == "component"]
    field_type_by_name = {name: fix_type for name, fix_type in field_items}
    delimiter = _first_trigger_field(grp, all_components_map)
    nwords = max(1, (len(field_items) + 63) // 64)

    decode_triggers = []
    for t, name, payload in items:
        if t == "component":
            trigger_item = _first_trigger_item(payload, all_components_map)
            if trigger_item:
                trigger_tag, trigger_name = trigger_item
                if trigger_tag == "field":
                    decode_triggers.append((name, trigger_name, []))
                else:
                    decode_triggers.append((name, trigger_name, []))

    fh.write(f"void {scope}::reset() noexcept {{\n")
    for tag, name in ordered_items:
        if tag == "field":
            fh.write(f"    m_{name} = {_field_init(name, field_type_by_name[name])};\n")
        elif tag == "component":
            fh.write(f"    m_{name}.reset();\n")
    fh.write("    m_presence.fill(0);\n" if nwords > 1 else "    m_presence = 0;\n")
    fh.write("}\n\n")

    fh.write(f"bool {scope}::checkRequired() const noexcept {{\n")
    if nwords > 1:
        fh.write("    for (std::size_t i = 0; i < NWORDS; ++i) {\n")
        fh.write("        if ((m_presence[i] & REQUIRED[i]) != REQUIRED[i]) {\n")
        fh.write("            return false;\n")
        fh.write("        }\n")
        fh.write("    }\n")
    else:
        fh.write("    if ((m_presence & REQUIRED) != REQUIRED) {\n")
        fh.write("        return false;\n")
        fh.write("    }\n")
    component_checks = [f"(!m_{name}.hasAnySet() || m_{name}.checkRequired())" for tag, name in ordered_items if tag == "component"]
    if component_checks:
        fh.write("\n")
        fh.write("    return " + " &&\n".join(component_checks[:1]))
        for expr in component_checks[1:]:
            fh.write(" &&\n")
            fh.write(f"            {expr}")
        fh.write(";\n")
    else:
        fh.write("    return true;\n")
    fh.write("}\n\n")

    fh.write(f"bool {scope}::decode(const char *&p, const char *end) noexcept {{\n")
    fh.write("    bool consumed = false;\n")
    fh.write("    while (p < end) {\n")
    fh.write("        const char *tag_start = p;\n")
    fh.write("        int32_t tag = 0;\n")
    fh.write("        while (p < end && *p != '=') {\n")
    fh.write("            const unsigned d = static_cast<unsigned>(*p - '0');\n")
    fh.write("            if (d > 9) { return false; }\n")
    fh.write("            tag = tag * 10 + static_cast<int32_t>(d);\n")
    fh.write("            ++p;\n")
    fh.write("        }\n")
    fh.write("        if (p >= end) { return false; }\n")
    if delimiter:
        fh.write(f"        if (consumed && tag == field_id::{delimiter}) {{\n")
        fh.write("            p = tag_start;\n")
        fh.write("            break;\n")
        fh.write("        }\n")
    if decode_triggers:
        fh.write("        switch (tag) {\n")
        for comp_name, trigger, chain in decode_triggers:
            expr = f"m_{comp_name}" + "".join(f".getRef{part}()" for part in chain)
            fh.write(f"        case field_id::{trigger}:\n")
            fh.write("            p = tag_start;\n")
            if delimiter:
                fh.write(f"            if (!{expr}.decode(p, end, field_id::{delimiter}, true)) {{ return false; }}\n")
            else:
                fh.write(f"            if (!{expr}.decode(p, end)) {{ return false; }}\n")
            fh.write("            consumed = true;\n")
            fh.write("            continue;\n")
        fh.write("        default:\n")
        fh.write("            break;\n")
        fh.write("        }\n")
    fh.write("        ++p;\n")
    fh.write("        const char *valueBegin = p;\n")
    fh.write(f"        while (p < end && *p != {ns}::serialize::SOH) {{ ++p; }}\n")
    fh.write("        if (p >= end) { return false; }\n")
    fh.write("        const char *valueEnd = p;\n")
    fh.write("        ++p;\n")
    fh.write("        if (!setField(tag, valueBegin, valueEnd)) {\n")
    fh.write("            p = tag_start;\n")
    fh.write("            break;\n")
    fh.write("        }\n")
    fh.write("        consumed = true;\n")
    fh.write("    }\n")
    fh.write("    return consumed;\n")
    fh.write("}\n\n")

    fh.write(f"char *{scope}::encode(char *p, bool includeMilliseconds) const noexcept {{\n")
    for tag, name in ordered_items:
        if tag == "field":
            line = _emit_encode_field_line(name, field_type_by_name[name], ns, name_to_num)
            if line:
                fh.write(f"    if (has{name}()) {{ {line} }}\n")
        elif tag == "component":
            fh.write(f"    if ((p = m_{name}.encode(p, includeMilliseconds)) == nullptr) {{ return nullptr; }}\n")
    fh.write("    return p;\n")
    fh.write("}\n\n")

    _emit_setfield_switch(fh, scope, field_items, ns)


def _emit_group_component_impl(fh, cname: str, comp: dict, ns: str, field_type_map: Dict[str, str],
                               name_to_num: dict, all_components_map: dict):
    group_items = [(name, payload) for t, name, payload in _iter_component_items(comp, all_components_map) if t == "group"]
    nwords = max(1, (len(group_items) + 63) // 64)

    for gname, grp in group_items:
        scope = f"{cname}::{gname}"
        _emit_group_entry_impl(fh, scope, grp, ns, field_type_map, name_to_num, all_components_map)

    fh.write(f"void {cname}::reset() noexcept {{\n")
    for gname, _ in group_items:
        fh.write(f"    m_{gname}Group.clear();\n")
    fh.write("    m_presence.fill(0);\n" if nwords > 1 else "    m_presence = 0;\n")
    fh.write("}\n\n")

    fh.write(f"bool {cname}::checkRequired() const noexcept {{\n")
    if nwords > 1:
        fh.write("    for (std::size_t i = 0; i < NWORDS; ++i) {\n")
        fh.write("        if ((m_presence[i] & REQUIRED[i]) != REQUIRED[i]) {\n")
        fh.write("            return false;\n")
        fh.write("        }\n")
        fh.write("    }\n")
    else:
        fh.write("    if ((m_presence & REQUIRED) != REQUIRED) {\n")
        fh.write("        return false;\n")
        fh.write("    }\n")
    for gname, _ in group_items:
        fh.write(f"    for (const auto &entry : m_{gname}Group) {{\n")
        fh.write("        if (!entry.checkRequired()) {\n")
        fh.write("            return false;\n")
        fh.write("        }\n")
        fh.write("    }\n")
    fh.write("    return true;\n")
    fh.write("}\n\n")

    fh.write(f"bool {cname}::decode(const char *&p, const char *end) noexcept {{\n")
    fh.write("    return decode(p, end, 0, false);\n")
    fh.write("}\n\n")

    fh.write(f"bool {cname}::decode(const char *&p, const char *end, int32_t stop_tag, bool stop_on_repetition) noexcept {{\n")
    fh.write("    bool consumed = false;\n")
    fh.write("    while (p < end) {\n")
    fh.write("        const char *tag_start = p;\n")
    fh.write("        int32_t tag = 0;\n")
    fh.write("        while (p < end && *p != '=') {\n")
    fh.write("            const unsigned d = static_cast<unsigned>(*p - '0');\n")
    fh.write("            if (d > 9) { return false; }\n")
    fh.write("            tag = tag * 10 + static_cast<int32_t>(d);\n")
    fh.write("            ++p;\n")
    fh.write("        }\n")
    fh.write("        if (p >= end) { return false; }\n")
    fh.write("        if (stop_on_repetition && consumed && tag == stop_tag) {\n")
    fh.write("            p = tag_start;\n")
    fh.write("            break;\n")
    fh.write("        }\n")

    if group_items:
        fh.write("        switch (tag) {\n")
        for gname, _grp in group_items:
            fh.write(f"        case field_id::{gname}: {{\n")
            fh.write("            ++p;\n")
            fh.write("            const char *valueBegin = p;\n")
            fh.write(f"            while (p < end && *p != {ns}::serialize::SOH) {{ ++p; }}\n")
            fh.write("            if (p >= end) { return false; }\n")
            fh.write("            const char *valueEnd = p;\n")
            fh.write("            ++p;\n")
            fh.write("            const int64_t count_i64 = falconfix::atoi(valueBegin, valueEnd);\n")
            fh.write("            if (count_i64 < 0) { return false; }\n")
            fh.write("            const std::size_t count = static_cast<std::size_t>(count_i64);\n")
            fh.write(f"            m_{gname}Group.clear();\n")
            fh.write(f"            m_{gname}Group.reserve(count);\n")
            fh.write("            for (std::size_t i = 0; i < count; ++i) {\n")
            fh.write(f"                m_{gname}Group.emplace_back();\n")
            fh.write(f"                if (!m_{gname}Group.back().decode(p, end)) {{ return false; }}\n")
            fh.write("            }\n")
            fh.write("            consumed = true;\n")
            fh.write("            continue;\n")
            fh.write("        }\n")
        fh.write("        default:\n")
        fh.write("            p = tag_start;\n")
        fh.write("            return consumed;\n")
        fh.write("        }\n")

    fh.write("    }\n")
    fh.write("    return consumed;\n")
    fh.write("}\n\n")

    fh.write(f"char *{cname}::encode(char *p, bool includeMilliseconds) const noexcept {{\n")
    for gname, _grp in group_items:
        tag_num = name_to_num.get(gname)
        if tag_num:
            fh.write(f"    if (!m_{gname}Group.empty()) {{\n")
            fh.write(f"        WRITE_TAG{len(str(tag_num)) + 1}({ns}::tag_const::{gname});\n")
            fh.write(f"        p += falconfix::itoa(static_cast<int64_t>(m_{gname}Group.size()), p);\n")
            fh.write(f"        *p++ = {ns}::serialize::SOH;\n")
            fh.write(f"        for (const auto &entry : m_{gname}Group) {{\n")
            fh.write("            if ((p = entry.encode(p, includeMilliseconds)) == nullptr) { return nullptr; }\n")
            fh.write("        }\n")
            fh.write("    }\n")
    fh.write("    return p;\n")
    fh.write("}\n\n")

    fh.write(f"bool {cname}::setField(int32_t, const char *, const char *) noexcept {{\n")
    fh.write("    return false;\n")
    fh.write("}\n\n")


def _write_component_cpp(out_path: Path, cname: str, comp: dict, ns: str, include_root: str,
                         field_type_map: Dict[str, str], name_to_num: dict, all_components_map: dict):
    items = list(_iter_component_items(comp, all_components_map))
    has_groups = any(t == "group" for t, _, _ in items)

    with open_generated_file(out_path, encoding="utf-8") as fh:
        write_autogen_header(fh, f"Component {cname} implementation")
        fh.write(f"#include <{include_root}/components/{cname}.h>\n\n")
        fh.write("#include <cstring>\n\n")
        fh.write(f"#include <{include_root}/utils/macros.h>\n")
        fh.write(f"#include <{include_root}/utils/parser.h>\n")
        fh.write(f"#include <{include_root}/utils/serializer.h>\n")
        fh.write(f"#include <{include_root}/utils/tag_constants.h>\n")
        fh.write(f"#include <{include_root}/utils/datetime.h>\n")
        fh.write("#include <utils/string_utils.h>\n\n")
        fh.write(f"namespace {ns}::components {{\n\n")

        if has_groups:
            _emit_group_component_impl(fh, cname, comp, ns, field_type_map, name_to_num, all_components_map)
        else:
            _emit_plain_impl(fh, cname, comp, ns, field_type_map, name_to_num, all_components_map)

        fh.write(f"}} // namespace {ns}::components\n")


def generate(components_cpp_dir: Path, spec: dict):
    components_cpp_dir.mkdir(parents=True, exist_ok=True)
    ns, include_root, field_type_map, name_to_num, all_components_map = _build_context(spec)

    for comp in spec.get("components", []):
        cname = normalize_name(comp.get("name") or "")
        if not cname:
            continue
        _write_component_cpp(
            components_cpp_dir / f"{cname}.cpp",
            cname,
            comp,
            ns,
            include_root,
            field_type_map,
            name_to_num,
            all_components_map,
        )
