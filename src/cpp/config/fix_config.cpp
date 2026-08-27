// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <config/fix_config.h>

#include <fstream>
#include <iterator>
#include <sstream>

namespace falconfix {

namespace {

std::string make_parse_error(std::size_t lineNo, const std::string &message) {
    std::ostringstream oss;
    oss << "FIXConfig parse error at line " << lineNo << ": " << message;
    return oss.str();
}

} // namespace

void FIXConfig::trim(std::string_view &sv) noexcept {
    while (!sv.empty() && std::isspace(static_cast<unsigned char>(sv.front()))) {
        sv.remove_prefix(1);
    }

    while (!sv.empty() && std::isspace(static_cast<unsigned char>(sv.back()))) {
        sv.remove_suffix(1);
    }
}

bool FIXConfig::iequals(std::string_view lhs, std::string_view rhs) noexcept {
    if (lhs.size() != rhs.size()) return false;

    for (std::size_t i = 0; i < lhs.size(); ++i) {
        const char a = static_cast<char>(std::toupper(static_cast<unsigned char>(lhs[i])));
        const char b = static_cast<char>(std::toupper(static_cast<unsigned char>(rhs[i])));
        if (a != b) return false;
    }

    return true;
}

FIXConfig FIXConfig::parse(std::string_view text) {
    FIXConfig cfg;
    Section *current = nullptr;

    std::size_t pos = 0;
    std::size_t lineNo = 0;

    while (pos <= text.size()) {
        ++lineNo;

        std::size_t end = text.find('\n', pos);
        if (end == std::string_view::npos) {
            end = text.size();
        }

        auto line = text.substr(pos, end - pos);
        pos = (end == text.size()) ? text.size() + 1 : end + 1;

        if (!line.empty() && line.back() == '\r') {
            line.remove_suffix(1);
        }

        const std::size_t commentPos = line.find_first_of("#;");
        if (commentPos != std::string_view::npos) {
            line = line.substr(0, commentPos);
        }

        trim(line);

        if (line.empty()) {
            if (end == text.size()) break;
            continue;
        }

        if (line.front() == '[') {
            if (line.back() != ']') {
                throw ParseError(make_parse_error(lineNo, "unterminated section header"));
            }

            auto name = line.substr(1, line.size() - 2);
            trim(name);

            if (name.empty()) {
                throw ParseError(make_parse_error(lineNo, "empty section name"));
            }

            if (iequals(name, "DEFAULT")) {
                current = &cfg.m_defaults;
            }
            else if (iequals(name, "SESSION")) {
                cfg.m_sections.emplace_back();
                current = &cfg.m_sections.back();
            }
            else {
                throw ParseError(make_parse_error(lineNo, "unsupported section: " + std::string(name)));
            }

            if (end == text.size()) break;
            continue;
        }

        const std::size_t eq = line.find('=');
        if (eq == std::string_view::npos) {
            throw ParseError(make_parse_error(lineNo, "expected key=value"));
        }

        auto key = line.substr(0, eq);
        auto val = line.substr(eq + 1);

        trim(key);
        trim(val);

        if (key.empty()) {
            throw ParseError(make_parse_error(lineNo, "empty key"));
        }

        if (!current) {
            throw ParseError(make_parse_error(lineNo, "key/value outside section"));
        }

        (*current)[std::string(key)] = std::string(val);

        if (end == text.size()) break;
    }

    return cfg;
}

FIXConfig FIXConfig::parseFile(std::string_view filePath) {
    std::ifstream in{std::string(filePath), std::ios::binary};
    if (!in) throw ParseError("FIXConfig: unable to open file: " + std::string(filePath));

    std::string text{std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>()};
    return parse(text);
}

const std::string &FIXConfig::get(const Section &s, std::string_view key) {
    auto it = s.find(key);
    if (it == s.end()) {
        throw std::runtime_error("FIXConfig: key not found: " + std::string(key));
    }

    return it->second;
}

bool FIXConfig::tryGetResolved(std::size_t index, std::string_view key, std::string_view &out) const noexcept {
    if (index >= m_sections.size()) {
        return false;
    }

    const auto &s = m_sections[index];

    auto it = s.find(key);
    if (it != s.end()) {
        out = it->second;
        return true;
    }

    auto itd = m_defaults.find(key);
    if (itd != m_defaults.end()) {
        out = itd->second;
        return true;
    }

    return false;
}

std::string_view FIXConfig::getResolved(std::size_t index, std::string_view key) const {
    if (index >= m_sections.size()) {
        throw std::runtime_error("FIXConfig: invalid section index");
    }

    const auto &s = m_sections[index];

    auto it = s.find(key);
    if (it != s.end()) {
        return it->second;
    }

    auto itd = m_defaults.find(key);
    if (itd != m_defaults.end()) {
        return itd->second;
    }

    throw std::runtime_error("FIXConfig: key not found resolved: " + std::string(key));
}

} // namespace falconfix
