# -*- coding: utf-8 -*-

# SPDX-License-Identifier: MIT
# Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo

"""
generator_samples.py
---------------------------------------------------------
Generates message_samples.h with FIX RAW payloads in
constexpr std::string_view form, reading the XML dictionary directly and preserving:

- Actual XML declaration order
- Recursive groups / subgroups
- Inline components
- Defaults and overrides from samples.yaml
- only_required
- group_repetitions
- delimiter (SOH or custom)
- Actual BodyLength (9) and CheckSum (10) calculation

No types are inferred:
-> only YAML defaults (mode B) plus XML field types are used.
"""

from pathlib import Path
import re
import yaml
import xml.etree.ElementTree as ET

from code.common import (
    open_generated_file,
    write_generated_line_header,
    PRIMITIVE_CHAR,
    PRIMITIVE_BOOLEAN,
    PRIMITIVE_INT,
    PRIMITIVE_FLOAT,
    TEMPORAL_TYPES,
    STRING_LIKE_TYPES,
)
from code.xml_parser import parse as parse_xml
from code.generator_messages import _iter_message_items
from code.generator_components import _iter_component_items

# ---------------------------------------------------------
# LOAD SAMPLES CONFIG
# ---------------------------------------------------------

def _load_samples_config(config_file: Path, default_fix_version: str | None = None):
    """
    Loads samples.yaml.
    If does not exist -> creates automatically with safe seguros.
    """

    # If file does not exist -> generate initial template inicial
    if not config_file.exists():
        with open_generated_file(config_file, encoding="utf-8") as fh:
            write_generated_line_header(fh, "#")
            fh.write(
f"""fix_version: {default_fix_version}
only_required: false
group_repetitions: 1
delimiter: SOH
checksum: true

header_defaults:
  SenderCompID: BRK
  TargetCompID: CLT
  MsgSeqNum: 1
  SendingTime: 20250101000000000

default_values:

  ## ───────── CHAR / BOOLEAN ─────────
  CHAR: "1"
  BOOLEAN: "Y"

  ## ───────── INT FAMILY ─────────
  INT: "1"
  SEQNUM: "1"
  NUMINGROUP: "1"
  LENGTH: "1"
  DAYOFMONTH: "15"

  ## ───────── FLOAT FAMILY ─────────
  PRICE: 35.5
  QTY: 100
  AMT: 100
  FLOAT: 100
  PRICEOFFSET: 1
  PERCENTAGE: 99

  ## ───────── DATES / TIME ─────────
  UTCTIMESTAMP: "20250101000000000"
  UTCDATE: "20250101"
  UTCDATEONLY: "20250101"
  UTCTIMEONLY: "10:00:00.000"
  TZTIMEONLY: "10:00:00.000Z"
  LOCALMKTDATE: "20250101"

  ## ───────── STRING FAMILY ─────────
  STRING: "VAL"
  MULTIPLEVALUESTRING: "0"
  MULTIPLESTRINGVALUE: "0"
  MULTIPLECHARVALUE: "0"
  EXCHANGE: "NYSE"
  COUNTRY: "US"
  CURRENCY: "DOL"
  LANGUAGE: "EN"
  XMLDATA: "XML-DATA"
  DATA: "DATA"
  MONTHYEAR: "122035"

override_fields:
  TestReqID: REQID_1
  ClOrdID: CLORDID_123

""".lstrip(),
            )

    with open(config_file, "r", encoding="utf-8") as fp:
        cfg = yaml.safe_load(fp) or {}

    # defaults
    cfg.setdefault("only_required", False)
    cfg.setdefault("group_repetitions", 1)
    cfg.setdefault("delimiter", "SOH")
    cfg.setdefault("checksum", True)
    cfg.setdefault("default_values", {})
    cfg.setdefault("override_fields", {})
    cfg.setdefault("header_defaults", {})

    cfg["default_values"].setdefault("UTCDATE", "20250101")
    cfg["default_values"].setdefault("UTCDATEONLY", "20250101")
    cfg["default_values"].setdefault("UTCTIMEONLY", "10:00:00.000")
    cfg["default_values"].setdefault("TZTIMEONLY", "10:00:00.000Z")
    cfg["default_values"].setdefault("LOCALMKTDATE", "20250101")

    # delimiter → "\x01"
    if str(cfg["delimiter"]).upper() == "SOH":
        cfg["_delim"] = "\x01"
    else:
        cfg["_delim"] = str(cfg["delimiter"])

    # FIX version (can be overwritten pela do XML)
    if default_fix_version:
        cfg["fix_version"] = default_fix_version

    return cfg


# ---------------------------------------------------------
# XML HELPERS / MAPS
# ---------------------------------------------------------

