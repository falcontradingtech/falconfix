// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <gtest/gtest.h>

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include <session/session_settings.h>
#include <store/file_message_store.h>
#include <store/memory_message_store.h>
#include <store/message_store_factory.h>

namespace {

std::filesystem::path makeTempStorePath(const char *suffix) {
    const auto stamp = std::chrono::steady_clock::now().time_since_epoch().count();
    auto path = std::filesystem::temp_directory_path() /
        (std::string("falconfix_store_tests_") + suffix + "_" + std::to_string(stamp));
    std::filesystem::create_directories(path);
    return path;
}

std::filesystem::path storeSessionDir(const std::filesystem::path &root,
                                      const falconfix::SessionID &sid) {
    std::string sidText = sid.toString();
    for (char &ch : sidText) {
        if (ch == ':' || ch == '-' || ch == '>') {
            ch = '_';
        }
    }
    return root / sidText;
}

std::filesystem::path outgoingFilePath(const std::filesystem::path &root,
                                       const falconfix::SessionID &sid) {
    return storeSessionDir(root, sid) / "outgoing.bin";
}

std::filesystem::path incomingFilePath(const std::filesystem::path &root,
                                       const falconfix::SessionID &sid) {
    return storeSessionDir(root, sid) / "incoming.bin";
}

struct FileRecordHeaderWire {
    int64_t seqNum{0};
    uint32_t msgTypeSize{0};
    uint32_t rawSize{0};
    uint8_t admin{0};
};

void appendCorruptRecordHeader(const std::filesystem::path &path,
                               const FileRecordHeaderWire &header) {
    std::ofstream out(path, std::ios::binary | std::ios::app);
    ASSERT_TRUE(out.is_open());
    out.write(reinterpret_cast<const char *>(&header), sizeof(header));
    ASSERT_TRUE(out.good());
}

void appendBytes(const std::filesystem::path &path, std::string_view bytes) {
    std::ofstream out(path, std::ios::binary | std::ios::app);
    ASSERT_TRUE(out.is_open());
    out.write(bytes.data(), static_cast<std::streamsize>(bytes.size()));
    ASSERT_TRUE(out.good());
}

} // namespace

TEST(FileMessageStoreTests, PersistsOutgoingAndSequenceAcrossInstances) {
    const auto root = makeTempStorePath("persist");
    const falconfix::SessionID sid("FIX4.4", "FS", "FC");
    {
        falconfix::FileMessageStore store(root.string(), sid);
        ASSERT_TRUE(store.storeOutgoing(1, "TEST", true, "msg-1").ok());
        ASSERT_TRUE(store.storeOutgoing(2, "TEST", true, "msg-2").ok());
        store.setNextSenderSeqNum(21);
        store.setNextTargetSeqNum(34);
    }
    {
        falconfix::FileMessageStore store(root.string(), sid);
        std::vector<falconfix::StoredMessageView> out;
        ASSERT_TRUE(store.getOutgoing(1, 3, out).ok());
        ASSERT_EQ(out.size(), 2U);
        EXPECT_EQ(out[0].raw, "msg-1");
        EXPECT_EQ(out[1].raw, "msg-2");
        EXPECT_EQ(store.nextSenderSeqNum(), 21);
        EXPECT_EQ(store.nextTargetSeqNum(), 34);
    }
    std::filesystem::remove_all(root);
}

TEST(FileMessageStoreTests, ResetClearsPersistedDataAndSequenceNumbers) {
    const auto root = makeTempStorePath("reset");
    const falconfix::SessionID sid("FIX4.4", "RS", "RC");
    {
        falconfix::FileMessageStore store(root.string(), sid);
        ASSERT_TRUE(store.storeOutgoing(1, "TEST", true, "x").ok());
        store.setNextSenderSeqNum(8);
        store.setNextTargetSeqNum(9);
        store.reset();
    }
    {
        falconfix::FileMessageStore store(root.string(), sid);
        std::vector<falconfix::StoredMessageView> out;
        ASSERT_TRUE(store.getOutgoing(1, 100, out).ok());
        EXPECT_TRUE(out.empty());
        EXPECT_EQ(store.nextSenderSeqNum(), 1);
        EXPECT_EQ(store.nextTargetSeqNum(), 1);
    }
    std::filesystem::remove_all(root);
}

TEST(FileMessageStoreTests, RestoresValidPrefixBeforeTruncatedRecord) {
    const auto root = makeTempStorePath("truncated");
    const falconfix::SessionID sid("FIX4.4", "TR", "TC");

    {
        falconfix::FileMessageStore store(root.string(), sid);
        ASSERT_TRUE(store.storeOutgoing(1, "TEST", true, "msg-1").ok());
        ASSERT_TRUE(store.storeOutgoing(2, "TEST", false, "msg-2").ok());
    }

    const auto outgoing = outgoingFilePath(root, sid);
    FileRecordHeaderWire truncated{};
    truncated.seqNum = 3;
    truncated.msgTypeSize = 4;
    truncated.rawSize = 10;
    truncated.admin = 1;
    appendCorruptRecordHeader(outgoing, truncated);
    appendBytes(outgoing, "TESTonly-part");

    {
        falconfix::FileMessageStore store(root.string(), sid);
        std::vector<falconfix::StoredMessageView> out;
        ASSERT_TRUE(store.getOutgoing(1, 10, out).ok());
        ASSERT_EQ(out.size(), 2U);
        EXPECT_EQ(out[0].seqNum, 1);
        EXPECT_EQ(out[0].raw, "msg-1");
        EXPECT_EQ(out[1].seqNum, 2);
        EXPECT_EQ(out[1].raw, "msg-2");
    }

    std::filesystem::remove_all(root);
}

