// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

#include <utils/error_codes.h>

namespace falconfix {

struct StoredMessageView {
    int64_t seqNum{0};
    std::string_view msgType{};
    bool admin{false};
    std::string_view raw{};
};

class MessageStore {
public:
    virtual ~MessageStore() = default;

    virtual FFStatus storeOutgoing(int64_t seqNum,
                                   std::string_view msgType,
                                   bool admin,
                                   std::string_view raw) noexcept = 0;

    virtual FFStatus storeIncoming(int64_t seqNum,
                                   std::string_view raw) noexcept = 0;

    virtual FFStatus getOutgoing(int64_t beginSeqNum,
                                 int64_t endSeqNum,
                                 std::vector<StoredMessageView> &out) const noexcept = 0;

    virtual int64_t nextSenderSeqNum() const noexcept = 0;
    virtual int64_t nextTargetSeqNum() const noexcept = 0;

    virtual void setNextSenderSeqNum(int64_t seqNum) noexcept = 0;
    virtual void setNextTargetSeqNum(int64_t seqNum) noexcept = 0;

    virtual void incrNextSenderSeqNum() noexcept = 0;
    virtual void incrNextTargetSeqNum() noexcept = 0;

    virtual void reset() noexcept = 0;
};

} // namespace falconfix
