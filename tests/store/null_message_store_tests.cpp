// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <gtest/gtest.h>

#include <string>
#include <vector>

#include <store/null_message_store.h>

namespace {

using namespace falconfix;

class NullMessageStoreTests : public ::testing::Test {
protected:
	NullMessageStore store;
};

// ============================================================================
// CONSTRUCTOR AND LIFECYCLE TESTS
// ============================================================================

TEST_F(NullMessageStoreTests, ConstructorDoesNotThrow) {
	EXPECT_NO_THROW(NullMessageStore());
}

TEST_F(NullMessageStoreTests, InitialStateIsValid) {
	EXPECT_EQ(store.nextSenderSeqNum(), 1);
	EXPECT_EQ(store.nextTargetSeqNum(), 1);
}

// ============================================================================
// STORE OPERATIONS - They Should Always Succeed But Do Nothing
// ============================================================================

TEST_F(NullMessageStoreTests, StoreOutgoingDoesNotThrow) {
	auto result = store.storeOutgoing(1, "A", false, "raw_message_data");
	EXPECT_TRUE(result.ok());
}

TEST_F(NullMessageStoreTests, StoreIncomingDoesNotThrow) {
	auto result = store.storeIncoming(1, "raw_incoming_data");
	EXPECT_TRUE(result.ok());
}

TEST_F(NullMessageStoreTests, StoreOutgoingMultipleTimes) {
	for (int i = 1; i <= 100; ++i) {
		auto result = store.storeOutgoing(i, "A", false, "message_" + std::to_string(i));
		EXPECT_TRUE(result.ok());
	}
}

TEST_F(NullMessageStoreTests, StoreIncomingMultipleTimes) {
	for (int i = 1; i <= 100; ++i) {
		auto result = store.storeIncoming(i, "message_" + std::to_string(i));
		EXPECT_TRUE(result.ok());
	}
}

TEST_F(NullMessageStoreTests, StoreWithEmptyStrings) {
	EXPECT_TRUE(store.storeOutgoing(1, "", false, "").ok());
	EXPECT_TRUE(store.storeIncoming(1, "").ok());
}

TEST_F(NullMessageStoreTests, StoreWithLargeSequenceNumbers) {
	auto result = store.storeOutgoing(INT64_MAX - 1, "A", false, "large_seq");
	EXPECT_TRUE(result.ok());
}

TEST_F(NullMessageStoreTests, StoreOutgoingWithAdminFlag) {
	EXPECT_TRUE(store.storeOutgoing(1, "A", true, "admin_message").ok());
	EXPECT_TRUE(store.storeOutgoing(2, "0", false, "regular_message").ok());
}

// ============================================================================
// RETRIEVE OPERATIONS - Should Always Fail Gracefully
// ============================================================================

TEST_F(NullMessageStoreTests, GetOutgoingReturnsError) {
	std::vector<StoredMessageView> out;
	auto result = store.getOutgoing(1, 10, out);
	EXPECT_FALSE(result.ok());
	EXPECT_TRUE(out.empty());
}

TEST_F(NullMessageStoreTests, GetOutgoingWithZeroRange) {
	std::vector<StoredMessageView> out;
	auto result = store.getOutgoing(0, 0, out);
	EXPECT_FALSE(result.ok());
	EXPECT_TRUE(out.empty());
}

TEST_F(NullMessageStoreTests, GetOutgoingWithNegativeRange) {
	std::vector<StoredMessageView> out;
	auto result = store.getOutgoing(-100, 10, out);
	EXPECT_FALSE(result.ok());
	EXPECT_TRUE(out.empty());
}

TEST_F(NullMessageStoreTests, GetOutgoingWithReverseRange) {
	std::vector<StoredMessageView> out;
	auto result = store.getOutgoing(100, 10, out);
	EXPECT_FALSE(result.ok());
	EXPECT_TRUE(out.empty());
}

TEST_F(NullMessageStoreTests, GetOutgoingMultipleTimes) {
	std::vector<StoredMessageView> out;
	for (int i = 0; i < 10; ++i) {
		auto result = store.getOutgoing(i, i + 5, out);
		EXPECT_FALSE(result.ok());
	}
}

TEST_F(NullMessageStoreTests, GetOutgoingAfterStore) {
	// Even after storing, getOutgoing should fail (null store doesn't actually store)
	EXPECT_TRUE(store.storeOutgoing(1, "A", false, "msg").ok());

	std::vector<StoredMessageView> out;
	auto result = store.getOutgoing(1, 1, out);
	EXPECT_FALSE(result.ok());
	EXPECT_TRUE(out.empty());
}

// ============================================================================
// SEQUENCE MANAGEMENT - nextSenderSeqNum / nextTargetSeqNum
// ============================================================================

TEST_F(NullMessageStoreTests, NextSenderSeqNumInitialValue) {
	EXPECT_EQ(store.nextSenderSeqNum(), 1);
}

TEST_F(NullMessageStoreTests, NextTargetSeqNumInitialValue) {
	EXPECT_EQ(store.nextTargetSeqNum(), 1);
}

TEST_F(NullMessageStoreTests, SetNextSenderSeqNum) {
	store.setNextSenderSeqNum(100);
	EXPECT_EQ(store.nextSenderSeqNum(), 100);
}

TEST_F(NullMessageStoreTests, SetNextTargetSeqNum) {
	store.setNextTargetSeqNum(200);
	EXPECT_EQ(store.nextTargetSeqNum(), 200);
}

TEST_F(NullMessageStoreTests, SetNextSenderSeqNumToLargeValue) {
	store.setNextSenderSeqNum(1000000000);
	EXPECT_EQ(store.nextSenderSeqNum(), 1000000000);
}

TEST_F(NullMessageStoreTests, SetNextTargetSeqNumToLargeValue) {
	store.setNextTargetSeqNum(1000000000);
	EXPECT_EQ(store.nextTargetSeqNum(), 1000000000);
}

TEST_F(NullMessageStoreTests, SetNextSeqNumToZero) {
	store.setNextSenderSeqNum(0);
	store.setNextTargetSeqNum(0);
	EXPECT_EQ(store.nextSenderSeqNum(), 0);
	EXPECT_EQ(store.nextTargetSeqNum(), 0);
}

TEST_F(NullMessageStoreTests, SetNextSeqNumToNegative) {
	store.setNextSenderSeqNum(-1);
	store.setNextTargetSeqNum(-1);
	EXPECT_EQ(store.nextSenderSeqNum(), -1);
	EXPECT_EQ(store.nextTargetSeqNum(), -1);
}

// ============================================================================
// INCREMENT OPERATIONS
// ============================================================================

TEST_F(NullMessageStoreTests, IncrNextSenderSeqNum) {
	store.setNextSenderSeqNum(50);
	store.incrNextSenderSeqNum();
	EXPECT_EQ(store.nextSenderSeqNum(), 51);
}

TEST_F(NullMessageStoreTests, IncrNextTargetSeqNum) {
	store.setNextTargetSeqNum(75);
	store.incrNextTargetSeqNum();
	EXPECT_EQ(store.nextTargetSeqNum(), 76);
}

TEST_F(NullMessageStoreTests, IncrNextSenderSeqNumMultipleTimes) {
	for (int i = 0; i < 10; ++i) {
		store.incrNextSenderSeqNum();
	}
	EXPECT_EQ(store.nextSenderSeqNum(), 11); // Started at 1
}

TEST_F(NullMessageStoreTests, IncrNextTargetSeqNumMultipleTimes) {
	for (int i = 0; i < 10; ++i) {
		store.incrNextTargetSeqNum();
	}
	EXPECT_EQ(store.nextTargetSeqNum(), 11); // Started at 1
}

TEST_F(NullMessageStoreTests, IncrNextSeqNumFromZero) {
	store.setNextSenderSeqNum(0);
	store.setNextTargetSeqNum(0);
	store.incrNextSenderSeqNum();
	store.incrNextTargetSeqNum();
	EXPECT_EQ(store.nextSenderSeqNum(), 1);
	EXPECT_EQ(store.nextTargetSeqNum(), 1);
}

TEST_F(NullMessageStoreTests, IncrNextSeqNumFromNegative) {
	store.setNextSenderSeqNum(-5);
	store.setNextTargetSeqNum(-5);
	store.incrNextSenderSeqNum();
	store.incrNextTargetSeqNum();
	EXPECT_EQ(store.nextSenderSeqNum(), -4);
	EXPECT_EQ(store.nextTargetSeqNum(), -4);
}

// ============================================================================
// RESET OPERATION
// ============================================================================

TEST_F(NullMessageStoreTests, ResetResetsToInitialState) {
	store.setNextSenderSeqNum(100);
	store.setNextTargetSeqNum(200);
	store.reset();
	EXPECT_EQ(store.nextSenderSeqNum(), 1);
	EXPECT_EQ(store.nextTargetSeqNum(), 1);
}

TEST_F(NullMessageStoreTests, ResetAfterIncrement) {
	for (int i = 0; i < 50; ++i) {
		store.incrNextSenderSeqNum();
		store.incrNextTargetSeqNum();
	}
	store.reset();
	EXPECT_EQ(store.nextSenderSeqNum(), 1);
	EXPECT_EQ(store.nextTargetSeqNum(), 1);
}

TEST_F(NullMessageStoreTests, ResetAfterStore) {
	// Store many messages
	for (int i = 1; i <= 100; ++i) {
		store.storeOutgoing(i, "A", false, "msg");
		store.storeIncoming(i, "msg");
	}
	store.reset();
	EXPECT_EQ(store.nextSenderSeqNum(), 1);
	EXPECT_EQ(store.nextTargetSeqNum(), 1);
}

TEST_F(NullMessageStoreTests, ResetAfterModifyingSeqNum) {
	store.setNextSenderSeqNum(-999);
	store.setNextTargetSeqNum(999);
	store.reset();
	EXPECT_EQ(store.nextSenderSeqNum(), 1);
	EXPECT_EQ(store.nextTargetSeqNum(), 1);
}

TEST_F(NullMessageStoreTests, MultipleResets) {
	for (int cycle = 0; cycle < 5; ++cycle) {
		store.setNextSenderSeqNum(100 * (cycle + 1));
		store.setNextTargetSeqNum(200 * (cycle + 1));
		store.reset();
		EXPECT_EQ(store.nextSenderSeqNum(), 1);
		EXPECT_EQ(store.nextTargetSeqNum(), 1);
	}
}

// ============================================================================
// COMBINED OPERATIONS - Stress Testing
// ============================================================================

TEST_F(NullMessageStoreTests, StoreAndSequenceManipulation) {
	store.setNextSenderSeqNum(10);

	for (int i = 10; i < 20; ++i) {
		store.storeOutgoing(i, "A", false, "msg_" + std::to_string(i));
		store.incrNextSenderSeqNum();
	}

	EXPECT_EQ(store.nextSenderSeqNum(), 20);

	// Even after all these operations, getOutgoing should fail
	std::vector<StoredMessageView> out;
	EXPECT_FALSE(store.getOutgoing(10, 19, out).ok());
}

TEST_F(NullMessageStoreTests, AlternatingSetAndIncr) {
	for (int i = 0; i < 10; ++i) {
		store.setNextSenderSeqNum(i * 10);
		store.setNextTargetSeqNum(i * 20);
		store.incrNextSenderSeqNum();
		store.incrNextTargetSeqNum();
	}

	EXPECT_EQ(store.nextSenderSeqNum(), 91); // 90 + 1
	EXPECT_EQ(store.nextTargetSeqNum(), 181); // 180 + 1
}

TEST_F(NullMessageStoreTests, NullStoreIsAlwaysNull) {
	// The null store should always behave the same regardless of operations

	// Do various operations
	store.storeOutgoing(1, "A", false, "msg1");
	store.storeIncoming(1, "msg1");
	store.setNextSenderSeqNum(100);
	store.incrNextSenderSeqNum();

	// Try to retrieve: should always fail
	std::vector<StoredMessageView> out;
	EXPECT_FALSE(store.getOutgoing(1, 100, out).ok());
	EXPECT_TRUE(out.empty());
}

TEST_F(NullMessageStoreTests, LargeSequenceNumbers) {
	const int64_t LARGE_SEQ = 9223372036854775700LL; // Near INT64_MAX

	store.setNextSenderSeqNum(LARGE_SEQ);
	EXPECT_EQ(store.nextSenderSeqNum(), LARGE_SEQ);

	store.storeOutgoing(LARGE_SEQ, "A", false, "large_seq_msg");
	EXPECT_TRUE(true); // No exception thrown
}

// ============================================================================
// INTERFACE COMPLIANCE
// ============================================================================

TEST_F(NullMessageStoreTests, IsMessageStoreDerivative) {
	// Compile-time check that NullMessageStore is a MessageStore
	MessageStore* interface_ptr = &store;
	EXPECT_NE(interface_ptr, nullptr);
}

TEST_F(NullMessageStoreTests, AllMethodsAreOverridden) {
	// This test verifies that all virtual methods can be called
	store.storeOutgoing(1, "A", false, "msg");
	store.storeIncoming(1, "msg");
	std::vector<StoredMessageView> out;
	store.getOutgoing(1, 10, out);
	store.nextSenderSeqNum();
	store.nextTargetSeqNum();
	store.setNextSenderSeqNum(100);
	store.setNextTargetSeqNum(200);
	store.incrNextSenderSeqNum();
	store.incrNextTargetSeqNum();
	store.reset();

	EXPECT_TRUE(true); // All methods callable
}

} // anonymous namespace