TEST(FileMessageStoreTests, StopsGracefullyOnInvalidRecordHeader) {
    const auto root = makeTempStorePath("bad_header");
    const falconfix::SessionID sid("FIX4.4", "BH", "BC");

    {
        falconfix::FileMessageStore store(root.string(), sid);
        ASSERT_TRUE(store.storeOutgoing(1, "TEST", true, "msg-1").ok());
    }

    const auto outgoing = outgoingFilePath(root, sid);
    FileRecordHeaderWire invalid{};
    invalid.seqNum = 0;
    invalid.msgTypeSize = 4;
    invalid.rawSize = 5;
    invalid.admin = 1;
    appendCorruptRecordHeader(outgoing, invalid);
    appendBytes(outgoing, "TESTjunk!");

    {
        falconfix::FileMessageStore store(root.string(), sid);
        std::vector<falconfix::StoredMessageView> out;
        ASSERT_TRUE(store.getOutgoing(1, 10, out).ok());
        ASSERT_EQ(out.size(), 1U);
        EXPECT_EQ(out[0].seqNum, 1);
        EXPECT_EQ(out[0].raw, "msg-1");
    }

    std::filesystem::remove_all(root);
}

TEST(FileMessageStoreTests, StopsGracefullyOnInvalidRawSize) {
    const auto root = makeTempStorePath("bad_raw");
    const falconfix::SessionID sid("FIX4.4", "BR", "BC");

    {
        falconfix::FileMessageStore store(root.string(), sid);
        ASSERT_TRUE(store.storeOutgoing(1, "TEST", true, "msg-1").ok());
    }

    const auto outgoing = outgoingFilePath(root, sid);
    FileRecordHeaderWire invalid{};
    invalid.seqNum = 2;
    invalid.msgTypeSize = 4;
    invalid.rawSize = 0;
    invalid.admin = 1;
    appendCorruptRecordHeader(outgoing, invalid);
    appendBytes(outgoing, "TEST");

    {
        falconfix::FileMessageStore store(root.string(), sid);
        std::vector<falconfix::StoredMessageView> out;
        ASSERT_TRUE(store.getOutgoing(1, 10, out).ok());
        ASSERT_EQ(out.size(), 1U);
        EXPECT_EQ(out[0].seqNum, 1);
        EXPECT_EQ(out[0].raw, "msg-1");
    }

    std::filesystem::remove_all(root);
}

TEST(FileMessageStoreTests, StopsGracefullyOnInvalidMsgTypeSize) {
    const auto root = makeTempStorePath("bad_type");
    const falconfix::SessionID sid("FIX4.4", "BT", "BC");

    {
        falconfix::FileMessageStore store(root.string(), sid);
        ASSERT_TRUE(store.storeOutgoing(1, "TEST", true, "msg-1").ok());
    }

    const auto outgoing = outgoingFilePath(root, sid);
    FileRecordHeaderWire invalid{};
    invalid.seqNum = 2;
    invalid.msgTypeSize = 33;
    invalid.rawSize = 5;
    invalid.admin = 1;
    appendCorruptRecordHeader(outgoing, invalid);
    appendBytes(outgoing, "TESTmsg-2");

    {
        falconfix::FileMessageStore store(root.string(), sid);
        std::vector<falconfix::StoredMessageView> out;
        ASSERT_TRUE(store.getOutgoing(1, 10, out).ok());
        ASSERT_EQ(out.size(), 1U);
        EXPECT_EQ(out[0].seqNum, 1);
        EXPECT_EQ(out[0].raw, "msg-1");
    }

    std::filesystem::remove_all(root);
}

TEST(FileMessageStoreTests, IgnoresIncomingFileCorruptionDuringRestore) {
    const auto root = makeTempStorePath("incoming_corrupt");
    const falconfix::SessionID sid("FIX4.4", "IC", "BC");

    {
        falconfix::FileMessageStore store(root.string(), sid);
        ASSERT_TRUE(store.storeOutgoing(1, "TEST", true, "msg-1").ok());
        ASSERT_TRUE(store.storeIncoming(1, "incoming-1").ok());
    }

    const auto incoming = incomingFilePath(root, sid);
    FileRecordHeaderWire invalid{};
    invalid.seqNum = 2;
    invalid.msgTypeSize = 999999;
    invalid.rawSize = 999999;
    invalid.admin = 1;
    appendCorruptRecordHeader(incoming, invalid);
    appendBytes(incoming, "broken");

    {
        falconfix::FileMessageStore store(root.string(), sid);
        std::vector<falconfix::StoredMessageView> out;
        ASSERT_TRUE(store.getOutgoing(1, 10, out).ok());
        ASSERT_EQ(out.size(), 1U);
        EXPECT_EQ(out[0].seqNum, 1);
        EXPECT_EQ(out[0].raw, "msg-1");
        EXPECT_EQ(store.nextSenderSeqNum(), 1);
        EXPECT_EQ(store.nextTargetSeqNum(), 1);
    }

    std::filesystem::remove_all(root);
}

