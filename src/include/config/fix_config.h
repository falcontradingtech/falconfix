// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <charconv>
#include <cctype>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <vector>

namespace falconfix {

struct TransparentStringHash {
    using is_transparent = void;

    std::size_t operator()(std::string_view sv) const noexcept {
        return std::hash<std::string_view>{}(sv);
    }

    std::size_t operator()(const std::string &s) const noexcept {
        return std::hash<std::string_view>{}(s);
    }

    std::size_t operator()(const char *s) const noexcept {
        return std::hash<std::string_view>{}(s);
    }
};

struct TransparentStringEqual {
    using is_transparent = void;

    bool operator()(std::string_view lhs,
                    std::string_view rhs) const noexcept {
        return lhs == rhs;
    }
};

class FIXConfig {
    template <typename>
    struct always_false : std::false_type {};

    static void trim(std::string_view &sv) noexcept;
    static bool iequals(std::string_view lhs, std::string_view rhs) noexcept;

public:
    using Section = std::unordered_map<
        std::string,
        std::string,
        TransparentStringHash,
        TransparentStringEqual
    >;

    struct ParseError : public std::runtime_error {
        explicit ParseError(const std::string &msg) : std::runtime_error(msg) {}
    };

private:
    Section m_defaults;
    std::vector<Section> m_sections;

public:
    static FIXConfig parse(std::string_view text);
    static FIXConfig parseFile(std::string_view filePath);

    const Section &defaults() const noexcept { return m_defaults; }
    std::size_t sectionCount() const noexcept { return m_sections.size(); }
    const Section &section(std::size_t index) const { return m_sections.at(index); }

    static bool has(const Section &s, std::string_view key) noexcept { return s.find(key) != s.end(); }
    static const std::string &get(const Section &s, std::string_view key);

    bool tryGetResolved(std::size_t index, std::string_view key, std::string_view &out) const noexcept;
    std::string_view getResolved(std::size_t index, std::string_view key) const;

    template <typename T>
    bool tryGetAs(std::size_t index, std::string_view key, T &out) const noexcept {
        std::string_view v;
        if (!tryGetResolved(index, key, v)) {
            return false;
        }

        if constexpr (std::is_same_v<T, int32_t>) {
            const char *begin = v.data();
            const char *end = v.data() + v.size();
            const auto res = std::from_chars(begin, end, out);
            return res.ec == std::errc{} && res.ptr == end;
        }
        else if constexpr (std::is_same_v<T, uint16_t>) {
            uint32_t tmp = 0;
            const char *begin = v.data();
            const char *end = v.data() + v.size();
            const auto res = std::from_chars(begin, end, tmp);
            if (res.ec != std::errc{} || res.ptr != end || tmp > UINT16_MAX) {
                return false;
            }
            out = static_cast<uint16_t>(tmp);
            return true;
        }
        else if constexpr (std::is_same_v<T, bool>) {
            if (iequals(v, "Y") || iequals(v, "YES") || iequals(v, "TRUE") || v == "1") {
                out = true;
                return true;
            }
            if (iequals(v, "N") || iequals(v, "NO") || iequals(v, "FALSE") || v == "0") {
                out = false;
                return true;
            }
            return false;
        }
        else if constexpr (std::is_same_v<T, std::string>) {
            out.assign(v.data(), v.size());
            return true;
        }
        else {
            static_assert(always_false<T>::value, "FIXConfig::tryGetAs unsupported type");
        }
    }

    template <typename T>
    T getAs(std::size_t index, std::string_view key) const {
        T out{};
        if (!tryGetAs<T>(index, key, out)) {
            throw std::runtime_error("FIXConfig: invalid or missing value: " + std::string(key));
        }
        return out;
    }

    template <typename T>
    T getAsOr(std::size_t index, std::string_view key, const T &fallback) const noexcept {
        T out{};
        if (!tryGetAs<T>(index, key, out)) {
            return fallback;
        }
        return out;
    }
};

} // namespace falconfix
