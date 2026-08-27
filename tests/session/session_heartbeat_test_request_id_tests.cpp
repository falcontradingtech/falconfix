// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <gtest/gtest.h>

#include <string>

#include <app/fix_application.h>
#include <runtime/protocol_factory.h>

#include <FIX4.4/engine/outbound_prepare.h>
#include <FIX4.4/messages/Heartbeat.h>
#include <connection/i_connection.h>
#include <logger/message_log.h>
#include <store/memory_message_store.h>
#include <utils/fast_buffer.h>

#include <support/app_test_support.h>

namespace {

class TestApp final : public falconfix::FIXApplication {
public:
    void onCreate(const falconfix::SessionID &) override {}
    void onLogon(const falconfix::SessionID &) override {}
    void onLogout(const falconfix::SessionID &) override {}

    FFStatus toAdmin(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override { return FF_OK(); }
    FFStatus fromAdmin(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override { return FF_OK(); }
    FFStatus toApp(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override { return FF_OK(); }
    FFStatus fromApp(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override { return FF_OK(); }
};

class TestConnection final : public falconfix::IConnection {
public:
    FFStatus start() noexcept override { return FF_OK(); }
    FFStatus write(const char *, std::size_t) noexcept override { return FF_OK(); }
    FFStatus writeAsync(const char *, std::size_t) noexcept override { return FF_OK(); }
    void close() noexcept override {}
    void detach() noexcept override {}
    FFStatus onTimer() noexcept override { return FF_OK(); }
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

fix44::types::STRING<32> makeTestReqID(const char *value, std::size_t size) {
    fix44::types::STRING<32> id;
    id.assign(value, size);
    return id;
}

} // namespace

TEST(FIXSessionTests, HeartbeatWithWrongTestReqIDDoesNotSatisfyTestRequest) {
    TestApp app;
    TestConnection connection;
    auto session = makeBoundSession(app, connection, "SERVER_HB_WRONG", "CLIENT_HB_WRONG");
    ASSERT_NE(session, nullptr);

    session->setAwaitingTestResponseForTest(true, 123);
    ASSERT_TRUE(session->awaitingTestResponseForTest());
    ASSERT_EQ(session->testRequestIDForTest(), 123U);

    const falconfix::SessionID clientSid(app_test_support::beginString(), "CLIENT_HB_WRONG", "SERVER_HB_WRONG");

    fix44::messages::Heartbeat hb;
    hb.setTestReqID(makeTestReqID("999", 3));

    auto raw = encodeInboundMessage(hb, session->nextTargetSeqNumForTest(), clientSid);
    FFStatus rc = session->onReceive(raw.data(), raw.size());

    EXPECT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);
    EXPECT_TRUE(session->awaitingTestResponseForTest());
    EXPECT_EQ(session->testRequestIDForTest(), 123U);
}

TEST(FIXSessionTests, HeartbeatWithoutTestReqIDDoesNotSatisfyTestRequest) {
    TestApp app;
    TestConnection connection;
    auto session = makeBoundSession(app, connection, "SERVER_HB_NOID", "CLIENT_HB_NOID");
    ASSERT_NE(session, nullptr);

    session->setAwaitingTestResponseForTest(true, 123);
    ASSERT_TRUE(session->awaitingTestResponseForTest());
    ASSERT_EQ(session->testRequestIDForTest(), 123U);

    const falconfix::SessionID clientSid(app_test_support::beginString(), "CLIENT_HB_NOID", "SERVER_HB_NOID");

    fix44::messages::Heartbeat hb;

    auto raw = encodeInboundMessage(hb, session->nextTargetSeqNumForTest(), clientSid);
    FFStatus rc = session->onReceive(raw.data(), raw.size());

    EXPECT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);
    EXPECT_TRUE(session->awaitingTestResponseForTest());
    EXPECT_EQ(session->testRequestIDForTest(), 123U);
}

TEST(FIXSessionTests, HeartbeatWithMatchingTestReqIDSatisfiesTestRequest) {
    TestApp app;
    TestConnection connection;
    auto session = makeBoundSession(app, connection, "SERVER_HB_MATCH", "CLIENT_HB_MATCH");
    ASSERT_NE(session, nullptr);

    session->setAwaitingTestResponseForTest(true, 123);
    ASSERT_TRUE(session->awaitingTestResponseForTest());
    ASSERT_EQ(session->testRequestIDForTest(), 123U);

    const falconfix::SessionID clientSid(app_test_support::beginString(), "CLIENT_HB_MATCH", "SERVER_HB_MATCH");

    fix44::messages::Heartbeat hb;
    hb.setTestReqID(makeTestReqID("123", 3));

    auto raw = encodeInboundMessage(hb, session->nextTargetSeqNumForTest(), clientSid);
    FFStatus rc = session->onReceive(raw.data(), raw.size());

    EXPECT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);
    EXPECT_FALSE(session->awaitingTestResponseForTest());
    EXPECT_EQ(session->testRequestIDForTest(), 123U);
}