import xml.etree.ElementTree as ET
def extract_fix_version_from_xml(root) -> str:
    """
    Reads <fix major="" minor="" servicepack=""> from XML file
    and returns normalized version example: FIX4.4 or FIX5.0SP2
    """

    fix = root.find(".//fix")
    if fix is None:
        major = root.attrib.get("major")
        minor = root.attrib.get("minor")
        sp = root.attrib.get("servicepack")
        if not major or not minor:
            raise ValueError("missing FIX metadata in XML root attributes 'major'/'minor'")
        version = f"FIX{major}.{minor}"
        if sp and sp not in ("0", "0SP0", ""):
            version += f"SP{sp}"
        return version

    major = fix.get("major")
    minor = fix.get("minor")
    sp    = fix.get("servicepack")

    # Standard assembly
    version = f"FIX{major}.{minor}"

    # SP exists -> FIX.5.0SP2
    if sp and sp not in ("0", "0SP0", ""):
        version += f"SP{sp}"

    return version


def _normalize_name(name: str) -> str:
    """Simple normalization of field names compatible with the rest of the project."""
    return name.replace(" ", "").replace("-", "").lower()


def _namespace_from_fix_version(fix_version: str) -> str:
    """
    Converts FIX versions to canonical namespace:
      "FIX.4.4"      → fix44
      "FIX.4.4SP0"   → fix44
      "FIX.5.0SP2"   → fix50sp2
    """
    s = fix_version.strip().lower()

    # remove prefix "fix"
    if s.startswith("fix"):
        s = s[3:]

    # Remove punctuation and keep only alphanumeric characters.
    clean = "".join(ch for ch in s if ch.isalnum())

    # Some FIX XML variants append "sp0"; normalize it away.
    clean = re.sub(r"sp0$", "", clean)

    if not clean:
        return "fix"

    return "fix" + clean


def _build_field_maps_from_xml(root: ET.Element):
    """
    Mapa de:
      - field_type_map[fname] -> tipo FIX (example: "STRING", "INT", "PRICE")
      - field_num_map[fname]  -> tag number (int)
      - field_info_map[fname] -> dict bruto do elemento <field>, para length/etc.
    """
    field_type_map: dict[str, str] = {}
    field_num_map: dict[str, int] = {}
    field_info_map: dict[str, dict] = {}

    fields_parent = root.find("./fields")
    if fields_parent is None:
        return field_type_map, field_num_map, field_info_map

    for f in fields_parent.findall("./field"):
        name = f.attrib.get("name")
        if not name:
            continue
        fname = _normalize_name(name)
        num = int(f.attrib["number"])
        ftype = f.attrib.get("type", "STRING")

        field_type_map[fname] = ftype
        field_num_map[fname] = num
        # guardamos infos de tamanho etc.
        info = {
            "length": f.attrib.get("length"),
            "maxlength": f.attrib.get("maxlength"),
        }
        field_info_map[fname] = info

    return field_type_map, field_num_map, field_info_map


def _build_components_map_from_xml(root: ET.Element) -> dict[str, ET.Element]:
    """
    Mapa de componentes: nome -> elemento <component>.
    """
    comps: dict[str, ET.Element] = {}
    comps_parent = root.find("./components")
    if comps_parent is None:
        return comps

    for c in comps_parent.findall("./component"):
        name = c.attrib.get("name")
        if not name:
            continue
        comps[name] = c
    return comps


def _build_field_paths(root: ET.Element, field_num_map: dict[str, int]):
    """
    Retorna:
      field_path_by_tag[num] = Component::Group::Field
    respeitando exatamente os owners reais da spec FIX.
    """

    field_path_by_tag: dict[int, str] = {}
    field_name_by_tag: dict[int, str] = {}

    # 1) Mapa simples de nomes
    fields_parent = root.find("./fields")
    if fields_parent is not None:
        for f in fields_parent:
            num = int(f.attrib["number"])
            field_name_by_tag[num] = f.attrib["name"]

    # 2) Indexar components
    components_parent = root.find("./components")
    components = {}  # name -> element
    if components_parent is not None:
        for comp in components_parent.findall("component"):
            components[comp.attrib["name"]] = comp

    # 3) Caminho correto baseado em components reais
    def walk_component(comp: ET.Element, prefix: str):
        for child in comp:
            tag = child.tag
            name = child.attrib.get("name")

            if tag == "field":
                fname = name.lower()
                num = field_num_map.get(fname)
                if num:
                    field_path_by_tag[num] = f"{prefix}{name}"

            elif tag == "group":
                walk_group(child, f"{prefix}{name}::")

            elif tag == "component":
                cname = child.attrib["name"]
                if cname in components:
                    walk_component(components[cname], f"{prefix}{cname}::")

    def walk_group(group: ET.Element, prefix: str):
        for child in group:
            tag = child.tag
            name = child.attrib.get("name")

            if tag == "field":
                fname = name.lower()
                num = field_num_map.get(fname)
                if num:
                    field_path_by_tag[num] = f"{prefix}{name}"

            elif tag == "group":
                walk_group(child, f"{prefix}{name}::")

            elif tag == "component":
                cname = child.attrib["name"]
                if cname in components:
                    walk_component(components[cname], f"{prefix}{cname}::")

    # 4) Para cada componente da spec, registrar tudo
    for cname, comp in components.items():
        walk_component(comp, f"{cname}::")

    return field_name_by_tag, field_path_by_tag


