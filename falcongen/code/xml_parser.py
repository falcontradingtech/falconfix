# -*- coding: utf-8 -*-

# SPDX-License-Identifier: MIT
# Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo

"""
xml_parser.py - Refactored FIX XML parser

This parser resolves critical issues from the previous implementation:
- Eliminates field duplication
- Preserves the hierarchical structure
- Extracts enumerations and their values
- Maintains type safety
- Remains compatible with the existing code generators
"""

from dataclasses import dataclass, field as dc_field
from pathlib import Path
from typing import Optional, Dict, List
import xml.etree.ElementTree as ET


# ===== DATACLASSES (Type Safety) =====

@dataclass
class FieldValue:
    """Enum value for CHAR/ENUM fields"""
    enum: str
    description: str


@dataclass
class FieldDefinition:
    """Central definition of a FIX field (unique, no duplication)"""
    number: int
    name: str
    type: str  # "STRING", "CHAR", "INT", etc
    values: List[FieldValue] = dc_field(default_factory=list)
    description: str = ""

    def __hash__(self):
        return hash(self.number)


@dataclass
class FieldUsage:
    """Uso de um campo em contexto (message, component, group)"""
    field_number: int
    required: bool
    field_name: str = ""  # Name cache for convenience

    def __repr__(self):
        req = "REQUIRED" if self.required else "optional"
        return f"{self.field_name}({self.field_number}) [{req}]"


@dataclass
class Group:
    """Repeating group inside a message or component."""
    number_field: str  # Example: "NoHops" -> group counter field number
    name: str
    fields: List[FieldUsage] = dc_field(default_factory=list)
    groups: List["Group"] = dc_field(default_factory=list)
    components: List[str] = dc_field(default_factory=list)  # Component references
    raw_children: List[dict] = dc_field(default_factory=list)


@dataclass
class Component:
    """Definition of reusable component"""
    name: str
    fields: List[FieldUsage] = dc_field(default_factory=list)
    groups: List[Group] = dc_field(default_factory=list)
    components: List[str] = dc_field(default_factory=list)  # References to subcomponents
    raw_children: List[dict] = dc_field(default_factory=list)


@dataclass
class Message:
    """Definition of FIX message"""
    name: str
    msgtype: str
    msgcat: str  # "admin" ou "app"
    fields: List[FieldUsage] = dc_field(default_factory=list)
    groups: List[Group] = dc_field(default_factory=list)
    components: List[str] = dc_field(default_factory=list)
    raw_children: List[dict] = dc_field(default_factory=list)


@dataclass
class FixSpec:
    """Complete FIX specification"""
    version: str  # ex: "4.4"
    fields: Dict[int, FieldDefinition]  # field_number -> definition
    header: Optional[Component]
    trailer: Optional[Component]
    messages: Dict[str, Message]  # message_name -> definition
    components: Dict[str, Component]  # component_name -> definition


