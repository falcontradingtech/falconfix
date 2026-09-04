# SPDX-License-Identifier: MIT
# Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo

from pathlib import Path
import re
import xml.etree.ElementTree as ET
from dataclasses import dataclass
from typing import List, Dict, Tuple, Union

from code.common import open_generated_file, write_generated_line_header


# ===========================================================
# Small FIX structure model (Fields / Components / Groups)
# ===========================================================

@dataclass
class FieldRef:
    tag: int


@dataclass
class ComponentRef:
    name: str


@dataclass
class GroupRef:
    name: str       # example: "NoSecurityAltID"
    num_tag: int    # tag of NUMINGROUP field (example: 454)
    entries: List["Entry"]


Entry = Union[FieldRef, ComponentRef, GroupRef]


def _xml_local_name(tag: str) -> str:
    """Remove namespace from XML tag if present."""
    if "}" in tag:
        return tag.split("}", 1)[1]
    return tag


def _load_fix_model_from_xml(xml_file: Path):
    """
    Reads FIX XML and builds:
    - fields_by_name: map name->tag
    - messages_model: map messageName->List[Entry]
    - components_model: map componentName->List[Entry]
    """
    tree = ET.parse(xml_file)
    root = tree.getroot()

    fields_by_name: Dict[str, int] = {}

    # ---------- <fields> ----------
    fields_section = None
    for child in root:
        if _xml_local_name(child.tag) == "fields":
            fields_section = child
            break
    if fields_section is None:
        raise RuntimeError("XML FIX: section <fields> not found.")

    for f in fields_section:
        if _xml_local_name(f.tag) != "field":
            continue
        name = f.attrib["name"]
        num = int(f.attrib["number"])
        fields_by_name[name] = num

    # ---------- generic parser for entries ----------
    def parse_entries(parent) -> List[Entry]:
        entries: List[Entry] = []
        for elem in parent:
            tag_name = _xml_local_name(elem.tag)
            if tag_name == "field":
                fname = elem.attrib["name"]
                if fname not in fields_by_name:
                    continue
                tag_num = fields_by_name[fname]
                entries.append(FieldRef(tag=tag_num))

            elif tag_name == "component":
                cname = elem.attrib["name"]
                entries.append(ComponentRef(name=cname))

            elif tag_name == "group":
                gname = elem.attrib["name"]
                num_tag: int

                num_attr = elem.attrib.get("numInGroup")
                if num_attr and num_attr in fields_by_name:
                    num_tag = fields_by_name[num_attr]
                elif num_attr and num_attr.isdigit():
                    num_tag = int(num_attr)
                else:
                    if gname not in fields_by_name:
                        continue
                    num_tag = fields_by_name[gname]

                child_entries = parse_entries(elem)
                entries.append(GroupRef(name=gname, num_tag=num_tag, entries=child_entries))

        return entries

    # ---------- <components> ----------
    components_model: Dict[str, List[Entry]] = {}
    components_section = None
    for child in root:
        if _xml_local_name(child.tag) == "components":
            components_section = child
            break

    if components_section is not None:
        for comp in components_section:
            if _xml_local_name(comp.tag) != "component":
                continue
            cname = comp.attrib["name"]
            components_model[cname] = parse_entries(comp)

    # ---------- HEADER / TRAILER (necessary addition) ----------
    header_section = None
    trailer_section = None

    for child in root:
        tag = _xml_local_name(child.tag)
        if tag == "header":
            header_section = child
        elif tag == "trailer":
            trailer_section = child

    if header_section is not None:
        components_model["header"] = parse_entries(header_section)

    if trailer_section is not None:
        components_model["trailer"] = parse_entries(trailer_section)

    # ---------- <messages> ----------
    messages_model: Dict[str, List[Entry]] = {}
    messages_section = None
    for child in root:
        if _xml_local_name(child.tag) == "messages":
            messages_section = child
            break
    if messages_section is None:
        raise RuntimeError("XML FIX: section <messages> not found.")

    for msg in messages_section:
        if _xml_local_name(msg.tag) != "message":
            continue
        mname = msg.attrib["name"]
        messages_model[mname] = parse_entries(msg)


    return fields_by_name, messages_model, components_model


