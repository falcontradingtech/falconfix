// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <gtest/gtest.h>

#include <store/memory_message_store.h>
#include <store/null_message_store.h>

TEST(MemoryMessageStoreTests, DefaultsStartAtSeqNumOne) {
    falconfix::MemoryMessageStore store;
    EXPECT_EQ(store.nextSenderSeqNum(), 1);
    EXPECT_EQ(store.nextTargetSeqNum(), 1);
}

TEST(MemoryMessageStoreTests, StoreOutgoingAndGetRange) {
    falconfix::MemoryMessageStore store;
    ASSERT_TRUE(store.storeOutgoing(1, "TEST", true, "msg-1").ok());
    ASSERT_TRUE(store.storeOutgoing(2, "TEST", true, "msg-2").ok());
    ASSERT_TRUE(store.storeOutgoing(3, "TEST", true, "msg-3").ok());
    std::vector<falconfix::StoredMessageView> out;
    ASSERT_TRUE(store.getOutgoing(2, 3, out).ok());
    ASSERT_EQ(out.size(), 2U);
    EXPECT_EQ(out[0].raw, "msg-2");
    EXPECT_EQ(out[1].raw, "msg-3");
}

TEST(MemoryMessageStoreTests, IncomingStoreReturnsOk) {
    falconfix::MemoryMessageStore store;
    EXPECT_TRUE(store.storeIncoming(7, "incoming").ok());
}

TEST(MemoryMessageStoreTests, SeqNumSetAndIncrementWorks) {
    falconfix::MemoryMessageStore store;
    store.setNextSenderSeqNum(41);
    store.setNextTargetSeqNum(73);
    EXPECT_EQ(store.nextSenderSeqNum(), 41);
    EXPECT_EQ(store.nextTargetSeqNum(), 73);
    store.incrNextSenderSeqNum();
    store.incrNextTargetSeqNum();
    EXPECT_EQ(store.nextSenderSeqNum(), 42);
    EXPECT_EQ(store.nextTargetSeqNum(), 74);
}

TEST(MemoryMessageStoreTests, ResetClearsDataAndSequenceNumbers) {
    falconfix::MemoryMessageStore store;
    ASSERT_TRUE(store.storeOutgoing(1, "TEST", true, "msg-1").ok());
    ASSERT_TRUE(store.storeOutgoing(2, "TEST", true, "msg-2").ok());
    store.setNextSenderSeqNum(11);
    store.setNextTargetSeqNum(19);
    store.reset();
    std::vector<falconfix::StoredMessageView> out;
    ASSERT_TRUE(store.getOutgoing(1, 100, out).ok());
    EXPECT_TRUE(out.empty());
    EXPECT_EQ(store.nextSenderSeqNum(), 1);
    EXPECT_EQ(store.nextTargetSeqNum(), 1);
}

TEST(NullMessageStoreTests, DefaultsAndRejectGetOutgoing) {
    falconfix::NullMessageStore store;
    EXPECT_EQ(store.nextSenderSeqNum(), 1);
    EXPECT_EQ(store.nextTargetSeqNum(), 1);
    std::vector<falconfix::StoredMessageView> out;
    EXPECT_FALSE(store.getOutgoing(1, 2, out).ok());
}

TEST(MemoryMessageStoreTests, PersistMessagesFalseSkipsMessageStorageButTracksSeqNums) {
    falconfix::MemoryMessageStore store(/*persistMessages=*/false);

    ASSERT_TRUE(store.storeOutgoing(1, "D", false, "msg-1").ok());
    ASSERT_TRUE(store.storeOutgoing(2, "D", false, "msg-2").ok());
    store.incrNextSenderSeqNum();
    store.incrNextSenderSeqNum();

    std::vector<falconfix::StoredMessageView> out;
    ASSERT_TRUE(store.getOutgoing(1, 2, out).ok());
    EXPECT_TRUE(out.empty());

    EXPECT_EQ(store.nextSenderSeqNum(), 3);
}

TEST(MemoryMessageStoreTests, PersistMessagesTrueStoresBothMessages) {
    falconfix::MemoryMessageStore store(/*persistMessages=*/true);

    ASSERT_TRUE(store.storeOutgoing(1, "D", false, "msg-1").ok());
    ASSERT_TRUE(store.storeOutgoing(2, "D", false, "msg-2").ok());

    std::vector<falconfix::StoredMessageView> out;
    ASSERT_TRUE(store.getOutgoing(1, 2, out).ok());
    EXPECT_EQ(out.size(), 2U);
}
