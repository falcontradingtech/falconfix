// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <unordered_map>

#include <session/session_base.h>
#include <session/session_id.h>

namespace falconfix {

class SessionRegistry {
    std::unordered_map<uint64_t, SessionBase *> m_sessions;

public:
    void add(SessionBase *session) {
        m_sessions.emplace(session->sessionID().hash64(), session);
    }

    SessionBase *find(const SessionID &sid) noexcept {
        auto it = m_sessions.find(sid.hash64());
        if (it == m_sessions.end()) return nullptr;

        SessionBase *s = it->second;
        if (!(s->sessionID() == sid)) return nullptr;

        return s;
    }
};

} // namespace falconfix