def _build_field_owner_map(root, components_map, field_num_map):
    """
    Build map: tag -> list of possible owners according to XML structure,
    but does NOT assume this is the final owner inside a given message.
    This is just the GLOBAL POSSIBLE OWNERS.
    The final owner is resolved per message.
    """
    owner_map = {num: [] for num in field_num_map.values()}

    def add_owner(tagnum, path):
        if path not in owner_map[tagnum]:
            owner_map[tagnum].append(path)

    # Walk components
    for cname, comp in components_map.items():
        def walk(elem, path):
            for child in elem:
                name = child.attrib.get("name")
                if not name:
                    continue
                fname = name.replace(" ", "").replace("-", "").lower()

                if child.tag == "field":
                    if fname in field_num_map:
                        add_owner(field_num_map[fname], "::".join(path + [name]))

                elif child.tag == "component":
                    if name in components_map:
                        walk(components_map[name], path + [name])

                elif child.tag == "group":
                    walk(child, path + [name])

        walk(comp, [cname])

    # Walk message-level fields (top-level)
    msgs = root.find("./messages")
    if msgs is not None:
        for msg in msgs.findall("./message"):
            mname = msg.attrib.get("name", "Unknown")
            def walk_msg(elem, path):
                for child in elem:
                    name = child.attrib.get("name")
                    if not name:
                        continue
                    fname = name.replace(" ", "").replace("-", "").lower()

                    if child.tag == "field":
                        if fname in field_num_map:
                            add_owner(field_num_map[fname], "::".join(path + [name]))

                    elif child.tag == "component":
                        if name in components_map:
                            walk_msg(components_map[name], path + [name])

                    elif child.tag == "group":
                        walk_msg(child, path + [name])

            walk_msg(msg, [mname])

    return owner_map


def _build_message_specific_owner_map(
        msg_elem: ET.Element,
        components_map: dict[str, ET.Element],
        field_num_map: dict[str, int]
    ) -> dict[int, list[str]]:

    owner_map: dict[int, list[str]] = {}

    def collect(elem: ET.Element, path: list[str]):
        # ---- Fields (tag simples) ----
        for f in elem.findall("./field"):
            fname = f.attrib.get("name")
            if fname and fname in field_num_map:
                tag = field_num_map[fname]
                owner = "::".join(path + [fname])
                owner_map.setdefault(tag, []).append(owner)

        # ---- Groups (NumInGroup) ----
        for g in elem.findall("./group"):
            gname = g.attrib.get("name")
            new_path = path + ([gname] if gname else [])

            if gname and gname in field_num_map:
                tag = field_num_map[gname]
                owner = "::".join(new_path)
                owner_map.setdefault(tag, []).append(owner)

            collect(g, new_path)

        # ---- Components ----
        for c in elem.findall("./component"):
            cname = c.attrib.get("name")
            if cname and cname in components_map:
                collect(components_map[cname], path + [cname])

    msg_name = msg_elem.attrib.get("name", "Message")
    collect(msg_elem, [msg_name])

    return owner_map


# ---------------------------------------------------------
# DEFAULT VALUE
# ---------------------------------------------------------

# ================================
# AUTOMATIC DEFAULTS (if not present in samples.yaml)
# ================================
AUTO_DEFAULTS = {
    **{t: "1" for t in PRIMITIVE_INT},          # INT, LENGTH, NUMINGROUP, etc.
    **{t: "35.5" for t in PRIMITIVE_FLOAT},     # PRICE, QTY, PERCENTAGE...
    **{t: "1" for t in PRIMITIVE_CHAR},         # CHAR default  -> '1'
    **{t: "Y" for t in PRIMITIVE_BOOLEAN},      # BOOLEAN default -> Y

    # TEMPORAL (final version)
    "UTCDATE":        "20250101",
    "UTCDATEONLY":    "20250101",
    "LOCALMKTDATE":   "20250101",
    "UTCTIMEONLY":    "00:00:00.000",   # FIX canonical
    "TZTIMEONLY":     "00:00:00.000Z",  # canonical with UTC zone suffix
    "UTCTIMESTAMP":   "20250101000000000",  # continuous (compatible with serializer)
    "MONTHYEAR":      "012025",  # MMYYYY
}

