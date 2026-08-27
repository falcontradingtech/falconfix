// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <store/message_store_base.h>

namespace falconfix {

class NullMessageStore final : public MessageStore {
    int64_t m_nextSenderSeqNum{1};
    int64_t m_nextTargetSeqNum{1};

public:
    FFStatus storeOutgoing(int64_t seqNum,
                                   std::string_view msgType,
                                   bool admin,
                                   std::string_view raw) noexcept override { return FF_OK(); }
    FFStatus storeIncoming(int64_t seqNum,
                                   std::string_view raw) noexcept override { return FF_OK(); }

    virtual FFStatus getOutgoing(int64_t beginSeqNum,
                                 int64_t endSeqNum,
                                 std::vector<StoredMessageView> &out) const noexcept override {
        return FF_FIX_SESSION_ERR(RESEND_FAILED, 0);
    }

    int64_t nextSenderSeqNum() const noexcept override { return m_nextSenderSeqNum; }
    int64_t nextTargetSeqNum() const noexcept override { return m_nextTargetSeqNum; }

    void setNextSenderSeqNum(int64_t seqNum) noexcept override { m_nextSenderSeqNum = seqNum; }
    void setNextTargetSeqNum(int64_t seqNum) noexcept override {  m_nextTargetSeqNum = seqNum; }

    void incrNextSenderSeqNum() noexcept override { ++m_nextSenderSeqNum; }
    void incrNextTargetSeqNum() noexcept override { ++m_nextTargetSeqNum; }

    void reset() noexcept override {
        m_nextSenderSeqNum = 1;
        m_nextTargetSeqNum = 1;
    }
};

} // namespace falconfix
