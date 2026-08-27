# -*- coding: utf-8 -*-

# SPDX-License-Identifier: MIT
# Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo

"""
generator_types.py
------------------------------------
Generates core/types.h using ONLY global definitions vindas de:
  - common_strings: PRIMITIVE_INT, PRIMITIVE_FLOAT,
                    TEMPORAL_TYPES, STRING_LIKE_TYPES
  - common_code: write_autogen_header
  - common_paths:   namespace_from_spec
"""

from pathlib import Path

from code.common import (
    open_generated_file,
    write_autogen_header,
    align,
    namespace_from_spec,
    PRIMITIVE_INT,
    PRIMITIVE_FLOAT,
    TEMPORAL_TYPES,
    STRING_LIKE_TYPES,
)

# ------------------------------------------------------------
# Principal Generator 
# ------------------------------------------------------------

def generate(core_dir: Path, spec: dict):

    out_path = core_dir / "types.h"
    ns = namespace_from_spec(spec)

    INT_TYPES      = sorted(PRIMITIVE_INT)
    FLOAT_TYPES    = sorted(PRIMITIVE_FLOAT)
    TEMP_TYPES     = sorted(TEMPORAL_TYPES)
    STRING_TYPES   = sorted(STRING_LIKE_TYPES)

    with open_generated_file(out_path, encoding="utf-8") as fh:

        write_autogen_header(fh, "FalconFIX Base Types")

        fh.write("#pragma once\n\n")
        fh.write("#include <cstdint>\n")
        fh.write("#include <utils/fixed_string.h>\n\n")

        fh.write(f"namespace {ns} {{\n")
        fh.write("namespace types {\n\n")

        # ----------------------------------------------------
        # Boolean/Char
        # ----------------------------------------------------
        fh.write("// --------------------------------------------------------------\n")
        fh.write("// Boolean & Char\n")
        fh.write("// --------------------------------------------------------------\n")
        fh.write(f"using {align('BOOLEAN')} = bool;\n")
        fh.write(f"using {align('CHAR')} = char;\n\n")

        # ----------------------------------------------------
        # Integer types
        # ----------------------------------------------------
        fh.write("// --------------------------------------------------------------\n")
        fh.write("// Scalar integer aliases (64-bit)\n")
        fh.write("// --------------------------------------------------------------\n")
        for t in INT_TYPES:
            fh.write(f"using {align(t)} = int64_t;\n")
        fh.write("\n")

        # ----------------------------------------------------
        # Floating-point types
        # ----------------------------------------------------
        fh.write("// --------------------------------------------------------------\n")
        fh.write("// Floating-point aliases (double)\n")
        fh.write("// --------------------------------------------------------------\n")
        for t in FLOAT_TYPES:
            fh.write(f"using {align(t)} = double;\n")
        fh.write("\n")

        # ----------------------------------------------------
        # Temporal types
        # ----------------------------------------------------
        fh.write("// --------------------------------------------------------------\n")
        fh.write("// Temporal encodings — minimal, deterministic types\n")
        fh.write("// Representations:\n")
        fh.write("//   UTCTIMESTAMP  → milliseconds since Unix epoch (int64_t)\n")
        fh.write("//   UTCDATEONLY   → YYYYMMDD numeric (int32_t)\n")
        fh.write("//   UTCTIMEONLY   → HHMMSSmmm numeric (int32_t)\n")
        fh.write("//   LOCALMKTDATE  → YYYYMMDD numeric (int32_t)\n")
        fh.write("// --------------------------------------------------------------\n")
        for t in TEMP_TYPES:
            # regras:
            #  - Timestamp → int64
            #  - Date/TimeOnly → int32
            if t in {"UTCTIMESTAMP"}:
                ctype = "int64_t"
            elif t in {"UTCDATEONLY", "UTCTIMEONLY", "LOCALMKTDATE"}:
                ctype = "int32_t"
            else:
                ctype = "int64_t"

            fh.write(f"using {align(t)} = {ctype};\n")
        fh.write("\n")

        # ----------------------------------------------------
        # STRING types → FixedString<N>
        # ----------------------------------------------------
        fh.write("// --------------------------------------------------------------\n")
        fh.write("// FixedString-based textual types\n")
        fh.write("// --------------------------------------------------------------\n")
        for t in STRING_TYPES:
            fh.write(f"template <std::size_t N> using {align(t)} = falconfix::FixedString<N>;\n")

        fh.write("\n} // namespace types\n")
        fh.write(f"}} // namespace {ns}\n")

    print(f"[ok] types.h written → {out_path}")