def _trim_float(v: str) -> str:
    """Remove unnecessary zeros from FIX FLOAT numbers."""
    if "." not in v:
        return v
    return v.rstrip("0").rstrip(".")

def _fake_value_for(field_name, fix_type, cfg, tagnum, maxlen=None):
    # -----------------------------------------------
    # Special case: tags that need specific time formatting in FIX4.3
    # (MDEntryTime=273, TotalVolumeTradedTime=450)
    # -----------------------------------------------
    if tagnum in (273, 450):
        return "00:00:00.000"[:maxlen] if maxlen else "00:00:00.000"

    # -----------------------------------------------
    # 1) OVERRIDE DO USUARIO (vence tudo)
    # -----------------------------------------------
    if (ov := cfg["override_fields"].get(field_name)) is not None:
        val = str(ov)
        return val[:maxlen] if maxlen else val

    # -----------------------------------------------
    # 2) DEFAULT DO YAML OU AUTO_DEFAULT
    # -----------------------------------------------
    default = cfg["default_values"].get(fix_type) or AUTO_DEFAULTS.get(fix_type)

    # fallback final — nunca pode gerar None
    if default is None:
        default = "0"

    s = str(default)

    # -----------------------------------------------
    # 3) FLOAT -> normalize removing useless decimals
    # -----------------------------------------------
    if fix_type in PRIMITIVE_FLOAT:
        s = _trim_float(s)
        return s[:maxlen] if maxlen else s

    # -----------------------------------------------
    # 4) INT → sem decimal, sem sufixo nunca
    # -----------------------------------------------
    if fix_type in PRIMITIVE_INT:
        return s[:maxlen] if maxlen else s

    # -----------------------------------------------
    # 5) BOOLEAN / CHAR → sempre 1 char
    # -----------------------------------------------
    if fix_type in PRIMITIVE_BOOLEAN or fix_type in PRIMITIVE_CHAR:
        return s[0]

    # -----------------------------------------------
    # 6) TEMPORAL -- keeps exact
    # -----------------------------------------------
    if fix_type in TEMPORAL_TYPES:
        return s[:maxlen] if maxlen else s

    # -----------------------------------------------
    # 7) STRING-LIKE -- only case with suffix
    # -----------------------------------------------
    if fix_type in STRING_LIKE_TYPES:
        # se default != "VAL" respeitamos, sem sufixo
        if s != "VAL":
            return s[:maxlen] if maxlen else s
        v = f"VAL_{tagnum}"
        return v[:maxlen] if maxlen else v

    # -----------------------------------------------
    # 8) Tipos desconhecidos — fail-safe FIX-like
    # -----------------------------------------------
    return "0"


# ---------------------------------------------------------
# EMIT COMPONENT (XML)
# ---------------------------------------------------------

def _emit_component_xml(
    comp_elem: ET.Element,
    cfg: dict,
    field_type_map: dict[str, str],
    field_num_map: dict[str, int],
    field_info_map: dict[str, dict],
    components_map: dict[str, ET.Element],
    only_req: bool,
    delim: str,
):
    out: list[str] = []
    children = list(comp_elem)
    ordered_children = (
        [child for child in children if child.tag == "field"]
        + [child for child in children if child.tag == "component"]
        + [child for child in children if child.tag == "group"]
    )

    for child in ordered_children:
        tag = child.tag  # "field", "component", "group"
        name = child.attrib.get("name")
        if not name:
            continue
        fname = _normalize_name(name)

        # FIELD
        if tag == "field":
            if only_req and child.attrib.get("required", "N") != "Y":
                continue

            t = field_num_map[fname]
            fix_type = field_type_map.get(fname, "STRING")
            finfo = field_info_map.get(fname, {})
            maxlen = finfo.get("length") or finfo.get("maxlength")

            v = _fake_value_for(name, fix_type, cfg, t, maxlen)
            out.append(f"{t}={v}{delim}")
            continue

        # COMPONENT
        elif tag == "component":
            sub_comp = components_map.get(name)
            if sub_comp is None:
                continue
            sub = _emit_component_xml(
                sub_comp,
                cfg,
                field_type_map,
                field_num_map,
                field_info_map,
                components_map,
                only_req,
                delim,
            )
            out.extend(sub)
            continue

        # GROUP
        elif tag == "group":
            sub = _emit_group_xml(
                child,
                cfg,
                field_type_map,
                field_num_map,
                field_info_map,
                components_map,
                only_req,
                delim,
            )
            out.extend(sub)
            continue

    return out


