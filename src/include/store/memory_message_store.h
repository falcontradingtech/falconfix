// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <string>
#include <string_view>

#include <store/message_store_base.h>

namespace falconfix {

class MemoryMessageStore final : public MessageStoreBase {
public:
    explicit MemoryMessageStore(bool persistMessages = true) noexcept
        : MessageStoreBase(persistMessages) {}

    FFStatus storeOutgoing(int64_t seqNum,
                           std::string_view msgType,
                           bool admin,
                           std::string_view raw) noexcept override {
        if (!m_persistMessages)
            return FF_OK();

        try {
            m_outgoing.push_back({
                seqNum,
                std::string(msgType),
                admin,
                std::string(raw)
            });

            return FF_OK();
        }
        catch (...) {
            return FF_SYS_ERR(BAD_ALLOC);
        }
    }

    FFStatus storeIncoming(int64_t,
                           std::string_view) noexcept override {
        return FF_OK();
    }
};

} // namespace falconfix