TEST(FileMessageStoreTests, StopsGracefullyOnAbsurdRawSizeLargerThanFile) {
    const auto root = makeTempStorePath("absurd_raw");
    const falconfix::SessionID sid("FIX4.4", "AR", "BC");

    {
        falconfix::FileMessageStore store(root.string(), sid);
        ASSERT_TRUE(store.storeOutgoing(1, "TEST", true, "msg-1").ok());
        ASSERT_TRUE(store.storeOutgoing(2, "TEST", false, "msg-2").ok());
    }

    const auto outgoing = outgoingFilePath(root, sid);
    FileRecordHeaderWire invalid{};
    invalid.seqNum = 3;
    invalid.msgTypeSize = 4;
    invalid.rawSize = 0x7fffffffU;
    invalid.admin = 1;
    appendCorruptRecordHeader(outgoing, invalid);
    appendBytes(outgoing, "TESTtiny");

    {
        falconfix::FileMessageStore store(root.string(), sid);
        std::vector<falconfix::StoredMessageView> out;
        ASSERT_TRUE(store.getOutgoing(1, 10, out).ok());
        ASSERT_EQ(out.size(), 2U);
        EXPECT_EQ(out[0].seqNum, 1);
        EXPECT_EQ(out[0].raw, "msg-1");
        EXPECT_EQ(out[1].seqNum, 2);
        EXPECT_EQ(out[1].raw, "msg-2");
    }

    std::filesystem::remove_all(root);
}

TEST(MessageStoreFactoryTests, CreatesAllConfiguredStoreTypes) {
    falconfix::SessionConfig cfg{};
    cfg.sessionID = falconfix::SessionID("FIX4.4", "SF", "TF");
    cfg.storage.storeType = falconfix::StoreType::None;
    {
        auto store = falconfix::MessageStoreFactory::create(cfg);
        EXPECT_NE(dynamic_cast<falconfix::NullMessageStore *>(store.get()), nullptr);
    }
    cfg.storage.storeType = falconfix::StoreType::Memory;
    {
        auto store = falconfix::MessageStoreFactory::create(cfg);
        EXPECT_NE(dynamic_cast<falconfix::MemoryMessageStore *>(store.get()), nullptr);
    }
    cfg.storage.storeType = falconfix::StoreType::File;
    cfg.storage.path = makeTempStorePath("factory").string();
    {
        auto store = falconfix::MessageStoreFactory::create(cfg);
        EXPECT_NE(dynamic_cast<falconfix::FileMessageStore *>(store.get()), nullptr);
    }
    std::filesystem::remove_all(cfg.storage.path);
}

TEST(FileMessageStoreTests, PersistMessagesFalseSkipsMessageStorageButTracksSeqNums) {
    const auto root = makeTempStorePath("persist_false");
    const falconfix::SessionID sid("FIX4.4", "SPNM", "TPNM");

    {
        falconfix::FileMessageStore store(root.string(), sid, /*persistMessages=*/false);
        ASSERT_TRUE(store.storeOutgoing(1, "D", false, "msg-1").ok());
        ASSERT_TRUE(store.storeOutgoing(2, "D", false, "msg-2").ok());
        store.incrNextSenderSeqNum();
        store.incrNextSenderSeqNum();
    }

    falconfix::FileMessageStore store2(root.string(), sid, /*persistMessages=*/false);

    std::vector<falconfix::StoredMessageView> out;
    ASSERT_TRUE(store2.getOutgoing(1, 2, out).ok());
    EXPECT_TRUE(out.empty());

    EXPECT_EQ(store2.nextSenderSeqNum(), 3);

    std::filesystem::remove_all(root);
}

TEST(MessageStoreFactoryTests, PersistMessagesFalseProducesEmptyGetOutgoing) {
    falconfix::SessionConfig cfg{};
    cfg.sessionID = falconfix::SessionID("FIX4.4", "SFPM", "TFPM");
    cfg.storage.storeType = falconfix::StoreType::Memory;
    cfg.storage.persistMessages = false;

    auto store = falconfix::MessageStoreFactory::create(cfg);
    ASSERT_TRUE(store->storeOutgoing(1, "D", false, "msg-1").ok());
    store->incrNextSenderSeqNum();

    std::vector<falconfix::StoredMessageView> out;
    ASSERT_TRUE(store->getOutgoing(1, 1, out).ok());
    EXPECT_TRUE(out.empty());
    EXPECT_EQ(store->nextSenderSeqNum(), 2);
}
