// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <gtest/gtest.h>

#include <memory>
#include <type_traits>

#include <socket/socket_types.h>

namespace falconfix::socket {

TEST(SocketTypesTests, DefaultCallbacksAreNoopsAndDoNotThrow) {
	SocketCallbacks cb;

	ASSERT_NE(cb.onConnected, nullptr);
	ASSERT_NE(cb.onClosed, nullptr);
	ASSERT_NE(cb.onRead, nullptr);
	ASSERT_NE(cb.onError, nullptr);

	EXPECT_NO_THROW(cb.onConnected(cb.m_ctx));
	EXPECT_NO_THROW(cb.onClosed(cb.m_ctx));
	EXPECT_NO_THROW(cb.onRead(cb.m_ctx, SocketReadView{}));
	EXPECT_NO_THROW(cb.onError(cb.m_ctx, falconfix::errors::ErrorCode{}, "err"));
}

TEST(SocketTypesTests, DefaultAcceptCallbacksAreNoops) {
	TCPAcceptCallbacks cb;

	ASSERT_NE(cb.onAccept, nullptr);
	ASSERT_NE(cb.onError, nullptr);

	// Exercises detail::noopAccept with a null socket pointer.
	std::shared_ptr<ITCPSocket> sock;
	EXPECT_NO_THROW(cb.onAccept(cb.m_ctx, sock));
	EXPECT_NO_THROW(cb.onError(cb.m_ctx, falconfix::errors::ErrorCode{}, "err"));
}

TEST(SocketTypesTests, SocketReadViewIsTriviallyCopyable) {
	EXPECT_TRUE(std::is_trivially_copyable_v<SocketReadView>);
}

} // namespace falconfix::socket