class FixXmlParser:
    """Refactored parser with separation of concerns"""

    def __init__(self, xml_path: Path):
        self.xml_path = xml_path
        self.tree = ET.parse(xml_path)
        self.root = self.tree.getroot()
        self.fields_dict: Dict[int, FieldDefinition] = {}

    def parse(self) -> FixSpec:
        """Parse completo respeitando estrutura FIX"""

        # 1. Extract field definitions FIRST (source of truth)
        self._populate_fields_dict()

        # 2. Extract structures that use the fields
        header = self._parse_header()
        trailer = self._parse_trailer()
        messages = self._parse_messages()
        components = self._parse_components()

        return FixSpec(
            version=self._extract_version(),
            fields=self.fields_dict,
            header=header,
            trailer=trailer,
            messages=messages,
            components=components,
        )

    def _extract_version(self) -> str:
        """Extracts FIX version from root attribute"""
        major = self.root.attrib.get("major", "4")
        minor = self.root.attrib.get("minor", "4")
        return f"{major}.{minor}"

    def _populate_fields_dict(self):
        """
        Populates unique field dictionary a partir de <fields> section.

        This is the CENTRAL SOURCE OF TRUTH - each field appears only once here.
        """
        fields_elem = self.root.find(".//fields")
        if fields_elem is None:
            return

        for field_elem in fields_elem.findall("field"):
            field_def = self._parse_field_definition(field_elem)
            self.fields_dict[field_def.number] = field_def

    def _parse_field_definition(self, elem) -> FieldDefinition:
        """Parse <field> in <fields> section"""
        number = int(elem.attrib.get("number", 0))
        name = elem.attrib.get("name", "")
        field_type = elem.attrib.get("type", "STRING")

        # Extract enums/values if they exist
        values = []
        for value_elem in elem.findall("value"):
            values.append(FieldValue(
                enum=value_elem.attrib.get("enum", ""),
                description=value_elem.attrib.get("description", "")
            ))

        return FieldDefinition(
            number=number,
            name=name,
            type=field_type,
            values=values,
        )

    def _parse_field_usage(self, elem) -> FieldUsage:
        """
        Parse de <field> em contexto (message, component, group).
        REFERENCES the definition in fields_dict, no duplication.
        """
        name = elem.attrib.get("name", "")
        required = elem.attrib.get("required", "N") == "Y"

        # Look for field number in central definition
        field_number = 0
        for fd in self.fields_dict.values():
            if fd.name == name:
                field_number = fd.number
                break

        return FieldUsage(
            field_number=field_number,
            required=required,
            field_name=name,
        )

    def _parse_element_content(self, elem) -> tuple[List[FieldUsage], List[Group], List[str], List[dict]]:
        """
        Extracts content (fields, groups, component refs) de um elemento.
        Works for header, message, component - structure is the same!
        """
        fields = []
        groups = []
        component_refs = []
        raw_children = []
        seen_entries = set()

        for child in elem:
            tag = child.tag
            name = child.attrib.get("name", "")
            dedupe_key = (tag, name)

            if name and dedupe_key in seen_entries:
                continue

            if name:
                seen_entries.add(dedupe_key)

            if child.tag == "field":
                fields.append(self._parse_field_usage(child))
                raw_children.append({"tag": "field", "name": name})
            elif child.tag == "group":
                groups.append(self._parse_group(child))
                raw_children.append({"tag": "group", "name": name})
            elif child.tag == "component":
                component_refs.append(name)
                raw_children.append({"tag": "component", "name": name})

        return fields, groups, component_refs, raw_children

    def _parse_group(self, elem) -> Group:
        """Parse de <group> (estrutura repetitiva)"""
        name = elem.attrib.get("name", "")
        fields, groups, component_refs, raw_children = self._parse_element_content(elem)

        return Group(
            number_field=name,  # Ex: "NoHops"
            name=name,
            fields=fields,
            groups=groups,
            components=component_refs,
            raw_children=raw_children,
        )

    def _parse_header(self) -> Optional[Component]:
        """Parse de <header>"""
        header_elem = self.root.find(".//header")
        if header_elem is None:
            return None

        fields, groups, comps, raw_children = self._parse_element_content(header_elem)
        return Component(
            name="Header",
            fields=fields,
            groups=groups,
            components=comps,
            raw_children=raw_children,
        )

    def _parse_trailer(self) -> Optional[Component]:
        """Parse de <trailer>"""
        trailer_elem = self.root.find(".//trailer")
        if trailer_elem is None:
            return None

        fields, groups, comps, raw_children = self._parse_element_content(trailer_elem)
        return Component(
            name="Trailer",
            fields=fields,
            groups=groups,
            components=comps,
            raw_children=raw_children,
        )

    def _parse_messages(self) -> Dict[str, Message]:
        """Parse all <message> elements"""
        messages = {}
        messages_elem = self.root.find(".//messages")
        if messages_elem is None:
            return messages

        for msg_elem in messages_elem.findall("message"):
            msg_name = msg_elem.attrib.get("name", "")
            msg_type = msg_elem.attrib.get("msgtype", "")
            msg_cat = msg_elem.attrib.get("msgcat", "app")

            fields, groups, comps, raw_children = self._parse_element_content(msg_elem)

            messages[msg_name] = Message(
                name=msg_name,
                msgtype=msg_type,
                msgcat=msg_cat,
                fields=fields,
                groups=groups,
                components=comps,
                raw_children=raw_children,
            )

        return messages

    def _parse_components(self) -> Dict[str, Component]:
        """Parse all <component> elements"""
        components = {}
        components_elem = self.root.find(".//components")
        if components_elem is None:
            return components

        for comp_elem in components_elem.findall("component"):
            comp_name = comp_elem.attrib.get("name", "")
            fields, groups, comps, raw_children = self._parse_element_content(comp_elem)

            components[comp_name] = Component(
                name=comp_name,
                fields=fields,
                groups=groups,
                components=comps,
                raw_children=raw_children,
            )

        return components


# ===== COMPATIBILITY WITH GENERATOR CODE EXISTENTE =====

def parse(xml_path: Path) -> dict:
    """
    Compatibility function - returns dict like old parser.

    This allows using the new parser without changing generator code immediately.
    """
    parser = FixXmlParser(xml_path)
    spec = parser.parse()

    # Converter para formato antigo (dict)
    return {
        "version": spec.version,
        "fields": [
            {
                "name": fd.name,
                "number": fd.number,
                "type": fd.type,
                "values": [{"enum": v.enum, "description": v.description} for v in fd.values]
            }
            for fd in spec.fields.values()
        ],
        "header": _component_to_dict(spec.header) if spec.header else None,
        "trailer": _component_to_dict(spec.trailer) if spec.trailer else None,
        "messages": [
            {
                "name": msg.name,
                "msgtype": msg.msgtype,
                "msgcat": msg.msgcat,
                "fields": [{"name": fu.field_name, "required": "Y" if fu.required else "N"} for fu in msg.fields],
                "groups": [_group_to_dict(g) for g in msg.groups],
                "components": msg.components,
                "raw_children": msg.raw_children,
            }
            for msg in spec.messages.values()
        ],
        "components": [
            {
                "name": comp.name,
                "fields": [{"name": fu.field_name, "required": "Y" if fu.required else "N"} for fu in comp.fields],
                "groups": [_group_to_dict(g) for g in comp.groups],
                "components": comp.components,
                "raw_children": comp.raw_children,
            }
            for comp in spec.components.values()
        ]
    }


def _component_to_dict(comp: Component) -> dict:
    """Converte Component para dict (compatibilidade)"""
    return {
        "name": comp.name,
        "fields": [{"name": fu.field_name, "required": "Y" if fu.required else "N"} for fu in comp.fields],
        "groups": [_group_to_dict(g) for g in comp.groups],
        "components": comp.components,
        "raw_children": comp.raw_children,
    }


def _group_to_dict(group: Group) -> dict:
    """Converte Group para dict (compatibilidade)"""
    return {
        "name": group.name,
        "fields": [{"name": fu.field_name, "required": "Y" if fu.required else "N"} for fu in group.fields],
        "groups": [_group_to_dict(g) for g in group.groups],
        "components": group.components,
        "raw_children": group.raw_children,
    }


# ===== EXPORTS =====

__all__ = ['parse', 'FixXmlParser', 'FixSpec', 'FieldDefinition', 'FieldUsage', 'Message', 'Component', 'Group']