def _read_namespaces_from_fix_samples(fix_samples: Path) -> tuple[str, str]:
    """
    Reads fix_messages.h and discovers:
      - samples_namespace: example: "fix44::samples"
      - messages_namespace_prefix: example: "fix44" (para gerar fix44::messages::X)

    Nothing hardcoded: everything comes from 'namespace ...::samples {' do arquivo.
    """
    text = fix_samples.read_text(encoding="utf-8")

    m = re.search(r'namespace\s+([a-zA-Z0-9_:]+)::samples\s*{', text)
    if not m:
        raise RuntimeError("Could not find 'namespace <X>::samples {' in fix_messages.h")

    ns_prefix = m.group(1)            # ex: "fix44" ou "fix44sp0"
    samples_ns = f"{ns_prefix}::samples"
    return samples_ns, ns_prefix


# ===========================================================
# Leitura do fix_messages.h (map_samples_data)
# ===========================================================

def _unescape_cpp_string_literal(s: str) -> str:
    """
    Converts C\+\+ string escapes (\\x01, \\n, \\", etc) to actual text.
    """
    return bytes(s, "utf-8").decode("unicode_escape")


def _read_samples_map(fix_samples: Path) -> Dict[str, str]:
    """
    Reads fix_messages.h and extracts o map_samples_data[] como:

        { "Heartbeat": "8=FIX4.4\x01...10=128\x01", ... }

    Nothing hardcoded: everything is parsed a partir do C++ gerado.
    """
    text = fix_samples.read_text(encoding="utf-8")

    samples: Dict[str, str] = {}

    # Captures each SamplePair{"Name", ... }
    entry_pattern = re.compile(
        r'SamplePair\{"([^"]+)",\s*(?P<content>.*?)\n\s*},',
        re.DOTALL,
    )

    # Extracts C\+\+ string literals from do 
    string_literal_pattern = re.compile(r'"((?:[^"\\]|\\.)*)"')

    for m in entry_pattern.finditer(text):
        msg_name = m.group(1)
        content = m.group("content")

        literal_parts = []
        for sm in string_literal_pattern.finditer(content):
            literal_cpp = sm.group(1)
            literal_parts.append(_unescape_cpp_string_literal(literal_cpp))

        raw_fix = "".join(literal_parts)
        samples[msg_name] = raw_fix

    return samples


def _parse_fix_raw_into_fields(raw_fix: str) -> List[Tuple[int, str]]:
    """
    FIX string with real SOH -> ordered list of (tag, value).
    """
    fields: List[Tuple[int, str]] = []
    for part in raw_fix.split("\x01"):
        if not part:
            continue
        if "=" not in part:
            continue
        tag_str, value = part.split("=", 1)
        tag_str = tag_str.strip()
        if not tag_str:
            continue
        try:
            tag = int(tag_str)
        except ValueError:
            continue
        fields.append((tag, value))
    return fields


def _cpp_escape_string(value: str) -> str:
    """
    Escapes text for literal C++.
    """
    value = value.replace("\\", "\\\\")
    value = value.replace('"', '\\"')
    return value


# ===========================================================
# C\+\+ code emission (respecting components and groups)
# ===========================================================

def _load_header_trailer_tags(xml_file: Path,
                              fields_by_name: Dict[str, int]) -> tuple[set[int], set[int]]:
    """
    Reads <header> and <trailer> from XML and returns:
    - header_tags: set of tags that belong to header
    - trailer_tags: set of tags that belong to trailer
    """
    tree = ET.parse(xml_file)
    root = tree.getroot()

    header_tags: set[int] = set()
    trailer_tags: set[int] = set()

    def collect_tags(node) -> set[int]:
        tags: set[int] = set()
        for elem in node:
            tag_name = _xml_local_name(elem.tag)
            if tag_name == "field":
                fname = elem.attrib.get("name")
                if fname in fields_by_name:
                    tags.add(fields_by_name[fname])
            elif tag_name == "group":
                # header/trailer podem ter grupos (ex: NoHops)
                tags |= collect_tags(elem)
        return tags

    for child in root:
        tag_name = _xml_local_name(child.tag)
        if tag_name == "header":
            header_tags = collect_tags(child)
        elif tag_name == "trailer":
            trailer_tags = collect_tags(child)

    return header_tags, trailer_tags


def _consume_field(fields: List[Tuple[int, str]],
                   idx: int,
                   tag: int,
                   tags_seen: set[int]) -> Tuple[int, str | None]:
    """
    Searches for the next field com 'tag' a partir de idx.
    If found, returns (new_idx, value) e marks the tag em tags_seen.
    If not found, returns (idx_final, None).
    """
    n = len(fields)
    i = idx
    while i < n and fields[i][0] != tag:
        i += 1
    if i >= n:
        return n, None
    _, value = fields[i]
    tags_seen.add(tag)
    return i + 1, value


