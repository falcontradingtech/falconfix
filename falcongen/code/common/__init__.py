# -*- coding: utf-8 -*-

# SPDX-License-Identifier: MIT
# Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo

"""
common - FIX Code Generation Common Utilities

Refactored modules for better organization and without globals:
  - types: FIX type classification
  - paths: Gerenciamento de caminhos
  - formatters: Code formatting (dates, indentation, etc)
  - writers: Escrita de headers C++
  - strings: String manipulation utilities
"""

# ===== TYPES =====
from .fix_types import (
    PRIMITIVE_INT,
    PRIMITIVE_FLOAT,
    PRIMITIVE_CHAR,
    PRIMITIVE_BOOLEAN,
    STRING_LIKE_TYPES,
    INT_LIKE_TYPES,
    FLOAT_LIKE_TYPES,
    TEMPORAL_TYPES,
    TEMPORAL_DATE_TYPES,
    TEMPORAL_TIME_TYPES,
    TEMPORAL_TIMESTAMP_TYPES,
    classify_fix_type,
    is_string_like,
    resolve_cpp_type,
    max_value_encode_bytes,
)

# ===== PATHS =====
from .paths import (
    ensure_base_include_structure,
    get_base_path,
    get_core_path,
    get_components_path,
    get_engine_path,
    get_messages_path,
    get_serialize_path,
    get_parse_path,
    get_reset_path,
    get_samples_path,
    get_session_path,
    get_proto_path,
    get_utils_path,
    namespace_from_spec,
)

# ===== FORMATTERS =====
from .formatters import (
    format_current_datetime,
    format_date_header,
    format_cpp_comment,
    format_cpp_multiline_comment,
    indent,
    indent_lines,
    align,
    utc_now_iso,
)

# ===== WRITERS =====
from .writers import (
    open_generated_file,
    write_autogen_header,
    write_generated_line_header,
    write_namespace_open,
    write_namespace_close,
    emit_common_macros,
    emit_datetime_header,
    write_include_guard_open,
    write_include_guard_close,
    write_include,
)

# ===== STRINGS =====
from .strings import (
    normalize_name,
    to_upper,
    to_lower,
    parse_fix_version_from_xml,
    normalize_version_string,
    namespace_from_version,
    short_version,
    load_string_size_map,
    get_string_size,
)

__all__ = [
    # Types
    "PRIMITIVE_INT",
    "PRIMITIVE_FLOAT",
    "PRIMITIVE_CHAR",
    "PRIMITIVE_BOOLEAN",
    "STRING_LIKE_TYPES",
    "INT_LIKE_TYPES",
    "FLOAT_LIKE_TYPES",
    "TEMPORAL_TYPES",
    "classify_fix_type",
    "is_string_like",
    "resolve_cpp_type",
    "max_value_encode_bytes",
    # Paths
    "ensure_base_include_structure",
    "get_core_path",
    "get_components_path",
    "get_engine_path",
    "get_messages_path",
    "get_serialize_path",
    "get_parse_path",
    "get_reset_path",
    "get_samples_path",
    "get_session_path",
    "get_proto_path",
    "get_utils_path",
    "namespace_from_spec",
    # Formatters
    "format_current_datetime",
    "format_date_header",
    "format_cpp_comment",
    "format_cpp_multiline_comment",
    "indent",
    "indent_lines",
    "align",
    "utc_now_iso",
    # Writers
    "open_generated_file",
    "write_autogen_header",
    "write_generated_line_header",
    "write_namespace_open",
    "write_namespace_close",
    "emit_common_macros",
    "emit_datetime_header",
    "write_include_guard_open",
    "write_include_guard_close",
    "write_include",
    # Strings
    "normalize_name",
    "to_upper",
    "to_lower",
    "parse_fix_version_from_xml",
    "normalize_version_string",
    "namespace_from_version",
    "short_version",
    "load_string_size_map",
    "get_string_size",
]
