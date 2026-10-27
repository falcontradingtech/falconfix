// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <gtest/gtest.h>

#include <session/session_config.h>

namespace falconfix {

TEST(SessionConfigEndpointTests, ValidRequiresHostAndPositivePort) {
	SessionConfig::Connection::Endpoint ep;
	EXPECT_FALSE(ep.valid()); // empty host, zero port

	ep.host = "example.com";
	EXPECT_FALSE(ep.valid()); // host set but port still 0

	ep.port = 5001;
	EXPECT_TRUE(ep.valid()); // host + positive port
}

TEST(SessionConfigEndpointTests, ValidRejectsNonPositivePort) {
	SessionConfig::Connection::Endpoint ep;
	ep.host = "host";
	ep.port = 0;
	EXPECT_FALSE(ep.valid());

	ep.port = -1;
	EXPECT_FALSE(ep.valid());
}

TEST(SessionConfigEndpointTests, ValidRejectsEmptyHost) {
	SessionConfig::Connection::Endpoint ep;
	ep.host.clear();
	ep.port = 1234;
	EXPECT_FALSE(ep.valid());
}

} // namespace falconfix
