#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# SPDX-License-Identifier: MIT
# Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo


"""
FalconFIX — Offline C++ code generator for FIX protocol XML specs.
New simplified version using refactored modules:
    code/common/fix_types.py
    code/common/paths.py
    code/common/formatters.py
    code/common/writers.py
    code/common/strings.py
"""

import argparse
import sys
import shutil
import tempfile
import copy
import xml.etree.ElementTree as ET
from pathlib import Path




def _assert_unique_normalized_names(items, label, normalize_fn):
    seen = {}
    for raw in items:
        norm = normalize_fn(raw)
        if norm in seen:
            prev = seen[norm]
            raise ValueError(
                f"Name collision in {label}: '{raw}' and '{prev}' normalize to '{norm}'"
            )
        seen[norm] = raw


def _validate_scope(scope: dict, scope_label: str, normalize_fn):
    fields = [x.get("name", "") for x in scope.get("fields", []) if x.get("name")]
    components = [x for x in scope.get("components", []) if isinstance(x, str) and x]
    groups = [x.get("name", "") for x in scope.get("groups", []) if x.get("name")]

    _assert_unique_normalized_names(fields, f"{scope_label} fields", normalize_fn)
    _assert_unique_normalized_names(components, f"{scope_label} components", normalize_fn)
    _assert_unique_normalized_names(groups, f"{scope_label} groups", normalize_fn)

    for g in scope.get("groups", []):
        gname = g.get("name", "<unnamed-group>")
        _validate_scope(g, f"{scope_label} group '{gname}'", normalize_fn)


def _validate_spec_collisions(spec: dict, normalize_fn):
    field_names = [x.get("name", "") for x in spec.get("fields", []) if x.get("name")]
    message_names = [x.get("name", "") for x in spec.get("messages", []) if x.get("name")]
    component_names = [x.get("name", "") for x in spec.get("components", []) if x.get("name")]

    _assert_unique_normalized_names(field_names, "spec fields", normalize_fn)
    _assert_unique_normalized_names(message_names, "spec messages", normalize_fn)
    _assert_unique_normalized_names(component_names, "spec components", normalize_fn)

    header = spec.get("header")
    if header:
        _validate_scope(header, "header", normalize_fn)

    trailer = spec.get("trailer")
    if trailer:
        _validate_scope(trailer, "trailer", normalize_fn)

    for msg in spec.get("messages", []):
        _validate_scope(msg, f"message '{msg.get('name', '<unnamed-message>')}'", normalize_fn)

    for comp in spec.get("components", []):
        _validate_scope(comp, f"component '{comp.get('name', '<unnamed-component>')}'", normalize_fn)


def _xml_local_name(tag: str) -> str:
    if "}" in tag:
        return tag.split("}", 1)[1]
    return tag


