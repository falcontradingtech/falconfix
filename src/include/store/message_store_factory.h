// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <memory>


#include <session/session_settings.h>
#include <store/message_store.h>
#include <store/memory_message_store.h>
#include <store/null_message_store.h>
#include <store/file_message_store.h>

namespace falconfix {

class MessageStoreFactory {
public:
    static std::unique_ptr<MessageStore> create(const SessionConfig &config) {
        switch (config.storage.storeType) {
        case StoreType::None:
            return std::make_unique<NullMessageStore>();

        case StoreType::Memory:
            return std::make_unique<MemoryMessageStore>(config.storage.persistMessages);

        case StoreType::File:
            return std::make_unique<FileMessageStore>(config.storage.path, config.sessionID, config.storage.persistMessages);

        default:
            return std::make_unique<NullMessageStore>();
        }
    }
};

} // namespace falconfix
