// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <gtest/gtest.h>

#include <app/fix_application.h>
#include <runtime/protocol_factory.h>
#include <logger/message_log.h>
#include <session/session_id.h>
#include <session/session_settings.h>
#include <store/memory_message_store.h>

#include <FIX4.4/utils/datetime.h>

#include <support/app_test_support.h>

namespace {

class SendingTimeApp final : public falconfix::FIXApplication {
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

TEST(FIXSessionTests, RejectsExpiredSendingTime) {
    SendingTimeApp app;

    falconfix::SessionConfig cfg;
    cfg.protocol.version = falconfix::ProtocolVersion::FIX44;
    cfg.parameters["ValidateSendingTime"] = "Y";
    cfg.parameters["MaxLatencyMs"] = "1000";
    cfg.connection = falconfix::SessionConfig::Connection::fromParams(cfg.parameters);
    cfg.sessionID = falconfix::SessionID(
        app_test_support::beginString(),
        "CLIENT_ST",
        "SERVER_ST"
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

    const int64_t nowMs = fix44::datetime::detail::now_epoch_ms();

    FFStatus rc = session->validateSendingTimeForTest(nowMs);
    EXPECT_TRUE(rc.ok());

    rc = session->validateSendingTimeForTest(nowMs - 5000);
    EXPECT_FALSE(rc.ok());
    EXPECT_EQ(rc.reason, fix44::field_id::SendingTime);
}

TEST(FIXSessionTests, AcceptsValidSendingTime) {
    SendingTimeApp app;

    falconfix::SessionConfig cfg;
    cfg.protocol.version = falconfix::ProtocolVersion::FIX44;
    cfg.parameters["ValidateSendingTime"] = "Y";
    cfg.parameters["MaxLatencyMs"] = "1000";
    cfg.connection = falconfix::SessionConfig::Connection::fromParams(cfg.parameters);
    cfg.sessionID = falconfix::SessionID(
        app_test_support::beginString(),
        "CLIENT_ST_OK",
        "SERVER_ST_OK"
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

    const int64_t nowMs = fix44::datetime::detail::now_epoch_ms();

    FFStatus rc = session->validateSendingTimeForTest(nowMs);
    EXPECT_TRUE(rc.ok());

    rc = session->validateSendingTimeForTest(nowMs - 500);
    EXPECT_TRUE(rc.ok());

    rc = session->validateSendingTimeForTest(nowMs + 500);
}
