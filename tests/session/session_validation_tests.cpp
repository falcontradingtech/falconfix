// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <gtest/gtest.h>

#include <app/fix_application.h>
#include <runtime/fix_runtime.h>
#include <logger/message_log.h>
#include <session/session_id.h>
#include <session/session_settings.h>
#include <store/memory_message_store.h>

#include <support/app_test_support.h>

namespace {

class ValidationApp final : public falconfix::FIXApplication {
public:
    void onCreate(const falconfix::SessionID &) override {}
    void onLogon(const falconfix::SessionID &) override {}
    void onLogout(const falconfix::SessionID &) override {}
    FFStatus toAdmin(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override { return FF_OK(); }
    FFStatus fromAdmin(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override { return FF_OK(); }
    FFStatus toApp(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override { return FF_OK(); }
    FFStatus fromApp(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override { return FF_OK(); }
};

} // namespace

TEST(FIXSessionTests, RejectsMismatchedSessionIdentity) {
#ifndef FALCONFIX_ENABLE_TEST_API
    ValidationApp app;

    falconfix::SessionConfig cfg;
    cfg.protocol.version = falconfix::ProtocolVersion::FIX44;
    cfg.sessionID = falconfix::SessionID(
        app_test_support::beginString(),
        "CLIENT",
        "SERVER"
    );

    auto log = std::make_unique<falconfix::NullMessageLog>();
    auto store = std::make_unique<falconfix::MemoryMessageStore>();

    auto session = falconfix::ProtocolFactory::createSession(
        app,
        cfg,
        std::move(log),
        std::move(store)
    );
    ASSERT_NE(session, nullptr);

    FFStatus rc = session->validateSessionIdentityForTest(
        app_test_support::beginString(),
        "SERVER",
        "CLIENT"
    );
    EXPECT_TRUE(rc.ok());

    rc = session->validateSessionIdentityForTest(
        app_test_support::beginString(),
        "WRONG",
        "CLIENT"
    );
    EXPECT_FALSE(rc.ok());

    rc = session->validateSessionIdentityForTest(
        app_test_support::beginString(),
        "SERVER",
        "WRONG"
    );
    EXPECT_FALSE(rc.ok());

    rc = session->validateSessionIdentityForTest(
        "FIX.4.2",
        "SERVER",
        "CLIENT"
    );
    EXPECT_FALSE(rc.ok());
#endif
}