def _filter_fix_xml(xml_path: Path, message_filters: list[str]) -> Path:
    tree = ET.parse(xml_path)
    root = tree.getroot()

    fields_section = None
    header_section = None
    trailer_section = None
    messages_section = None
    components_section = None

    for child in root:
        lname = _xml_local_name(child.tag)
        if lname == "fields":
            fields_section = child
        elif lname == "header":
            header_section = child
        elif lname == "trailer":
            trailer_section = child
        elif lname == "messages":
            messages_section = child
        elif lname == "components":
            components_section = child

    if fields_section is None or messages_section is None:
        raise ValueError("Invalid FIX XML: <fields> and/or <messages> sections not found")

    component_map = {}
    if components_section is not None:
        for comp in components_section:
            if _xml_local_name(comp.tag) == "component":
                name = comp.attrib.get("name")
                if name:
                    component_map[name] = comp

    message_map = {}
    for msg in messages_section:
        if _xml_local_name(msg.tag) == "message":
            name = msg.attrib.get("name")
            if name:
                message_map[name] = msg

    missing = [name for name in message_filters if name not in message_map]
    if missing:
        raise ValueError(f"messages not found in XML: {', '.join(missing)}")

    selected_fields: set[str] = set()
    selected_components: set[str] = set()

    def walk_entry(parent: ET.Element) -> None:
        for child in parent:
            lname = _xml_local_name(child.tag)
            if lname == "field":
                fname = child.attrib.get("name")
                if fname:
                    selected_fields.add(fname)
            elif lname == "component":
                cname = child.attrib.get("name")
                if cname and cname not in selected_components:
                    selected_components.add(cname)
                    comp_elem = component_map.get(cname)
                    if comp_elem is not None:
                        walk_entry(comp_elem)
            elif lname == "group":
                gname = child.attrib.get("name")
                if gname:
                    selected_fields.add(gname)
                walk_entry(child)

    if header_section is not None:
        walk_entry(header_section)
    if trailer_section is not None:
        walk_entry(trailer_section)
    for message_name in message_filters:
        walk_entry(message_map[message_name])

    new_root = ET.Element(root.tag, root.attrib)

    for child in root:
        lname = _xml_local_name(child.tag)
        if lname == "fields":
            new_fields = ET.SubElement(new_root, child.tag, child.attrib)
            for field in child:
                if _xml_local_name(field.tag) != "field":
                    continue
                if field.attrib.get("name") in selected_fields:
                    new_fields.append(copy.deepcopy(field))
        elif lname == "header":
            new_root.append(copy.deepcopy(child))
        elif lname == "trailer":
            new_root.append(copy.deepcopy(child))
        elif lname == "messages":
            new_messages = ET.SubElement(new_root, child.tag, child.attrib)
            for message_name in message_filters:
                new_messages.append(copy.deepcopy(message_map[message_name]))
        elif lname == "components":
            new_components = ET.SubElement(new_root, child.tag, child.attrib)
            for comp in child:
                if _xml_local_name(comp.tag) != "component":
                    continue
                if comp.attrib.get("name") in selected_components:
                    new_components.append(copy.deepcopy(comp))
        else:
            new_root.append(copy.deepcopy(child))

    tmp = tempfile.NamedTemporaryFile(prefix="falconfix_filtered_", suffix=".xml", delete=False)
    tmp_path = Path(tmp.name)
    tmp.close()
    ET.ElementTree(new_root).write(tmp_path, encoding="utf-8", xml_declaration=True)
    return tmp_path


def _normalize_generated_indentation(base: Path) -> None:
    for path in base.rglob("*"):
        if path.suffix not in {".h", ".cpp"}:
            continue
        try:
            text = path.read_text(encoding="utf-8")
        except UnicodeDecodeError:
            # Fall back to latin-1 which accepts all byte values
            text = path.read_text(encoding="latin-1")
        lines = text.splitlines(keepends=True)
        changed = False
        normalized = []

        for line in lines:
            stripped = line.lstrip(" ")
            leading_spaces = len(line) - len(stripped)
            tabs = leading_spaces // 4
            remainder = leading_spaces % 4
            new_line = ("\t" * tabs) + (" " * remainder) + stripped
            if new_line != line:
                changed = True
            normalized.append(new_line)

        if changed:
            try:
                path.write_text("".join(normalized), encoding="utf-8")
            except UnicodeEncodeError:
                # Fall back to latin-1 if UTF-8 encoding fails
                path.write_text("".join(normalized), encoding="latin-1")

# ============================================================
# CLI
# ============================================================
class SingleLineHelp(argparse.HelpFormatter):
    """Compact formatter: does not break lines, keeps everything aligned."""

    def _split_lines(self, text, width):
        return [text]

    def _format_action(self, action):
        parts = []

        # ex:   --flag, -f
        opts = ", ".join(action.option_strings)

        if action.metavar:
            opts = f"{opts} {action.metavar}"

        # nice standard fixed alignment
        parts.append(f"  {opts:<30}")

        # help text
        if action.help:
            parts.append(action.help)

        return "".join(parts) + "\n"

    def _format_action_invocation(self, action):
        if not action.option_strings:
            return super()._format_action_invocation(action)

        parts = ", ".join(action.option_strings)

        if action.metavar:
            parts += f" {action.metavar}"

        return parts

