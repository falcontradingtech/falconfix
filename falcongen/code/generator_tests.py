# -*- coding: utf-8 -*-

# SPDX-License-Identifier: MIT
# Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo

"""
generator_tests.py
-------------------------------------------------
Generates C++ round-trip tests for each FIX version in:
    output/tests/<FIX_VERSION>/roundtrip.cpp

Data source:
    output/include/<FIX_VERSION>/samples/fix_messages.h
"""

from __future__ import annotations

from pathlib import Path
import re

from code.common import normalize_version_string


def _namespace_from_fix_version(fix_version: str) -> str:
    compact = fix_version.upper().replace("FIX", "").replace(".", "")
    return f"fix{compact.lower()}"


def _suite_name_from_fix_version(fix_version: str) -> str:
    safe = re.sub(r"[^A-Za-z0-9]+", "_", fix_version)
    return f"RoundTrip_{safe}"


def _parse_sample_names(samples_header: Path) -> list[str]:
    content = samples_header.read_text(encoding="utf-8")
    names = re.findall(r'SamplePair\{"([A-Za-z0-9_]+)"\s*,', content)

    seen = set()
    out = []
    for name in names:
        if name in seen:
            continue
        seen.add(name)
        out.append(name)

    return out


def _load_group_repetitions(samples_config: Path) -> int:
    content = samples_config.read_text(encoding="utf-8")
    m = re.search(r"^\s*group_repetitions\s*:\s*([0-9]+)\s*$", content, flags=re.MULTILINE)
    if not m:
        raise RuntimeError(f"group_repetitions not found in: {samples_config}")
    reps = int(m.group(1))
    if reps < 1:
        raise RuntimeError(f"group_repetitions must be >= 1 (got {reps})")
    return reps


def _render_roundtrip_cpp(fix_version: str, namespace: str, suite_name: str, message_names: list[str]) -> str:
    include_root = normalize_version_string(fix_version)
    lines: list[str] = []
    lines.append("// Auto-generated roundtrip tests. Do not edit by hand.")
    lines.append(f"// FIX version: {fix_version}")
    lines.append("")
    lines.append("#include <gtest/gtest.h>")
    lines.append("")
    lines.append("#include <string_view>")
    lines.append("#include <type_traits>")
    lines.append("#include <utility>")
    lines.append("")
    lines.append(f"#include <{include_root}/utils/all_messages.h>")
    lines.append(f"#include <{include_root}/samples/fix_messages.h>")
    lines.append("")
    lines.append("#include <utils/error_codes.h>")
    lines.append("#include <utils/fast_buffer.h>")
    lines.append("")
    lines.append("template <typename MsgT>")
    lines.append("inline void runRoundTripFor() {}")
    lines.append("")
    lines.append("namespace {")
    lines.append("")
    lines.append("template<typename T, typename = void>")
    lines.append("struct has_set_string : std::false_type {};")
    lines.append("")
    lines.append("template<typename T>")
    lines.append("struct has_set_string<T, std::void_t<decltype(std::declval<T&>().setString(std::declval<std::string_view>()))>> : std::true_type {};")
    lines.append("")
    lines.append("template<typename T, typename = void>")
    lines.append("struct has_to_string : std::false_type {};")
    lines.append("")
    lines.append("template<typename T>")
    lines.append("struct has_to_string<T, std::void_t<decltype(std::declval<const T&>().toString(std::declval<falconfix::FastBuffer&>()))>> : std::true_type {};")
    lines.append("")
    lines.append("template<typename MsgT>")
    lines.append("void runRoundTripFor(std::string_view sample_name) {")
    lines.append("    if constexpr (!has_set_string<MsgT>::value || !has_to_string<MsgT>::value) {")
    lines.append("        GTEST_SKIP() << \"roundtrip unsupported for this message API\";")
    lines.append("    }")
    lines.append("    else {")
    lines.append(f"    const std::string_view fixraw = {namespace}::samples::map_samples[sample_name];")
    lines.append("    ASSERT_FALSE(fixraw.empty()) << \"missing sample for \" << sample_name;")
    lines.append("")
    lines.append("    MsgT msg;")
    lines.append("    const auto decode_rc = msg.setString(fixraw);")
    lines.append("    ASSERT_TRUE(decode_rc.ok())")
    lines.append("        << \"decode failed err=\" << decode_rc.code")
    lines.append("        << \" (\" << falconfix::errors::error_message(decode_rc) << \")\";")
    lines.append("")
    lines.append("    falconfix::FastBuffer outbuf;")
    lines.append("    const auto encode_rc = msg.toString(outbuf);")
    lines.append("    ASSERT_TRUE(encode_rc.ok())")
    lines.append("        << \"encode failed err=\" << encode_rc.code")
    lines.append("        << \" (\" << falconfix::errors::error_message(encode_rc) << \")\";")
    lines.append("    ASSERT_EQ(outbuf.view(), fixraw) << \"Roundtrip mismatch\";")
    lines.append("    }")
    lines.append("}")
    lines.append("")
    lines.append("")
    lines.append("} // namespace")
    lines.append("")
    for message in message_names:
        lines.append(f"TEST_F({suite_name}, {message}_MatchesGeneratedSample) {{")
        lines.append(f"    runRoundTripFor<{namespace}::messages::{message}>(\"{message}\");")
        lines.append("}")
        lines.append("")
    return "\n".join(lines)