def _emit_entries(entries: List[Entry],
                  access_expr: str,
                  indent: str,
                  fields: List[Tuple[int, str]],
                  idx: int,
                  components_model: Dict[str, List[Entry]],
                  tags_seen: set[int]) -> Tuple[List[str], int]:
    """
    Recursive walk through structure (fields, components, groups)
    and generates setField/comp/group adequadas.

    Note:
      - Field consumption order follows the FIX sample order
        via .idx and '_consume_field'.
      - Components .StandardHeader. and .StandardTrailer. are inlined
        directly in .access_expr..
    """
    lines: List[str] = []

    for entry in entries:
        if isinstance(entry, FieldRef):
            idx, val = _consume_field(fields, idx, entry.tag, tags_seen)
            if val is None:
                continue
            cpp_val = _cpp_escape_string(val)
            lines.append(f'{indent}{access_expr}.setField({entry.tag}, "{cpp_val}");')

        elif isinstance(entry, ComponentRef):
            cname = entry.name
            # Inlinar StandardHeader/StandardTrailer direto em access_expr
            if cname in ("StandardHeader", "StandardTrailer"):
                inner_entries = components_model.get(cname, [])
                inner_lines, idx = _emit_entries(
                    inner_entries,
                    access_expr,
                    indent,
                    fields,
                    idx,
                    components_model,
                    tags_seen,
                )
                lines.extend(inner_lines)
            else:
                var_name = f"comp_{cname}"
                lines.append(f"{indent}auto &{var_name} = {access_expr}.getRef{cname}();")
                inner_entries = components_model.get(cname, [])
                inner_lines, idx = _emit_entries(
                    inner_entries,
                    var_name,
                    indent,
                    fields,
                    idx,
                    components_model,
                    tags_seen,
                )
                lines.extend(inner_lines)

        elif isinstance(entry, GroupRef):
            # Reads the group counter (NUMINGROUP)
            idx, count_val = _consume_field(fields, idx, entry.num_tag, tags_seen)
            if count_val is None:
                continue
            try:
                count = int(count_val)
            except ValueError:
                count = 0

            if count <= 0:
                continue

            gname = entry.name        # ex: "NoLegs"
            vec_name = f"vec_{gname}"
            grp_name = f"g_{gname}"

            # setter do contador
            lines.append(f'{indent}{access_expr}.setField({entry.num_tag}, "{_cpp_escape_string(count_val)}");')

            # HERE IS THE FIX:
            # Correct name for vector getter:
            #   getRef<NomeDoGrupo>Group()
            group_accessor = f"getRef{gname}Group()"

            lines.append(f"{indent}auto &{vec_name} = {access_expr}.{group_accessor};")
            lines.append(f"{indent}for (int i = 0; i < {count}; ++i) {{")
            lines.append(f"{indent}    auto &{grp_name} = {vec_name}.emplace_back();")

            inner_entries = entry.entries
            inner_lines, idx = _emit_entries(
                inner_entries,
                grp_name,
                indent + "    ",
                fields,
                idx,
                components_model,
                tags_seen,
            )
            lines.extend(inner_lines)
            lines.append(f"{indent}}}")


    return lines, idx


