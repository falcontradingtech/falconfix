// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <deque>
#include <string>

#include <store/message_store.h>

namespace falconfix {

class MessageStoreBase : public MessageStore {
protected:
    struct StoredMessage {
        int64_t seqNum{0};
        std::string msgType;
        bool admin{false};
        std::string raw;
    };

    std::deque<StoredMessage> m_outgoing;

    int64_t m_nextSenderSeqNum{1};
    int64_t m_nextTargetSeqNum{1};
    bool    m_persistMessages{true};

public:
    explicit MessageStoreBase(bool persistMessages = true) noexcept
        : m_persistMessages(persistMessages) {}

    FFStatus getOutgoing(int64_t beginSeqNum,
                         int64_t endSeqNum,
                         std::vector<StoredMessageView> &out) const noexcept override {
        try {
            out.clear();

            for (const auto &msg : m_outgoing) {
                if (msg.seqNum < beginSeqNum) {
                    continue;
                }

                if (msg.seqNum > endSeqNum) {
                    break;
                }

                out.push_back({
                    msg.seqNum,
                    msg.msgType,
                    msg.admin,
                    msg.raw
                });
            }

            return FF_OK();
        }
        catch (...) {
            return FF_SYS_ERR(BAD_ALLOC);
        }
    }

    int64_t nextSenderSeqNum() const noexcept override { return m_nextSenderSeqNum; }
    int64_t nextTargetSeqNum() const noexcept override { return m_nextTargetSeqNum; }

    void setNextSenderSeqNum(int64_t seqNum) noexcept override {
        m_nextSenderSeqNum = seqNum;
        persistMeta();
    }

    void setNextTargetSeqNum(int64_t seqNum) noexcept override {
        m_nextTargetSeqNum = seqNum;
        persistMeta();
    }

    void incrNextSenderSeqNum() noexcept override {
        ++m_nextSenderSeqNum;
        persistMeta();
    }

    void incrNextTargetSeqNum() noexcept override {
        ++m_nextTargetSeqNum;
        persistMeta();
    }

    void reset() noexcept override {
        m_outgoing.clear();
        m_nextSenderSeqNum = 1;
        m_nextTargetSeqNum = 1;
        clearPersisted();
        persistMeta();
    }

protected:
    virtual void persistMeta() noexcept {}
    virtual void clearPersisted() noexcept {}
};

} // namespace falconfix