# ---------------------------------------------------------
# EMIT GROUP (XML)
# ---------------------------------------------------------

def _emit_group_xml(
    grp_elem: ET.Element,
    cfg: dict,
    field_type_map: dict[str, str],
    field_num_map: dict[str, int],
    field_info_map: dict[str, dict],
    components_map: dict[str, ET.Element],
    only_req: bool,
    delim: str,
):
    """
    Retorna FIX RAW do grupo (sem aspas).
    """

    reps = int(cfg["group_repetitions"])
    gname = grp_elem.attrib["name"]
    gfname = _normalize_name(gname)
    count_tag = field_num_map[gfname]

    repetitions: list[list[str]] = []

    # Repetitions
    children = list(grp_elem)
    for _ in range(reps):
        rep_out: list[str] = []
        ordered_children = (
            [child for child in children if child.tag == "field"]
            + [child for child in children if child.tag == "component"]
            + [child for child in children if child.tag == "group"]
        )
        for child in ordered_children:
            ctag = child.tag
            cname = child.attrib.get("name")
            if not cname:
                continue
            cfname = _normalize_name(cname)

            # FIELD
            if ctag == "field":
                if only_req and child.attrib.get("required", "N") != "Y":
                    continue

                # do not repeat the own NumInGroup
                if field_num_map[cfname] == count_tag:
                    continue

                fix_type = field_type_map.get(cfname, "STRING")
                finfo = field_info_map.get(cfname, {})
                maxlen = finfo.get("length") or finfo.get("maxlength")

                v = _fake_value_for(
                    cname,
                    fix_type,
                    cfg,
                    field_num_map[cfname],
                    maxlen,
                )
                rep_out.append(f"{field_num_map[cfname]}={v}{delim}")

            # COMPONENT
            elif ctag == "component":
                sub_comp = components_map.get(cname)
                if sub_comp is None:
                    continue
                sub = _emit_component_xml(
                    sub_comp,
                    cfg,
                    field_type_map,
                    field_num_map,
                    field_info_map,
                    components_map,
                    only_req,
                    delim,
                )
                rep_out.extend(sub)

            # SUBGROUP
            elif ctag == "group":
                sub = _emit_group_xml(
                    child,
                    cfg,
                    field_type_map,
                    field_num_map,
                    field_info_map,
                    components_map,
                    only_req,
                    delim,
                )
                rep_out.extend(sub)

        if rep_out:
            repetitions.append(rep_out)

    if not repetitions:
        return []

    out: list[str] = [f"{count_tag}={len(repetitions)}{delim}"]
    for rep_out in repetitions:
        out.extend(rep_out)
    return out


# ---------------------------------------------------------
# CONSTRUIR FIX RAW PARA UMA MENSAGEM (XML)
# ---------------------------------------------------------

def _build_fix_string_for_message_xml(
    msg_elem: ET.Element,
    root: ET.Element,
    cfg: dict,
    field_type_map: dict[str, str],
    field_num_map: dict[str, int],
    field_info_map: dict[str, dict],
    components_map: dict[str, ET.Element],
):
    delim = cfg["_delim"]
    only_req = cfg["only_required"]

    parts: list[str] = []

    # 8 – BeginString
    parts.append(f"8={cfg['fix_version']}{delim}")

    # 9 – BodyLength placeholder
    parts.append(f"9=000000{delim}")

    # 35 – MsgType
    msgtype = msg_elem.attrib.get("msgtype", "")
    parts.append(f"35={msgtype}{delim}")

    # HEADER (somente fields REQUIRED)
    header = root.find("./header")
    if header is not None:
        for child in header:
            if child.tag != "field":
                continue
            if child.attrib.get("required", "N") != "Y":
                continue

            name = child.attrib.get("name")
            if not name:
                continue

            fname = _normalize_name(name)
            t = field_num_map[fname]

            # already emitted
            if t in (8, 9, 35):
                continue

            fix_type = field_type_map.get(fname, "STRING")
            finfo = field_info_map.get(fname, {})
            maxlen = finfo.get("length") or finfo.get("maxlength")

            if name in cfg["header_defaults"]:
                v = str(cfg["header_defaults"][name])
                if maxlen:
                    v = v[:maxlen]
            else:
                v = _fake_value_for(name, fix_type, cfg, t, maxlen)

            parts.append(f"{t}={v}{delim}")

    # BODY (ordem exata do XML)
    for child in msg_elem:
        tag = child.tag
        name = child.attrib.get("name")
        if not name:
            continue
        fname = _normalize_name(name)

        # FIELD
        if tag == "field":
            if only_req and child.attrib.get("required", "N") != "Y":
                continue

            fnum = field_num_map[fname]
            fix_type = field_type_map.get(fname, "STRING")
            finfo = field_info_map.get(fname, {})
            maxlen = finfo.get("length") or finfo.get("maxlength")

            v = _fake_value_for(name, fix_type, cfg, fnum, maxlen)
            parts.append(f"{fnum}={v}{delim}")
            continue

        # COMPONENT
        elif tag == "component":
            comp_elem = components_map.get(name)
            if comp_elem is None:
                continue
            sub = _emit_component_xml(
                comp_elem,
                cfg,
                field_type_map,
                field_num_map,
                field_info_map,
                components_map,
                only_req,
                delim,
            )
            parts.extend(sub)
            continue

        # GROUP
        elif tag == "group":
            sub = _emit_group_xml(
                child,
                cfg,
                field_type_map,
                field_num_map,
                field_info_map,
                components_map,
                only_req,
                delim,
            )
            parts.extend(sub)
            continue

    # Concatena e aplica BodyLength + CheckSum
    fix_raw = "".join(parts)
    fix_raw = _fixup_lengths(fix_raw, delim) or ""
    return fix_raw


