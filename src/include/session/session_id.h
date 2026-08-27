// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <cstdint>
#include <string>
#include <string_view>

#include <utils/fixed_string.h>

namespace falconfix {

class SessionID {
    using FixedString32 = FixedString<32>;

    FixedString32 m_beginString;
    FixedString32 m_senderCompID;
    FixedString32 m_targetCompID;
    uint64_t m_hash{0};

public:
    SessionID() = default;

    SessionID(std::string_view beginString,
              std::string_view senderCompID,
              std::string_view targetCompID) noexcept
        : m_beginString(beginString),
          m_senderCompID(senderCompID),
          m_targetCompID(targetCompID),
          m_hash(computeHash(m_beginString, m_senderCompID, m_targetCompID)) {
    }

    const FixedString32 &beginString() const noexcept { return m_beginString; }
    const FixedString32 &senderCompID() const noexcept { return m_senderCompID; }
    const FixedString32 &targetCompID() const noexcept { return m_targetCompID; }

    uint64_t hash64() const noexcept { return m_hash; }
    size_t hash() const noexcept { return static_cast<size_t>(m_hash); }

    bool empty() const noexcept {
        return m_beginString.empty() || m_senderCompID.empty() || m_targetCompID.empty();
    }

    friend bool operator==(const SessionID &lhs, const SessionID &rhs) noexcept {
        return lhs.m_hash == rhs.m_hash
            && lhs.m_beginString == rhs.m_beginString
            && lhs.m_senderCompID == rhs.m_senderCompID
            && lhs.m_targetCompID == rhs.m_targetCompID;
    }

    friend std::string toString(const SessionID &sid) {
        std::string out;
        out.reserve(
            sid.beginString().size() +
            sid.senderCompID().size() +
            sid.targetCompID().size() + 4
        );

        out.append(sid.beginString().data(), sid.beginString().size());
        out.push_back(':');
        out.append(sid.senderCompID().data(), sid.senderCompID().size());
        out.append("->");
        out.append(sid.targetCompID().data(), sid.targetCompID().size());
        return out;
    }

    std::string toString() const {
        std::string out;
        out.reserve(
            m_beginString.size() +
            m_senderCompID.size() +
            m_targetCompID.size() + 4
        );

        out.append(m_beginString.data(), m_beginString.size());
        out.push_back(':');
        out.append(m_senderCompID.data(), m_senderCompID.size());
        out.append("->");
        out.append(m_targetCompID.data(), m_targetCompID.size());
        return out;
    }

private:
    static uint64_t fnv1a64_update(uint64_t h, const FixedString32 &s) noexcept {
        for (size_t i = 0; i < s.size(); ++i) {
            h ^= static_cast<uint8_t>(s[i]);
            h *= 1099511628211ULL;
        }
        return h;
    }

    static uint64_t computeHash(const FixedString32 &begin,
                                const FixedString32 &sender,
                                const FixedString32 &target) noexcept {
        uint64_t h = 1469598103934665603ULL;
        h = fnv1a64_update(h, begin);
        h ^= '|'; h *= 1099511628211ULL;
        h = fnv1a64_update(h, sender);
        h ^= '|'; h *= 1099511628211ULL;
        h = fnv1a64_update(h, target);
        return h;
    }
};

} // namespace falconfix

namespace std {

template<>
struct hash<falconfix::SessionID> {
    size_t operator()(const falconfix::SessionID &sid) const noexcept {
        return static_cast<size_t>(sid.hash());
    }
};

} // namespace std