def _render_samples_validation_cpp(
    fix_version: str,
    namespace: str,
    suite_name: str,
    message_names: list[str],
    group_repetitions: int,
) -> str:
    include_root = normalize_version_string(fix_version)
    has_mdr = "MarketDataRequest" in set(message_names)

    lines: list[str] = []
    lines.append("// Auto-generated samples validation tests. Do not edit by hand.")
    lines.append(f"// FIX version: {fix_version}")
    lines.append("")
    lines.append("#include <gtest/gtest.h>")
    lines.append("#include <cstddef>")
    lines.append("#include <memory>")
    lines.append("#include <string>")
    lines.append("#include <string_view>")
    lines.append("#include <type_traits>")
    lines.append("")
    lines.append(f"#include <{include_root}/samples/fix_messages.h>")
    if has_mdr:
        lines.append(f"#include <{include_root}/messages/MarketDataRequest.h>")
    lines.append("")
    lines.append(f"using namespace {namespace};")
    lines.append("")
    lines.append("namespace {")
    lines.append("")
    lines.append("bool containsField(std::string_view msg, int tag, std::string_view value) {")
    lines.append("    const std::string needle = std::to_string(tag) + \"=\" + std::string(value) + \"\\x01\";")
    lines.append("    return msg.find(needle) != std::string_view::npos;")
    lines.append("}")
    lines.append("")
    lines.append("std::size_t countTag(std::string_view msg, int tag) {")
    lines.append("    const std::string needle = std::to_string(tag) + \"=\";")
    lines.append("    std::size_t count = 0;")
    lines.append("    std::size_t pos = 0;")
    lines.append("    while ((pos = msg.find(needle, pos)) != std::string_view::npos) {")
    lines.append("        if (pos == 0 || msg[pos - 1] == '\\x01') {")
    lines.append("            ++count;")
    lines.append("        }")
    lines.append("        pos += needle.size();")
    lines.append("    }")
    lines.append("    return count;")
    lines.append("}")
    lines.append("")
    lines.append("std::string_view sampleByName(std::string_view name) {")
    lines.append(f"    for (const auto &sample : {namespace}::samples::map_samples_data) {{")
    lines.append("        if (sample.first == name) {")
    lines.append("            return sample.second;")
    lines.append("        }")
    lines.append("    }")
    lines.append("    return {};")
    lines.append("}")
    lines.append("")
    lines.append("} // namespace")
    lines.append("")
    lines.append(f"TEST_F({suite_name}, FixSamplesHaveBasicFixFields) {{")
    lines.append(f"    ASSERT_GT(std::size({namespace}::samples::map_samples_data), 0u);")
    lines.append(f"    for (const auto &sample : {namespace}::samples::map_samples_data) {{")
    lines.append("        const auto name = sample.first;")
    lines.append("        const auto msg = sample.second;")
    lines.append("        EXPECT_TRUE(containsField(msg, 8, \"FIX4.4\")) << \"sample=\" << name;")
    lines.append("        EXPECT_EQ(countTag(msg, 35), 1u) << \"sample=\" << name;")
    lines.append("        EXPECT_EQ(countTag(msg, 10), 1u) << \"sample=\" << name;")
    lines.append("    }")
    lines.append("}")
    lines.append("")
    if has_mdr:
        lines.append(f"TEST_F({suite_name}, MarketDataRequestRespectsConfiguredGroupRepetitionsInFixString) {{")
        lines.append(f"    constexpr int reps = {group_repetitions};")
        lines.append("    const auto mdr = sampleByName(\"MarketDataRequest\");")
        lines.append("    ASSERT_FALSE(mdr.empty()) << \"MarketDataRequest sample not found\";")
        lines.append("    EXPECT_TRUE(containsField(mdr, 267, std::to_string(reps))) << \"NoMDEntryTypes count mismatch\";")
        lines.append("    EXPECT_TRUE(containsField(mdr, 146, std::to_string(reps))) << \"NoRelatedSym count mismatch\";")
        lines.append("    EXPECT_EQ(countTag(mdr, 269), static_cast<std::size_t>(reps)) << \"MDEntryType entries mismatch\";")
        lines.append("}")
        lines.append("")
        lines.append(f"TEST_F({suite_name}, MarketDataRequestConstructedObjectMatchesConfiguredGroupRepetitions) {{")
        lines.append(f"    constexpr int reps = {group_repetitions};")
        lines.append(f"    auto obj = std::make_unique<{namespace}::messages::MarketDataRequest>();")
        lines.append("    obj->setMDReqID(\"REQID_1\");")
        lines.append("    obj->setSubscriptionRequestType('1');")
        lines.append("    obj->setMarketDepth(1);")
        lines.append("    auto &md_types = obj->getRefMDReqGrp();")
        lines.append("    for (int i = 0; i < reps; ++i) {")
        lines.append("        using MDTypesT = std::remove_reference_t<decltype(md_types)>;")
        lines.append("        typename MDTypesT::Entry *item = nullptr;")
        lines.append("        const auto rc = md_types.add(item);")
        lines.append("        ASSERT_EQ(rc, falconfix::errors::code::SUCCESS);")
        lines.append("        ASSERT_NE(item, nullptr);")
        lines.append("        item->setMDEntryType('1');")
        lines.append("    }")
        lines.append("    obj->setNoMDEntryTypesPresent();")
        lines.append("    auto &symbols = obj->getRefInstrmtMDReqGrp();")
        lines.append("    for (int i = 0; i < reps; ++i) {")
        lines.append("        using SymbolsT = std::remove_reference_t<decltype(symbols)>;")
        lines.append("        typename SymbolsT::Entry *item = nullptr;")
        lines.append("        const auto rc = symbols.add(item);")
        lines.append("        ASSERT_EQ(rc, falconfix::errors::code::SUCCESS);")
        lines.append("        ASSERT_NE(item, nullptr);")
        lines.append("        item->getRefInstrument().setSymbol(\"VAL_55\");")
        lines.append("    }")
        lines.append("    obj->setNoRelatedSymPresent();")
        lines.append("    EXPECT_EQ(obj->getRefMDReqGrp().size(), static_cast<std::size_t>(reps));")
        lines.append("    EXPECT_EQ(obj->getRefInstrmtMDReqGrp().size(), static_cast<std::size_t>(reps));")
        lines.append("    EXPECT_TRUE(obj->hasMDReqID());")
        lines.append("    EXPECT_TRUE(obj->hasSubscriptionRequestType());")
        lines.append("    EXPECT_TRUE(obj->hasMarketDepth());")
        lines.append("}")
        lines.append("")
    else:
        lines.append(f"TEST_F({suite_name}, MarketDataRequestNotPresentInThisFixVersion) {{")
        lines.append("    GTEST_SKIP() << \"MarketDataRequest sample not generated for this FIX version\";")
        lines.append("}")
        lines.append("")
    return "\n".join(lines)


