# -*- coding: utf-8 -*-

# SPDX-License-Identifier: MIT
# Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo

"""
generator_messages_cpp.py
-------------------------------------------------------
Generate messages/*.cpp implementations from XML spec.
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
from code.generator_messages import _iter_message_items


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


def _first_component_trigger(comp: dict, all_components_map: dict) -> str | None:
    for tag, name, payload in _iter_message_items(comp, all_components_map):
        if tag in ("field", "group"):
            return name
        if tag == "component":
            nested = _first_component_trigger(payload, all_components_map)
            if nested:
                return nested
    return None


def _first_component_trigger_item(comp: dict, all_components_map: dict) -> tuple[str, str] | None:
    for tag, name, payload in _iter_message_items(comp, all_components_map):
        if tag in ("field", "group"):
            return tag, name
        if tag == "component":
            nested = _first_component_trigger_item(payload, all_components_map)
            if nested:
                return nested
    return None


def _collect_scope_field_names(scope: dict, all_components_map: dict) -> set[str]:
    names: set[str] = set()
    for tag, name, payload in _iter_message_items(scope, all_components_map):
        if tag in ("field", "group"):
            names.add(name)
        elif tag == "component" and isinstance(payload, dict):
            names.update(_collect_scope_field_names(payload, all_components_map))
    return names


def _emit_group_cpp_impl(
    fh,
    owner_scope: str,
    group_name: str,
    group_payload: dict,
    ns: str,
    field_type_map: Dict[str, str],
    name_to_num: dict,
    all_components_map: dict,
):
    scope = f"{owner_scope}::{group_name}"
    items = list(_iter_message_items(group_payload, all_components_map))
    field_items = [(name, field_type_map.get(name, "STRING")) for tag, name, _ in items if tag == "field"]
    component_items = [(name, payload) for tag, name, payload in items if tag == "component"]
    subgroup_items = [(name, payload) for tag, name, payload in items if tag == "group"]
    nwords = max(1, (len(field_items) + 63) // 64)
    component_decode_routes = []
    delimiter = _first_component_trigger(group_payload, all_components_map)
    for name, payload in component_items:
        trigger_item = _first_component_trigger_item(payload, all_components_map)
        if trigger_item is None:
            continue
        trigger_tag, trigger_name = trigger_item
        if trigger_tag not in ("field", "group"):
            continue
        component_decode_routes.append((name, trigger_name))

    for sub_name, sub_payload in subgroup_items:
        _emit_group_cpp_impl(
            fh,
            scope,
            sub_name,
            sub_payload,
            ns,
            field_type_map,
            name_to_num,
            all_components_map,
        )

    fh.write(f"void {scope}::reset() noexcept {{\n")
    for name, fix_type in field_items:
        fh.write(f"    m_{name} = {_field_init(name, fix_type)};\n")
    for name, _payload in component_items:
        fh.write(f"    m_{name}.reset();\n")
    for name, _payload in subgroup_items:
        fh.write(f"    m_{name}Group.clear();\n")
    fh.write("    m_presence.fill(0);\n" if nwords > 1 else "    m_presence = 0;\n")
    fh.write("}\n\n")

    fh.write(f"bool {scope}::checkRequired() const noexcept {{\n")
    if not component_items and not subgroup_items:
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
        fh.write("    return true;\n")
    else:
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
        for name, _payload in component_items:
            fh.write(f"    if (m_{name}.hasAnySet() && !m_{name}.checkRequired()) {{\n")
            fh.write("        return false;\n")
            fh.write("    }\n")
        for name, _payload in subgroup_items:
            fh.write(f"    for (const auto &entry : m_{name}Group) {{\n")
            fh.write("        if (!entry.checkRequired()) {\n")
            fh.write("            return false;\n")
            fh.write("        }\n")
            fh.write("    }\n")
        fh.write("    return true;\n")
    fh.write("}\n\n")

    fh.write(f"bool {scope}::decode(const char *&p, const char *end) noexcept {{\n")
    fh.write("    bool consumed = false;\n")
    fh.write("    while (p < end) {\n")
    fh.write("        const char *tag_start = p;\n")
    fh.write("        int32_t tag = 0;\n\n")
    fh.write("        while (p < end && *p != '=') {\n")
    fh.write("            const unsigned d = static_cast<unsigned>(*p - '0');\n")
    fh.write("            if (d > 9) { return false; }\n")
    fh.write("            tag = tag * 10 + static_cast<int32_t>(d);\n")
    fh.write("            ++p;\n")
    fh.write("        }\n")
    fh.write("        if (p >= end) { return false; }\n\n")
    if delimiter:
        fh.write(f"        if (consumed && tag == field_id::{delimiter}) {{\n")
        fh.write("            p = tag_start;\n")
        fh.write("            break;\n")
        fh.write("        }\n")

    fh.write("        ++p;\n")
    fh.write("        const char *valueBegin = p;\n")
    fh.write(f"        while (p < end && *p != {ns}::serialize::SOH) {{ ++p; }}\n")
    fh.write("        const char *valueEnd = p;\n")
    fh.write("        if (p < end) { ++p; }\n\n")

    if subgroup_items:
        fh.write("        switch (tag) {\n")
        for name, _payload in subgroup_items:
            fh.write(f"        case field_id::{name}: {{\n")
            fh.write("            const std::size_t count = static_cast<std::size_t>(falconfix::atoi(valueBegin, valueEnd));\n")
            fh.write(f"            m_{name}Group.clear();\n")
            fh.write(f"            m_{name}Group.reserve(count);\n")
            fh.write("            for (std::size_t i = 0; i < count; ++i) {\n")
            fh.write(f"                m_{name}Group.emplace_back();\n")
            fh.write(f"                if (!m_{name}Group.back().decode(p, end)) {{ return false; }}\n")
            fh.write("            }\n")
            fh.write("            consumed = true;\n")
            fh.write("            continue;\n")
            fh.write("        }\n")
        fh.write("        default: break;\n")
        fh.write("        }\n")

    if component_decode_routes:
        fh.write("        switch (tag) {\n")
        for name, trigger in component_decode_routes:
            fh.write(f"        case field_id::{trigger}:\n")
            fh.write("            p = tag_start;\n")
            if delimiter:
                fh.write(f"            if (!m_{name}.decode(p, end, field_id::{delimiter}, true)) {{ return false; }}\n")
            else:
                fh.write(f"            if (!m_{name}.decode(p, end)) {{ return false; }}\n")
            fh.write("            consumed = true;\n")
            fh.write("            continue;\n")
        fh.write("        default: break;\n")
        fh.write("        }\n")

    fh.write("        if (!setField(tag, valueBegin, valueEnd)) {\n")
    fh.write("            p = tag_start;\n")
    fh.write("            return consumed;\n")
    fh.write("        }\n")
    fh.write("        consumed = true;\n")
    fh.write("    }\n\n")
    fh.write("    return consumed;\n")
    fh.write("}\n\n")

    fh.write(f"char *{scope}::encode(char *p, bool includeMilliseconds) const noexcept {{\n")
    for name, fix_type in field_items:
        line = _emit_encode_field_line(name, fix_type, ns, name_to_num)
        if line:
            fh.write(f"    if (has{name}()) {{ {line} }}\n")
    for name, _payload in component_items:
        fh.write(f"    p = m_{name}.encode(p, includeMilliseconds);\n")
    for name, _payload in subgroup_items:
        tag_num = name_to_num.get(name)
        if tag_num:
            fh.write(f"    if (!m_{name}Group.empty()) {{\n")
            fh.write(f"        WRITE_TAG{len(str(tag_num)) + 1}({ns}::tag_const::{name});\n")
            fh.write(f"        p += falconfix::itoa(static_cast<int64_t>(m_{name}Group.size()), p);\n")
            fh.write(f"        *p++ = {ns}::serialize::SOH;\n")
            fh.write(f"        for (const auto &entry : m_{name}Group) {{\n")
            fh.write("            p = entry.encode(p, includeMilliseconds);\n")
            fh.write("        }\n")
            fh.write("    }\n")
    fh.write("\n    return p;\n")
    fh.write("}\n\n")

    fh.write(f"bool {scope}::setField(int32_t tag, const char *begin, const char *end) noexcept {{\n")
    fh.write("    const std::size_t len = static_cast<std::size_t>(end - begin);\n\n")
    fh.write("    switch (tag) {\n")
    for name, fix_type in field_items:
        fh.write(f"    case field_id::{name}: {_emit_decode_set_line(name, fix_type, ns)} return true;\n")
    fh.write("    default: return false;\n")
    fh.write("    }\n")
    fh.write("}\n\n")


def _write_generic_message_cpp(
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
        write_autogen_header(fh, f"Message {mname} implementation")
        fh.write(f"#include <{include_root}/messages/{mname}.h>\n\n")
        fh.write("#include <cstring>\n\n")
        fh.write(f"#include <{include_root}/utils/macros.h>\n")
        fh.write(f"#include <{include_root}/utils/parser.h>\n")
        fh.write(f"#include <{include_root}/utils/serializer.h>\n")
        fh.write(f"#include <{include_root}/utils/tag_constants.h>\n")
        fh.write(f"#include <{include_root}/utils/datetime.h>\n")
        fh.write("#include <utils/string_utils.h>\n\n")
        fh.write(f"namespace {ns}::messages {{\n\n")

        items = list(_iter_message_items(msg, all_components_map))
        field_items = [(name, field_type_map.get(name, "STRING")) for tag, name, _p in items if tag == "field"]
        encode_field_items = field_items
        if is_header_or_trailer and mname == "Header":
            encode_field_items = [
                (name, fix_type)
                for name, fix_type in field_items
                if name not in {"BeginString", "BodyLength", "MsgType"}
            ]
        if is_header_or_trailer and mname == "Trailer":
            encode_field_items = [
                (name, fix_type)
                for name, fix_type in field_items
                if name != "CheckSum"
            ]
        group_items = [name for tag, name, _p in items if tag == "group"]
        group_payload_map = {name: payload for tag, name, payload in items if tag == "group"}
        component_items = [name for tag, name, _p in items if tag == "component"]
        component_payload_map = {name: payload for tag, name, payload in items if tag == "component"}
        ordered_items = [(tag, name) for tag, name, _p in items]
        field_type_by_name = {name: fix_type for name, fix_type in field_items}
        encode_field_type_by_name = {name: fix_type for name, fix_type in encode_field_items}
        nwords = max(1, (len(field_items) + 63) // 64)

        for gname in group_items:
            _emit_group_cpp_impl(
                fh,
                mname,
                gname,
                group_payload_map[gname],
                ns,
                field_type_map,
                name_to_num,
                all_components_map,
            )

        fh.write(f"void {mname}::reset() noexcept {{\n")
        if not is_header_or_trailer:
            fh.write("    m_Header.reset();\n")
            fh.write("    m_Trailer.reset();\n")
        for tag, name in ordered_items:
            if tag == "field":
                fh.write(f"    m_{name} = {_field_init(name, field_type_by_name[name])};\n")
            elif tag == "component":
                fh.write(f"    m_{name}.reset();\n")
            elif tag == "group":
                fh.write(f"    m_{name}Group.clear();\n")
        fh.write("    m_presence.fill(0);\n" if nwords > 1 else "    m_presence = 0;\n")
        fh.write("}\n\n")

        fh.write(f"bool {mname}::checkRequired() const noexcept {{\n")
        has_nested_checks = any(tag in ("component", "group") for tag, _name in ordered_items)
        if not has_nested_checks:
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
            fh.write("    return true;\n")
        else:
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
            for tag, name in ordered_items:
                if tag == "component":
                    fh.write(f"    if (m_{name}.hasAnySet() && !m_{name}.checkRequired()) {{\n")
                    fh.write("        return false;\n")
                    fh.write("    }\n")
                elif tag == "group":
                    fh.write(f"    for (const auto &entry : m_{name}Group) {{\n")
                    fh.write("        if (!entry.checkRequired()) {\n")
                    fh.write("            return false;\n")
                    fh.write("        }\n")
                    fh.write("    }\n")
            fh.write("    return true;\n")
        fh.write("}\n\n")

        decode_return_type = "bool" if is_header_or_trailer else "FFStatus"
        decode_method_name = "decode" if is_header_or_trailer else "setString"
        decode_ret_err = "return false;" if is_header_or_trailer else "return FF_FIX_PARSE_ERR(MALFORMED_TAG, 0);"
        decode_ret_ok = "return true;" if is_header_or_trailer else "return FF_OK();"

        fh.write(f"{decode_return_type} {mname}::{decode_method_name}(const char *&p, const char *end) noexcept {{\n")
        if not is_header_or_trailer:
            fh.write("    reset();\n\n")
        fh.write("    while (p < end) {\n")
        fh.write("        const char *tag_start = p;\n")
        fh.write("        int32_t tag = 0;\n\n")
        fh.write("        while (p < end && *p != '=') {\n")
        fh.write("            const unsigned d = static_cast<unsigned>(*p - '0');\n")
        fh.write("            if (d > 9) {\n")
        fh.write(f"                {decode_ret_err}\n")
        fh.write("            }\n")
        fh.write("            tag = tag * 10 + static_cast<int32_t>(d);\n")
        fh.write("            ++p;\n")
        fh.write("        }\n")
        fh.write("        if (p >= end) {\n")
        fh.write("            return false;\n" if is_header_or_trailer else "            return FF_FIX_PARSE_ERR(MISSING_EQUALS, 0);\n")
        fh.write("        }\n\n")

        if not is_header_or_trailer:
            fh.write("        switch (tag) {\n")
            fh.write("        case field_id::BeginString:\n")
            fh.write("            p = tag_start;\n")
            fh.write("            if (!m_Header.decode(p, end)) {\n")
            fh.write("                return FF_FIX_PARSE_ERR(DECODE_FAILED, field_id::BeginString);\n")
            fh.write("            }\n")
            fh.write("            continue;\n")
            fh.write("        case field_id::SignatureLength:\n")
            fh.write("        case field_id::Signature:\n")
            fh.write("        case field_id::CheckSum:\n")
            fh.write("            p = tag_start;\n")
            fh.write("            if (!m_Trailer.decode(p, end)) {\n")
            fh.write("                return FF_FIX_PARSE_ERR(DECODE_FAILED, field_id::SignatureLength);\n")
            fh.write("            }\n")
            fh.write("            continue;\n")
            for name in component_items:
                trigger_item = _first_component_trigger_item(component_payload_map.get(name, {}), all_components_map)
                if trigger_item is None:
                    continue
                trigger_tag, trigger_name = trigger_item
                if trigger_tag not in ("field", "group"):
                    continue
                fh.write(f"        case field_id::{trigger_name}:\n")
                fh.write("            p = tag_start;\n")
                fh.write(f"            if (!m_{name}.decode(p, end)) {{\n")
                fh.write(f"                return FF_FIX_PARSE_ERR(DECODE_FAILED, field_id::{trigger_name});\n")
                fh.write("            }\n")
                fh.write("            continue;\n")
            fh.write("        default:\n")
            fh.write("            break;\n")
            fh.write("        }\n\n")

        fh.write("        ++p;\n")
        fh.write("        const char *valueBegin = p;\n")
        fh.write(f"        while (p < end && *p != {ns}::serialize::SOH) {{\n")
        fh.write("            ++p;\n")
        fh.write("        }\n")
        fh.write("        const char *valueEnd = p;\n")
        fh.write("        if (p < end) {\n")
        fh.write("            ++p;\n")
        fh.write("        }\n")
        if not is_header_or_trailer:
            fh.write("        else {\n")
            fh.write("            return FF_FIX_PARSE_ERR(MISSING_SOH, tag);\n")
            fh.write("        }\n\n")

        if group_items:
            fh.write("        switch (tag) {\n")
            for name in group_items:
                fh.write(f"        case field_id::{name}: {{\n")
                fh.write("            const std::size_t count = static_cast<std::size_t>(falconfix::atoi(valueBegin, valueEnd));\n")
                fh.write(f"            m_{name}Group.clear();\n")
                fh.write(f"            m_{name}Group.reserve(count);\n")
                fh.write("            for (std::size_t i = 0; i < count; ++i) {\n")
                fh.write(f"                m_{name}Group.emplace_back();\n")
                fh.write(f"                if (!m_{name}Group.back().decode(p, end)) {{ return false; }}\n" if is_header_or_trailer else f"                if (!m_{name}Group.back().decode(p, end)) {{ return FF_FIX_PARSE_ERR(DECODE_FAILED, field_id::{name}); }}\n")
                fh.write("            }\n")
                fh.write("            continue;\n")
                fh.write("        }\n")
            fh.write("        default:\n")
            fh.write("            break;\n")
            fh.write("        }\n\n")

        fh.write("        if (!setField(tag, valueBegin, valueEnd)) {\n")
        fh.write("            p = tag_start;\n")
        if is_header_or_trailer:
            fh.write("            return true;\n")
        else:
            allowed_names = _collect_scope_field_names(msg, all_components_map)
            unexpected_names = [
                name for name in name_to_num
                if name not in allowed_names and name not in {"BeginString", "BodyLength", "CheckSum", "Signature", "SignatureLength"}
            ]
            fh.write("            switch (tag) {\n")
            for name in unexpected_names:
                fh.write(f"            case field_id::{name}: return FF_FIX_PARSE_ERR(UNEXPECTED_FIELD, tag);\n")
            fh.write("            default: return FF_FIX_PARSE_ERR(UNKNOWN_FIELD_ID, tag);\n")
            fh.write("            }\n")
        fh.write("        }\n")
        fh.write("    }\n\n")
        fh.write(f"    {decode_ret_ok}\n")
        fh.write("}\n\n")

        if is_header_or_trailer:
            fh.write(f"char *{mname}::encode(char *p, bool includeMilliseconds) const noexcept {{\n")
            for tag, name in ordered_items:
                if tag == "field" and name in encode_field_type_by_name:
                    line = _emit_encode_field_line(name, encode_field_type_by_name[name], ns, name_to_num)
                    if line:
                        fh.write(f"    if (has{name}()) {{ {line} }}\n")
                elif tag == "component":
                    fh.write(f"    p = m_{name}.encode(p, includeMilliseconds);\n")
                elif tag == "group":
                    tag_num = name_to_num.get(name)
                    if tag_num:
                        fh.write(f"    if (!m_{name}Group.empty()) {{\n")
                        fh.write(f"        WRITE_TAG{len(str(tag_num)) + 1}({ns}::tag_const::{name});\n")
                        fh.write(f"        p += falconfix::itoa(static_cast<int64_t>(m_{name}Group.size()), p);\n")
                        fh.write(f"        *p++ = {ns}::serialize::SOH;\n")
                        fh.write(f"        for (const auto &entry : m_{name}Group) {{\n")
                        fh.write("            p = entry.encode(p, includeMilliseconds);\n")
                        fh.write("        }\n")
                        fh.write("    }\n")
            fh.write("    return p;\n")
            fh.write("}\n\n")
        else:
            msgtype = msg.get("msgtype", "")
            fh.write(f"FFStatus {mname}::toString(falconfix::FastBuffer &out, bool includeMilliseconds) const noexcept {{\n")
            fh.write(f"    static constexpr std::string_view HDR = \"8={include_root}\\x01\"\"9=000000\\x01\"\"35={msgtype}\\x01\";\n")
            fh.write("    static constexpr std::size_t HDR_BL_POS = 11;\n")
            fh.write("    static constexpr std::size_t HDR_CALC_OFFSET = 18;\n\n")
            fh.write("    out.clear();\n")
            fh.write("    if (!out.reserve_noexcept(compute_buffer_size())) [[unlikely]] {\n")
            fh.write("        return FF_SYS_ERR(BAD_ALLOC);\n")
            fh.write("    }\n\n")
            fh.write("    char *p = out.data();\n")
            fh.write("    std::memcpy(p, HDR.data(), HDR.size());\n")
            fh.write("    p += HDR.size();\n")
            fh.write("    if ((p = m_Header.encode(p, includeMilliseconds)) == nullptr) {\n")
            fh.write("        return FF_FIX_VALIDATION_ERR(COMPONENT_VALIDATION_FAILED, field_id::BeginString);\n")
            fh.write("    }\n\n")
            for tag, name in ordered_items:
                if tag == "field":
                    line = _emit_encode_field_line(name, field_type_by_name[name], ns, name_to_num)
                    if line:
                        fh.write(f"    if (has{name}()) {{ {line} }}\n")
                elif tag == "component":
                    fh.write(f"    if ((p = m_{name}.encode(p, includeMilliseconds)) == nullptr) {{\n")
                    fh.write("        return FF_FIX_VALIDATION_ERR(COMPONENT_VALIDATION_FAILED, 0);\n")
                    fh.write("    }\n")
                elif tag == "group":
                    tag_num = name_to_num.get(name)
                    if tag_num:
                        fh.write(f"    if (!m_{name}Group.empty()) {{\n")
                        fh.write(f"        WRITE_TAG{len(str(tag_num)) + 1}({ns}::tag_const::{name});\n")
                        fh.write(f"        p += falconfix::itoa(static_cast<int64_t>(m_{name}Group.size()), p);\n")
                        fh.write(f"        *p++ = {ns}::serialize::SOH;\n")
                        fh.write(f"        for (const auto &entry : m_{name}Group) {{\n")
                        fh.write("            p = entry.encode(p, includeMilliseconds);\n")
                        fh.write("        }\n")
                        fh.write("    }\n")
            fh.write("\n")
            fh.write("    if ((p = m_Trailer.encode(p, includeMilliseconds)) == nullptr) {\n")
            fh.write("        return FF_FIX_VALIDATION_ERR(COMPONENT_VALIDATION_FAILED, field_id::SignatureLength);\n")
            fh.write("    }\n\n")
            fh.write("    const std::size_t msg_len_no_checksum = static_cast<std::size_t>(p - out.data());\n")
            fh.write("    const std::size_t body_len = msg_len_no_checksum - HDR_CALC_OFFSET;\n\n")
            fh.write("    char *digits = out.data() + HDR_BL_POS;\n")
            fh.write("    digits[0] = '0' + ((body_len / 100000) % 10);\n")
            fh.write("    digits[1] = '0' + ((body_len / 10000 ) % 10);\n")
            fh.write("    digits[2] = '0' + ((body_len / 1000  ) % 10);\n")
            fh.write("    digits[3] = '0' + ((body_len / 100   ) % 10);\n")
            fh.write("    digits[4] = '0' + ((body_len / 10    ) % 10);\n")
            fh.write("    digits[5] = '0' +  (body_len           % 10);\n\n")
            fh.write("    std::uint32_t cksum = 0;\n")
            fh.write("    for (const char *it = out.data(); it != out.data() + msg_len_no_checksum; ++it) {\n")
            fh.write("        cksum += static_cast<unsigned char>(*it);\n")
            fh.write("    }\n")
            fh.write("    cksum &= 0xFF;\n\n")
            fh.write("    *p++ = '1'; *p++ = '0'; *p++ = '=';\n")
            fh.write("    *p++ = char('0' + (cksum / 100));\n")
            fh.write("    *p++ = char('0' + ((cksum / 10) % 10));\n")
            fh.write("    *p++ = char('0' + (cksum % 10));\n")
            fh.write(f"    *p++ = {ns}::serialize::SOH;\n")
            fh.write("    out.commit(static_cast<std::size_t>(p - out.data()));\n")
            fh.write("    return FF_OK();\n")
            fh.write("}\n\n")

        fh.write(f"bool {mname}::setField(int32_t tag, const char *begin, const char *end) noexcept {{\n")
        fh.write("    const std::size_t len = static_cast<std::size_t>(end - begin);\n\n")
        fh.write("    switch (tag) {\n")
        for name, fix_type in field_items:
            fh.write(f"    case field_id::{name}: {_emit_decode_set_line(name, fix_type, ns)} return true;\n")
        fh.write("    default: return false;\n")
        fh.write("    }\n")
        fh.write("}\n\n")
        fh.write(f"}} // namespace {ns}::messages\n")


def generate(cpp_messages_dir: Path, spec: dict):
    cpp_messages_dir.mkdir(parents=True, exist_ok=True)

    ns, include_root, field_type_map, name_to_num, all_components_map = _build_context(spec)

    header = spec.get("header")
    trailer = spec.get("trailer")

    if header:
        _write_generic_message_cpp(
            cpp_messages_dir / "Header.cpp",
            "Header",
            header,
            ns,
            include_root,
            field_type_map,
            all_components_map,
            name_to_num,
            is_header_or_trailer=True,
        )

    if trailer:
        _write_generic_message_cpp(
            cpp_messages_dir / "Trailer.cpp",
            "Trailer",
            trailer,
            ns,
            include_root,
            field_type_map,
            all_components_map,
            name_to_num,
            is_header_or_trailer=True,
        )

    count = 0
    for msg in spec.get("messages", []):
        mname = normalize_name(msg.get("name") or "")
        if not mname:
            continue

        _write_generic_message_cpp(
            cpp_messages_dir / f"{mname}.cpp",
            mname,
            msg,
            ns,
            include_root,
            field_type_map,
            all_components_map,
            name_to_num,
            is_header_or_trailer=False,
        )
        count += 1

    print(f"[ok] Generated .cpp files for {count} messages")