# ---------------------------------------------------------
# CALCULAR BODYLENGTH (9=) E CHECKSUM (10=)
# ---------------------------------------------------------

def _fixup_lengths(fix_raw: str, delim: str):
    """
    BodyLength = bytes entre SOH depois do '9=xxxxx' e antes de '10='
    CheckSum   = soma de todos os bytes % 256 (incluindo delim)
    """

    m = re.search(r"9=000000" + re.escape(delim), fix_raw)
    if not m:
        return fix_raw

    start_body = m.end()

    last10 = re.search(r"10=\d{3}" + re.escape(delim) + r"$", fix_raw)
    raw_no_cs = fix_raw if not last10 else fix_raw[: last10.start()]

    body_len = len(raw_no_cs.encode()) - start_body

    fixed = fix_raw[:m.start()] + f"9={body_len:06d}{delim}" + fix_raw[m.end():]

    if last10:
        fixed = fixed[: last10.start()]

    chk = sum(fixed.encode()) % 256

    return fixed + f"10={chk:03d}{delim}"



# ---------------------------------------------------------
# CONVERTER FIX RAW → C++ LITERAL
# ---------------------------------------------------------

def _cpp_literal_from_fix_string(
        fix_str: str,
        numingroup_tags: set[int],
        owner_map: dict[int, str],
        cfg: dict) -> str:

    print_comments = cfg.get("print_owner_comments", False)

    out = []
    segments = fix_str.split("\x01")

    for seg in segments:
        if "=" not in seg:
            continue

        tag = int(seg.split("=", 1)[0])
        owner = owner_map.get(tag)

        if isinstance(owner, list):
            owner = owner[-1]      # fallback: takes only the last
        comment = f"  // {owner}" if (print_comments and owner) else ""

        out.append(f'        "{seg}\\x01"{comment}')

    return "\n".join(out)


def _build_field_maps_from_spec(spec: dict):
    field_type_map: dict[str, str] = {}
    field_num_map: dict[str, int] = {}
    field_info_map: dict[str, dict] = {}

    for f in spec.get("fields", []):
        name = f.get("name")
        number = f.get("number")
        if not name or number is None:
            continue
        fname = _normalize_name(name)
        field_type_map[fname] = f.get("type", "STRING")
        field_num_map[fname] = int(number)
        field_info_map[fname] = {
            "length": f.get("length"),
            "maxlength": f.get("maxlength"),
        }

    return field_type_map, field_num_map, field_info_map


def _emit_field_sample(
    field_name: str,
    required: str,
    cfg: dict,
    field_type_map: dict[str, str],
    field_num_map: dict[str, int],
    field_info_map: dict[str, dict],
    only_req: bool,
    delim: str,
) -> list[str]:
    if only_req and required != "Y":
        return []

    fname = _normalize_name(field_name)
    tag_num = field_num_map[fname]
    fix_type = field_type_map.get(fname, "STRING")
    finfo = field_info_map.get(fname, {})
    maxlen = finfo.get("length") or finfo.get("maxlength")
    value = _fake_value_for(field_name, fix_type, cfg, tag_num, maxlen)
    return [f"{tag_num}={value}{delim}"]