def generate(base_output: Path, fix_version: str, samples_config: Path) -> list[Path]:
    samples_header = base_output / "include" / fix_version / "samples" / "fix_messages.h"
    if not samples_header.exists():
        raise RuntimeError(f"Missing samples header for test generation: {samples_header}")

    message_names = _parse_sample_names(samples_header)
    if not message_names:
        raise RuntimeError(f"No sample entries found in: {samples_header}")

    namespace = _namespace_from_fix_version(fix_version)
    roundtrip_suite_name = _suite_name_from_fix_version(fix_version)

    tests_dir = base_output / "tests" / fix_version
    tests_dir.mkdir(parents=True, exist_ok=True)

    roundtrip_path = tests_dir / "roundtrip.cpp"
    roundtrip_content = _render_roundtrip_cpp(fix_version, namespace, roundtrip_suite_name, message_names) + "\n"
    roundtrip_path.write_text(roundtrip_content, encoding="utf-8")
    legacy_roundtrip_path = tests_dir / "roundtrip_generated.cpp"
    if legacy_roundtrip_path.exists():
        legacy_roundtrip_path.unlink()

    samples_path = tests_dir / "samples_generated.cpp"
    if samples_path.exists():
        samples_path.unlink()

    print(f"[ok] roundtrip tests written → {roundtrip_path} ({len(message_names)} cases)")
    print(f"[ok] removed point-validation test mirror → {samples_path}")
    return [roundtrip_path]