def _generate_object_function(msg_name: str,
                              fields: List[Tuple[int, str]],
                              msg_entries: List[Entry],
                              components_model: Dict[str, List[Entry]],
                              messages_ns_prefix: str,
                              header_tags: set[int],
                              trailer_tags: set[int]) -> str:
    """
    Generates:

        inline <ns>::messages::<Msg> create<Msg>() noexcept { ... }

    Rules:
    - Order always same as FIX sample.
    - Header ->  -> Trailer, but using the same order da FIX.
    - XML is only used to know where are components/groups.
    """

    func_name = f"create{msg_name}"
    msg_type  = f"{messages_ns_prefix}::messages::{msg_name}"

    lines: List[str] = []
    indent = "    "

    # ------------------------------------------------------------------
    # 1) Partitions FIX into header /  / trailer maintaining the ORDER
    # ------------------------------------------------------------------
    header_fields: List[Tuple[int, str]] = []
    _fields:   List[Tuple[int, str]] = []
    trailer_fields: List[Tuple[int, str]] = []

    for tag, value in fields:
        if tag in header_tags:
            header_fields.append((tag, value))
        elif tag in trailer_tags:
            trailer_fields.append((tag, value))
        else:
            _fields.append((tag, value))

    # ------------------------------------------------------------------
    # 2) C\+\+ function header
    # ------------------------------------------------------------------
    lines.append(f"inline {msg_type} {func_name}() noexcept {{")
    lines.append(f"{indent}{msg_type} msg;")

    # ------------------------------------------------------------------
    # 3) Emite HEADER exatamente nthe ORDER da FIX
    # ------------------------------------------------------------------
    for tag, value in header_fields:
        cpp_val = _cpp_escape_string(value)
        lines.append(f'{indent}msg.setField({tag}, "{cpp_val}");')

    # ------------------------------------------------------------------
    # 4) 
    #    - If the message has components/groups -> uses _emit_entries
    #    - If simple (only fields) -> only calls msg.setField nthe ORDER
    # ------------------------------------------------------------------
    has_complex = any(isinstance(e, (ComponentRef, GroupRef)) for e in msg_entries)
    tags_seen: set[int] = set()

    if has_complex:
        # usa apenas os campos de  para o XML
        _lines, _ = _emit_entries(
            msg_entries,
            "msg",
            indent,
            _fields,
            0,
            components_model,
            tags_seen,
        )
        lines.extend(_lines)

        # leftovers from: fields not described no XML -> msg.setField direto,
        # still in ORDER da FIX (_fields)
        for tag, value in _fields:
            if tag in tags_seen:
                continue
            cpp_val = _cpp_escape_string(value)
            lines.append(f'{indent}msg.setField({tag}, "{cpp_val}");')
    else:
        # simple message (example: Heartbeat, ResendRequest):
        # only emits o  nthe ORDER da FIX
        for tag, value in _fields:
            cpp_val = _cpp_escape_string(value)
            lines.append(f'{indent}msg.setField({tag}, "{cpp_val}");')

    # ------------------------------------------------------------------
    # 5) TRAILER no fim, nthe ORDER da FIX (usually only tag 10)
    # ------------------------------------------------------------------
    for tag, value in trailer_fields:
        cpp_val = _cpp_escape_string(value)
        lines.append(f'{indent}msg.setField({tag}, "{cpp_val}");')

    lines.append(f"{indent}return msg;")
    lines.append("}")

    return "\n".join(lines)


# ===========================================================
# Main function: generate
# ===========================================================

def generate(samples_dir: Path, xml_file: Path) -> None:
    """
    Generates object_messages.h from:
    - fix_messages.h         (which messages + valores reais)
    - xml_file (FIX)         (structure: fields, components, groups)

    Rules:
    - Somente mensagens presentes in fix_messages.h.
    - Nothing hardcoded: namespaces, names, tags, values -> fix_messages.h + XML.
    - Components: getRef<Componente>()
    - Groups:      get<NomeDoGrupo>()  + std::vector<GroupName>
    """
    fix_samples = samples_dir / "fix_messages.h"
    outpath = samples_dir / "object_messages.h"

    samples_map = _read_samples_map(fix_samples)
    fields_by_name, messages_model, components_model = _load_fix_model_from_xml(xml_file)
    header_tags, trailer_tags = _load_header_trailer_tags(xml_file, fields_by_name)

    # Reads namespaces from fix_messages.h
    samples_namespace, messages_ns_prefix = _read_namespaces_from_fix_samples(fix_samples)

    header_lines = [
        "#pragma once",
        "",
        '#include <fix_messages.h>',
        "",
        f"namespace {samples_namespace} {{",
        "",
    ]

    _sections: List[str] = []

    # Generates one function per message que existe in fix_messages.h
    for msg_name, raw_fix in samples_map.items():
        if msg_name not in messages_model:
            print(f"[warn] message not found in XML, skipping in object_messages.h")
            continue

        fields = _parse_fix_raw_into_fields(raw_fix)
        msg_entries = messages_model[msg_name]

        fn_code = _generate_object_function(
            msg_name,
            fields,
            msg_entries,
            components_model,
            messages_ns_prefix,
            header_tags,
            trailer_tags,
        )
        _sections.append(fn_code)
        _sections.append("")  # blank line between functions

    footer_lines = [
        f"}} // namespace {samples_namespace}",
        "",
    ]

    out_text = "\n".join(header_lines + _sections + footer_lines)
    with open_generated_file(outpath, encoding="utf-8") as fh:
        write_generated_line_header(fh)
        fh.write(out_text)

    print(f"[ok] object_messages.h written -> {outpath}")