def _emit_component_spec(
    comp_payload: dict,
    cfg: dict,
    field_type_map: dict[str, str],
    field_num_map: dict[str, int],
    field_info_map: dict[str, dict],
    all_components_map: dict[str, dict],
    only_req: bool,
    delim: str,
) -> list[str]:
    out: list[str] = []
    items = list(_iter_component_items(comp_payload, all_components_map))
    has_groups = any(tag == "group" for tag, _name, _payload in items)

    if has_groups:
        for tag, _name, payload in items:
            if tag == "group":
                out.extend(
                    _emit_group_spec(
                        payload,
                        cfg,
                        field_type_map,
                        field_num_map,
                        field_info_map,
                        all_components_map,
                        only_req,
                        delim,
                    )
                )
        return out

    for tag, name, payload in items:
        if tag == "field":
            out.extend(
                _emit_field_sample(
                    payload["name"],
                    payload.get("required", "N"),
                    cfg,
                    field_type_map,
                    field_num_map,
                    field_info_map,
                    only_req,
                    delim,
                )
            )
        elif tag == "component":
            out.extend(
                _emit_component_spec(
                    payload,
                    cfg,
                    field_type_map,
                    field_num_map,
                    field_info_map,
                    all_components_map,
                    only_req,
                    delim,
                )
            )

    return out


def _emit_group_spec(
    group_payload: dict,
    cfg: dict,
    field_type_map: dict[str, str],
    field_num_map: dict[str, int],
    field_info_map: dict[str, dict],
    all_components_map: dict[str, dict],
    only_req: bool,
    delim: str,
) -> list[str]:
    reps = int(cfg["group_repetitions"])
    group_name = group_payload["name"]
    count_tag = field_num_map[_normalize_name(group_name)]
    repetitions: list[list[str]] = []

    items = list(_iter_message_items(group_payload, all_components_map))
    field_items = [(name, payload) for tag, name, payload in items if tag == "field"]
    component_items = [(name, payload) for tag, name, payload in items if tag == "component"]
    subgroup_items = [(name, payload) for tag, name, payload in items if tag == "group"]

    for _ in range(reps):
        rep_out: list[str] = []
        for _name, payload in field_items:
            rep_out.extend(
                _emit_field_sample(
                    payload["name"],
                    payload.get("required", "N"),
                    cfg,
                    field_type_map,
                    field_num_map,
                    field_info_map,
                    only_req,
                    delim,
                )
            )
        for _name, payload in component_items:
            rep_out.extend(
                _emit_component_spec(
                    payload,
                    cfg,
                    field_type_map,
                    field_num_map,
                    field_info_map,
                    all_components_map,
                    only_req,
                    delim,
                )
            )
        for _name, payload in subgroup_items:
            rep_out.extend(
                _emit_group_spec(
                    payload,
                    cfg,
                    field_type_map,
                    field_num_map,
                    field_info_map,
                    all_components_map,
                    only_req,
                    delim,
                )
            )

        if rep_out:
            repetitions.append(rep_out)

    if not repetitions:
        return []

    out = [f"{count_tag}={len(repetitions)}{delim}"]
    for rep_out in repetitions:
        out.extend(rep_out)
    return out


def _build_fix_string_for_message_spec(
    msg_payload: dict,
    spec: dict,
    cfg: dict,
    field_type_map: dict[str, str],
    field_num_map: dict[str, int],
    field_info_map: dict[str, dict],
    all_components_map: dict[str, dict],
):
    delim = cfg["_delim"]
    only_req = cfg["only_required"]
    parts: list[str] = []

    parts.append(f"8={cfg['fix_version']}{delim}")
    parts.append(f"9=000000{delim}")
    parts.append(f"35={msg_payload.get('msgtype', '')}{delim}")

    header = spec.get("header")
    if header is not None:
        for tag, _name, payload in _iter_component_items(header, all_components_map):
            if tag != "field" or payload.get("required", "N") != "Y":
                continue
            field_name = payload["name"]
            fname = _normalize_name(field_name)
            tag_num = field_num_map[fname]
            if tag_num in (8, 9, 35):
                continue

            fix_type = field_type_map.get(fname, "STRING")
            finfo = field_info_map.get(fname, {})
            maxlen = finfo.get("length") or finfo.get("maxlength")

            if field_name in cfg["header_defaults"]:
                value = str(cfg["header_defaults"][field_name])
                if maxlen:
                    value = value[:maxlen]
            else:
                value = _fake_value_for(field_name, fix_type, cfg, tag_num, maxlen)

            parts.append(f"{tag_num}={value}{delim}")

    for tag, _name, payload in _iter_message_items(msg_payload, all_components_map):
        if tag == "field":
            parts.extend(
                _emit_field_sample(
                    payload["name"],
                    payload.get("required", "N"),
                    cfg,
                    field_type_map,
                    field_num_map,
                    field_info_map,
                    only_req,
                    delim,
                )
            )
        elif tag == "component":
            parts.extend(
                _emit_component_spec(
                    payload,
                    cfg,
                    field_type_map,
                    field_num_map,
                    field_info_map,
                    all_components_map,
                    only_req,
                    delim,
                )
            )
        elif tag == "group":
            parts.extend(
                _emit_group_spec(
                    payload,
                    cfg,
                    field_type_map,
                    field_num_map,
                    field_info_map,
                    all_components_map,
                    only_req,
                    delim,
                )
            )

    return _fixup_lengths("".join(parts), delim) or ""


