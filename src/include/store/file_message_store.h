// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <string>
#include <string_view>

#include <session/session_id.h>
#include <store/message_store_base.h>
#include <utils/error_codes.h>

namespace falconfix {

class FileMessageStore final : public MessageStoreBase {
    struct FileRecordHeader {
        int64_t  seqNum{0};
        uint32_t msgTypeSize{0};
        uint32_t rawSize{0};
        uint8_t  admin{0};
    };

    std::filesystem::path m_baseDir;
    std::filesystem::path m_outgoingFile;
    std::filesystem::path m_incomingFile;
    std::filesystem::path m_metaFile;

public:
    explicit FileMessageStore(std::string_view basePath,
                              const SessionID &sid,
                              bool persistMessages = true) noexcept
        : MessageStoreBase(persistMessages) {
        try {
            std::string sidText = sid.toString();

            for (char &ch : sidText) {
                if (ch == ':' || ch == '-' || ch == '>') {
                    ch = '_';
                }
            }

            m_baseDir = basePath.empty()
                ? std::filesystem::path(".falconfix_store") / sidText
                : std::filesystem::path(basePath) / sidText;

            m_outgoingFile = m_baseDir / "outgoing.bin";
            m_incomingFile = m_baseDir / "incoming.bin";
            m_metaFile = m_baseDir / "seq.meta";

            std::error_code ec;
            std::filesystem::create_directories(m_baseDir, ec);

            loadFromDisk();
        }
        catch (...) {
            MessageStoreBase::reset();
        }
    }

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

            return appendOutgoing(seqNum, msgType, admin, raw);
        }
        catch (const std::bad_alloc &) {
            return FF_SYS_ERR(BAD_ALLOC);
        }
        catch (...) {
            return FF_SYS_ERR(UNKNOWN);
        }
    }

    FFStatus storeIncoming(int64_t seqNum,
                           std::string_view raw) noexcept override {
        try {
            FileRecordHeader h{};
            h.seqNum = seqNum;
            h.rawSize = static_cast<uint32_t>(raw.size());

            std::ofstream out(m_incomingFile, std::ios::binary | std::ios::app);
            if (!out) {
                return FF_SYS_ERR(UNKNOWN);
            }

            out.write(reinterpret_cast<const char *>(&h), sizeof(h));
            out.write(raw.data(), static_cast<std::streamsize>(raw.size()));

            return out ? FF_OK() : FF_SYS_ERR(UNKNOWN);
        }
        catch (...) {
            return FF_SYS_ERR(UNKNOWN);
        }
    }

private:
    FFStatus appendOutgoing(int64_t seqNum,
                            std::string_view msgType,
                            bool admin,
                            std::string_view raw) noexcept {
        FileRecordHeader h{};
        h.seqNum = seqNum;
        h.msgTypeSize = static_cast<uint32_t>(msgType.size());
        h.rawSize = static_cast<uint32_t>(raw.size());
        h.admin = admin ? 1U : 0U;

        std::ofstream out(m_outgoingFile, std::ios::binary | std::ios::app);
        if (!out) {
            return FF_SYS_ERR(UNKNOWN);
        }

        out.write(reinterpret_cast<const char *>(&h), sizeof(h));
        out.write(msgType.data(), static_cast<std::streamsize>(msgType.size()));
        out.write(raw.data(), static_cast<std::streamsize>(raw.size()));

        return out ? FF_OK() : FF_SYS_ERR(UNKNOWN);
    }

    void loadFromDisk() noexcept {
        m_outgoing.clear();
        m_nextSenderSeqNum = 1;
        m_nextTargetSeqNum = 1;

        loadMeta();
        loadOutgoing();
    }

    void loadMeta() noexcept {
        std::ifstream meta(m_metaFile, std::ios::binary);
        if (!meta) {
            return;
        }

        int64_t sender = 1;
        int64_t target = 1;

        meta >> sender >> target;

        if (meta.good() || meta.eof()) {
            m_nextSenderSeqNum = sender > 0 ? sender : 1;
            m_nextTargetSeqNum = target > 0 ? target : 1;
        }
    }

    void loadOutgoing() noexcept {
        std::ifstream in(m_outgoingFile, std::ios::binary);
        if (!in) {
            return;
        }

        while (true) {
            FileRecordHeader h{};

            in.read(reinterpret_cast<char *>(&h), sizeof(h));
            if (!in) {
                break;
            }

            if (h.seqNum <= 0 || h.rawSize == 0 || h.msgTypeSize > 32) {
                break;
            }

            std::string msgType;
            std::string raw;

            msgType.resize(h.msgTypeSize);
            raw.resize(h.rawSize);

            in.read(msgType.data(), static_cast<std::streamsize>(h.msgTypeSize));
            in.read(raw.data(), static_cast<std::streamsize>(h.rawSize));

            if (!in) {
                break;
            }

            m_outgoing.push_back({
                h.seqNum,
                std::move(msgType),
                h.admin != 0,
                std::move(raw)
            });
        }
    }

    void persistMeta() noexcept override {
        try {
            std::ofstream meta(m_metaFile, std::ios::binary | std::ios::trunc);
            if (!meta) {
                return;
            }

            meta << m_nextSenderSeqNum << '\n'
                 << m_nextTargetSeqNum << '\n';
        }
        catch (...) {
        }
    }

    void clearPersisted() noexcept override {
        try {
            if (!m_outgoingFile.empty()) {
                std::ofstream out(m_outgoingFile, std::ios::binary | std::ios::trunc);
            }

            if (!m_incomingFile.empty()) {
                std::ofstream in(m_incomingFile, std::ios::binary | std::ios::trunc);
            }
        }
        catch (...) {
        }
    }
};

} // namespace falconfix