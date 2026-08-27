# -*- coding: utf-8 -*-

# SPDX-License-Identifier: MIT
# Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo

"""
formatters.py - Formatting Functions for Code Generation

Functions for formatting dates, versions, code strings, etc.
"""

from datetime import datetime
from typing import Optional


def format_current_datetime() -> str:
    """
    Returns the current timestamp in ISO format.

    Example:
        "2024-12-19T14:30:45.123456"
    """
    now = datetime.utcnow()
    return now.isoformat()


def format_date_header() -> str:
    """
    Returns a formatted date/time string for header comments.

    Example:
        "2024-12-19 14:30:45"
    """
    now = datetime.utcnow()
    return now.strftime("%Y-%m-%d %H:%M:%S")


def format_cpp_comment(text: str, indent: int = 0) -> str:
    """
    Formats text as a C++ comment (// style).

    Args:
        text: Text to comment
        indent: Number of indentation spaces

    Example:
        "Generated code" -> "// Generated code"
    """
    prefix = " " * indent
    return f"{prefix}// {text}"


def format_cpp_multiline_comment(text: str, indent: int = 0) -> str:
    """
    Formats text as a multi-line C++ comment (/* */ style).

    Args:
        text: Text to comment
        indent: Number of indentation spaces

    Example:
        "Auto-generated" ->
            /*
             * Auto-generated
             */
    """
    prefix = " " * indent
    lines = text.split("\n")

    result = [f"{prefix}/*"]
    for line in lines:
        result.append(f"{prefix} * {line}")
    result.append(f"{prefix} */")

    return "\n".join(result)


def indent(text: str, spaces: int = 4) -> str:
    """
    Adds indentation to all lines.

    Args:
        text: Text to indent
        spaces: Number of spaces

    Example:
        "line1\nline2" -> "    line1\n    line2"
    """
    prefix = " " * spaces
    lines = text.split("\n")
    return "\n".join(f"{prefix}{line}" if line.strip() else line for line in lines)


def indent_lines(lines: list[str], spaces: int = 4) -> list[str]:
    """
    Adds indentation to a list of lines.

    Args:
        lines: List of strings
        spaces: Number of spaces

    Returns:
        List with indented lines
    """
    prefix = " " * spaces
    return [f"{prefix}{line}" if line.strip() else line for line in lines]


def align(name: str, width: int = 32) -> str:
    """
    String alignment for columns.

    Args:
        name: String to align
        width: Minimum width

    Example:
        "MyVar" with width=20 -> "MyVar               "
    """
    return f"{name:<{width}}"


def utc_now_iso() -> str:
    """
    Returns the current UTC timestamp in ISO format with microseconds.

    Example:
        "2024-12-19T14:30:45.123456Z"
    """
    return datetime.utcnow().isoformat(timespec="microseconds") + "Z"