# ---------------------------------------------------------
# generates(STRING FIX)
# ---------------------------------------------------------

def generateFIX(samples_dir, config_file, xml_file):
    # Parse do XML / spec
    xml_file = Path(xml_file)
    tree = ET.parse(xml_file)
    root = tree.getroot()
    spec = parse_xml(xml_file)

    fix_version = extract_fix_version_from_xml(root)

    # Mapas de fields e componentes
    field_type_map, field_num_map, field_info_map = _build_field_maps_from_spec(spec)
    components_map = {
        _normalize_name(comp["name"]): comp
        for comp in spec.get("components", [])
        if comp.get("name")
    }
    xml_components_map = _build_components_map_from_xml(root)

    # Build maps for field comments
    field_name_by_tag, field_path_by_tag = _build_field_paths(root, field_num_map)
    owner_map_global = _build_field_owner_map(root, xml_components_map, field_num_map)


    # Which tags are NUMINGROUP (for group indentation no C++)
    numingroup_tags: set[int] = {
        num
        for fname, num in field_num_map.items()
        if field_type_map.get(fname, "").upper() == "NUMINGROUP"
    }

    # Carregar config (samples.yaml)
    cfg = _load_samples_config(config_file, fix_version)

    # Namespace derived from version
    ns = _namespace_from_fix_version(cfg["fix_version"])

    samples_dir.mkdir(parents=True, exist_ok=True)
    out_path = samples_dir / "fix_messages.h"

    header = f"""#pragma once

#include <string_view>
#include <utility>
#include <cstddef>

namespace {ns}::samples {{

using SamplePair = std::pair<std::string_view, std::string_view>;

// --------------------------------------------
// table
// --------------------------------------------
inline constexpr SamplePair map_samples_data[] = {{
"""


    footer = f"""}}; // map_samples_data

// -------------------------------------------------------
// operator[]
// -------------------------------------------------------
struct SampleMap {{
    const SamplePair* data;
    std::size_t size;

    // Message lookup by name
    constexpr std::string_view operator[](std::string_view key) const noexcept {{
        for (std::size_t i = 0; i < size; ++i) {{
            if (data[i].first == key)
                return data[i].second;
        }}
        return {{}};
    }}

    // Optional indexed access
    constexpr const SamplePair& operator[](std::size_t i) const noexcept {{
        return data[i];
    }}
}};

// --------------------------------------------
// Lookup helper
// --------------------------------------------
inline constexpr SampleMap map_samples{{
    map_samples_data,
    sizeof(map_samples_data) / sizeof(map_samples_data[0])
}};

}} // namespace {ns}::samples
"""


    lines: list[str] = [header]

    for msg_payload in spec.get("messages", []):
            mname = msg_payload.get("name", "Unknown")
            fix_str = _build_fix_string_for_message_spec(
                msg_payload,
                spec,
                cfg,
                field_type_map,
                field_num_map,
                field_info_map,
                components_map,
            )

            msg_elem = root.find(f"./messages/message[@name='{mname}']")
            owner_map = _build_message_specific_owner_map(msg_elem, _build_components_map_from_xml(root), field_num_map) if msg_elem is not None else {}

            body = _cpp_literal_from_fix_string(fix_str, numingroup_tags, owner_map, cfg)


            # cada entrada do map: {"MsgName", "8=FIX..."}
            lines.append(f'    SamplePair{{"{mname}",\n')
            lines.append(body)
            lines.append("\n    },\n")


    lines.append(footer)

    with open_generated_file(out_path, encoding="utf-8") as fh:
        write_generated_line_header(fh)
        fh.write("".join(lines))

    print(f"[ok] fix_messages.h written → {out_path}")


# ---------------------------------------------------------
# ENTRY POINT
# ---------------------------------------------------------

def generate(samples_dir: Path, config_file: Path, xml_file: Path):
    generateFIX(samples_dir, config_file, xml_file)