def parse_args():
    parser = argparse.ArgumentParser(
        prog="falcon-gen.py",
        description="FalconFIX-gen — offline C++ code generator for FIX XML",
        add_help=True,
        formatter_class=SingleLineHelp
    )

    # Required
    parser.add_argument("--xml-file",
                        dest="xml_file",
                        type=str,
                        help="FIX XML file")

    # Optional
    parser.add_argument("--output-dir",
                        dest="output_dir",
                        type=str,
                        default="output",
                        help="Base folder")

    parser.add_argument("--fix-version",
                        dest="fix_version",
                        type=str,
                        help="Override FIX version")

    parser.add_argument("--message-filter",
                        dest="message_filter",
                        type=str,
                        help="Generate only the named FIX message(s) and their dependencies (comma-separated)")


    # Templates (store_true → without metavar!)
    parser.add_argument("--string-size-template",
                        dest="generate_string_size_template",
                        action="store_true",
                        help="Generate string_sizes.yaml template")
    parser.add_argument("--samples-template",
                        dest="generate_samples_template",
                        action="store_true",
                        help="Generate samples.yaml template")

    return parser.parse_args()


# ============================================================
# MAIN
# ============================================================

def run_code(args):
    root_dir = Path(".")
    template_dir = root_dir / Path("templates")
    config_dir = root_dir / Path("config")
    template_string_size = config_dir / "string_sizes.yaml"
    template_samples = config_dir / "samples.yaml"

    xml_candidates = sorted(template_dir.glob("*.xml"))
    xml_candidate = Path(args.xml_file) if args.xml_file else (xml_candidates[0] if xml_candidates else None)


    # Handle template generation for samples.yaml
    if getattr(args, "generate_samples_template", False):
        if not args.xml_file:
            sys.exit("[error] --xml-file is required for --samples-template")
        from code.generator_samples import _load_samples_config, extract_fix_version_from_xml
        xml_path = Path(args.xml_file)
        if not xml_path.exists():
            sys.exit(f"[error] XML not found: {xml_path}")
        root = ET.parse(xml_path).getroot()
        fix_version = extract_fix_version_from_xml(root)
        _load_samples_config(template_samples, fix_version)
        print(f"[ok] samples.yaml template written to {template_samples}")
        return

    if not args.xml_file:
        sys.exit("[error] missing --xml-file")

    xml_path = Path(args.xml_file)
    if not xml_path.exists():
        sys.exit(f"[error] XML not found: {xml_path}")

    filtered_xml_path = None
    if args.message_filter:
        message_filters = [part.strip() for part in args.message_filter.split(",") if part.strip()]
        if not message_filters:
            sys.exit("[error] --message-filter was provided but no message names were parsed")
        filtered_xml_path = _filter_fix_xml(xml_path, message_filters)
        xml_path = filtered_xml_path
        print(f"[run] message filter active: {', '.join(message_filters)}")

    try:
        # ------------------------------------------------------------
        # Central (refactored) imports
        # ------------------------------------------------------------
        from code.common import (
        # strings functions
            parse_fix_version_from_xml,
            namespace_from_version,
            normalize_version_string,
            normalize_name,
            load_string_size_map,
        # paths functions
            ensure_base_include_structure,
            get_base_path,
            get_core_path,
            get_engine_path,
            get_utils_path,
            get_components_path,
            get_messages_path,
            get_samples_path,
        # writers functions
            emit_common_macros,
            emit_datetime_header,
        )

    # ------------------------------------------------------------
    # Templates mode
    # ------------------------------------------------------------
    #print(f"[ok] {config_dir}, {template_dir}, {template_file_default}")

        if args.generate_string_size_template:
            from code import generator_string_sizes
            template_file = template_dir
            if xml_candidate is None:
                sys.exit("[error] no XML template found under ./templates")
            generator_string_sizes.generate_or_validate_string_sizes(args.xml_file or xml_candidate,template_string_size)
            return

    # ------------------------------------------------------------
    # Parse FIX version (via common_strings)
    # ------------------------------------------------------------
        fix_version = parse_fix_version_from_xml(xml_path, override=args.fix_version)
        ns = namespace_from_version(fix_version)
        include_root = normalize_version_string(fix_version)

    # ------------------------------------------------------------
    # Criar estrutura de pastas padronizada
    # ------------------------------------------------------------
        base = Path(args.output_dir)
        ensure_base_include_structure(base,fix_version)
        base_include = get_base_path()

    # Nova estrutura: cpp/FIX4.4/messages/ e cpp/FIX4.4/components/ para .cpp files
        cpp_msg_dir = base / "cpp" / fix_version / "messages"
        cpp_msg_dir.mkdir(parents=True, exist_ok=True)
        cpp_comp_dir = base / "cpp" / fix_version / "components"
        cpp_comp_dir.mkdir(parents=True, exist_ok=True)

        core_dir      = get_core_path()
        utils_dir     = get_utils_path()
        components_dir= get_components_path()
        messages_dir  = get_messages_path()
        engine_dir    = get_engine_path()

        samples_dir   = get_samples_path()


    # ------------------------------------------------------------
    # Parse XML spec
    # ------------------------------------------------------------
        from code.xml_parser import parse as parse_xml
        spec = parse_xml(xml_path)
        _validate_spec_collisions(spec, normalize_name)

    # ------------------------------------------------------------
    # Main generators
    # ------------------------------------------------------------
        from code import generator_string_sizes
        from code import generator_types
        from code import generator_enums
        from code import generator_fields
        from code import generator_codec
        from code import generator_components
        from code import generator_components_cpp
        from code import generator_messages
        from code import generator_messages_cpp
        from code import generator_engine
        from code import generator_samples
        from code import generator_objects
        from code import generator_tests

    # generate string_sizes.yaml if missing
        generator_string_sizes.generate_or_validate_string_sizes(xml_path, template_string_size)

    # load global string sizes
        load_string_size_map(template_string_size)

    # ------------------------------------------------------------
    # START GENERATION
    # ------------------------------------------------------------
        emit_common_macros(utils_dir, ns)
        emit_datetime_header(utils_dir, ns, include_root)

        generator_types.generate(core_dir, spec)
        generator_enums.generate(core_dir, spec)
        generator_fields.generate(core_dir, spec)
        generator_codec.generate(core_dir, utils_dir, spec)
        generator_components.generate(components_dir, spec)
        generator_components_cpp.generate(cpp_comp_dir, spec)
        generator_messages.generate(messages_dir, spec)
        generator_messages_cpp.generate(cpp_msg_dir, spec)
        generator_engine.generate(engine_dir, spec)

    # In the .cpp-only pipeline, we no longer generate .inl files.
        for p in base.rglob("*.inl"):
            p.unlink(missing_ok=True)

    # Cleans legacy inline directories if they exist from previous runs.
        for stale_dir in ("parse", "reset", "serialize"):
            shutil.rmtree(base_include / stale_dir, ignore_errors=True)

    # Error codes are now API-owned under src/include/utils/error_codes.h
    # Keep generated core folder clean from stale legacy header.
        (core_dir / "error_codes.h").unlink(missing_ok=True)

        samples_config = config_dir / "samples.yaml"
        generator_samples.generate(samples_dir, samples_config, xml_path)
        generator_objects.generate(samples_dir, xml_path)
        generator_tests.generate(base, fix_version, samples_config)
        _normalize_generated_indentation(base)

        print(f"[ok] FalconFIX generated at: {base}")
    finally:
        if filtered_xml_path is not None:
            filtered_xml_path.unlink(missing_ok=True)


# ============================================================
# ENTRY POINT
# ============================================================

if __name__ == "__main__":
    try:
        args = parse_args()
        run_code(args)
    except Exception as e:
        print(f"[error] {e}")
        raise
