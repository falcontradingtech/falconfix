// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <../support/bench_protocols.h>
#include <../support/bench_runtime_support.h>

#include <session/session_id.h>
#include <store/file_message_store.h>
#include <store/memory_message_store.h>
#include <store/null_message_store.h>

#include <cstddef>
#include <string_view>
#include <vector>

namespace falconfix::bench {

inline falconfix::SessionID makeBenchSessionID(std::string_view senderCompID, std::string_view targetCompID) {
    return falconfix::SessionID{DefaultProtocolTraits::beginString, senderCompID, targetCompID};
}

inline std::vector<falconfix::StoredMessageView> makeOutgoingWindowBuffer(std::size_t window) {
    std::vector<falconfix::StoredMessageView> out;
    out.reserve(window);
    return out;
}

class FileStoreFixture {
    ScopedTempDir m_root;
    falconfix::SessionID m_sid;
    falconfix::FileMessageStore m_store;

public:
    FileStoreFixture(std::string_view suffix, std::string_view senderCompID, std::string_view targetCompID)
        : m_root(std::string("falconfix_store_bench_") + std::string(suffix)),
          m_sid(makeBenchSessionID(senderCompID, targetCompID)),
          m_store(m_root.path().string(), m_sid) {
    }

    falconfix::FileMessageStore &store() noexcept {
        return m_store;
    }
};

} // namespace falconfix::bench
