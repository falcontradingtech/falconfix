// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <gtest/gtest.h>

#include <atomic>
#include <string>

#include <app/fix_application.h>
#include <runtime/protocol_factory.h>

#include <FIX4.4/engine/outbound_prepare.h>
#include <FIX4.4/messages/SecurityListRequest.h>
#include <connection/i_connection.h>
#include <logger/message_log.h>
#include <session/session_id.h>
#include <session/session_settings.h>
#include <store/memory_message_store.h>
#include <utils/fast_buffer.h>

#include <support/app_test_support.h>

namespace {

constexpr char kSOH = '\x01';

class TestApp final : public falconfix::FIXApplication {
public:
    std::atomic<bool> lastPossResend{false};
    std::atomic<bool> lastPossDup{false};

    void onCreate(const falconfix::SessionID &) override {}
    void onLogon(const falconfix::SessionID &) override {}
    void onLogout(const falconfix::SessionID &) override {}

    FFStatus toAdmin(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override { return FF_OK(); }
    FFStatus fromAdmin(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override { return FF_OK(); }
    FFStatus toApp(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override { return FF_OK(); }
    FFStatus fromApp(falconfix::FIXMessageRef msg, const falconfix::SessionID &) noexcept override {
        lastPossResend.store(msg.possResend, std::memory_order_release);
        lastPossDup.store(msg.possDup, std::memory_order_release);
        return FF_OK();
    }
};

class TestConnection final : public falconfix::IConnection {
public:
    FFStatus start() noexcept override { return FF_OK(); }
    FFStatus write(const char *, std::size_t) noexcept override { return FF_OK(); }
    FFStatus writeAsync(const char *, std::size_t) noexcept override { return FF_OK(); }
    void close() noexcept override {}
    void detach() noexcept override {}
    FFStatus onTimer() noexcept override {  return FF_OK(); }
};

template<typename Msg>
std::string encodeInboundMessage(Msg &msg,
                                 int64_t seqNum,
                                 const falconfix::SessionID &sid) {
    FFStatus rc = fix44::engine::prepareOutbound(msg, seqNum, sid);
    EXPECT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    falconfix::FastBuffer out;
    rc = msg.toString(out);
    EXPECT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    return std::string(out.data(), out.size());
}

std::string makeSecurityListRequestRaw(int64_t seqNum,
                                       const falconfix::SessionID &sid,
                                       bool possResend,
                                       bool possDup) {
    fix44::messages::SecurityListRequest msg;

    fix44::types::STRING<32> reqId;
    reqId.assign("SECLIST_REQ", 11);
    msg.setSecurityReqID(reqId);
    msg.setSecurityListRequestType(4);
    if (possDup) {
        msg.getRefHeader().setPossDupFlag(true);
    }
    if (possResend) {
        msg.getRefHeader().setPossResend(true);
    }

    return encodeInboundMessage(msg, seqNum, sid);
}

std::unique_ptr<falconfix::SessionBase> makeBoundSession(TestApp &app,
                                                         TestConnection &connection,
                                                         std::string_view sender,
                                                         std::string_view target) {
    falconfix::SessionConfig cfg;
    cfg.protocol.version = falconfix::ProtocolVersion::FIX44;
    cfg.connection.heartBtIntMs = 30000;
    cfg.connection.validateSendingTime = false;
    cfg.sessionID = falconfix::SessionID(app_test_support::beginString(), sender, target);

    auto session = falconfix::ProtocolFactory::createSession(
        app,
        cfg,
        std::make_unique<falconfix::NullMessageLog>(),
        std::make_unique<falconfix::MemoryMessageStore>()
    );
    EXPECT_NE(session, nullptr);
    if (!session) {
        return nullptr;
    }

    session->bindConnection(connection);
    FFStatus rc = session->onConnected();
    EXPECT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);
    return session;
}

} // namespace

TEST(FIXSessionTests, PropagatesPossResendFlagToApplication) {
    TestApp app;
    TestConnection connection;
    auto session = makeBoundSession(app, connection, "SERVER_PR", "CLIENT_PR");
    ASSERT_NE(session, nullptr);

    const falconfix::SessionID clientSid(app_test_support::beginString(), "CLIENT_PR", "SERVER_PR");
    const int64_t expectedSeqNum = session->nextTargetSeqNumForTest();

    std::string raw = makeSecurityListRequestRaw(expectedSeqNum, clientSid, true, false);
    FFStatus rc = session->onReceive(raw.data(), raw.size());

    EXPECT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);
    EXPECT_TRUE(app.lastPossResend.load(std::memory_order_acquire));
    EXPECT_FALSE(app.lastPossDup.load(std::memory_order_acquire));
}

TEST(FIXSessionTests, PropagatesPossDupFlagToApplication) {
#ifndef NDEFALCONFIX_ENABLE_TEST_APIBUG
    TestApp app;
    TestConnection connection;
    auto session = makeBoundSession(app, connection, "SERVER_PD", "CLIENT_PD");
    ASSERT_NE(session, nullptr);

    const falconfix::SessionID clientSid(app_test_support::beginString(), "CLIENT_PD", "SERVER_PD");
    const int64_t expectedSeqNum = session->nextTargetSeqNumForTest();

    std::string raw = makeSecurityListRequestRaw(expectedSeqNum, clientSid, false, true);
    FFStatus rc = session->onReceive(raw.data(), raw.size());

    EXPECT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);
    EXPECT_FALSE(app.lastPossResend.load(std::memory_order_acquire));
    EXPECT_TRUE(app.lastPossDup.load(std::memory_order_acquire));
#endif
}
