// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <gtest/gtest.h>

#include <atomic>
#include <string>
#include <vector>

#include <app/fix_application.h>
#include <runtime/protocol_factory.h>

#include <FIX4.4/engine/outbound_prepare.h>
#include <FIX4.4/messages/Logon.h>
#include <connection/i_connection.h>
#include <logger/message_log.h>
#include <store/memory_message_store.h>
#include <utils/fast_buffer.h>

#include <support/app_test_support.h>

namespace {

class TestApp final : public falconfix::FIXApplication {
public:
    std::atomic<int> onLogonCount{0};
    std::atomic<int> onLogoutCount{0};

    void onCreate(const falconfix::SessionID &) override {}
    void onLogon(const falconfix::SessionID &) override { ++onLogonCount; }
    void onLogout(const falconfix::SessionID &) override { ++onLogoutCount; }

    FFStatus toAdmin(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override { return FF_OK(); }
    FFStatus fromAdmin(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override { return FF_OK(); }
    FFStatus toApp(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override { return FF_OK(); }
    FFStatus fromApp(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override { return FF_OK(); }
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

class CapturingConnection final : public falconfix::IConnection {
public:
    std::vector<std::string> writes;
    bool closeCalled{false};

    FFStatus start() noexcept override { return FF_OK(); }

    FFStatus write(const char *data, std::size_t size) noexcept override {
        writes.emplace_back(data, size);
        return FF_OK();
    }

    FFStatus writeAsync(const char *data, std::size_t size) noexcept override {
        writes.emplace_back(data, size);
        return FF_OK();
    }

    void close() noexcept override { closeCalled = true; }
    void detach() noexcept override {}
    FFStatus onTimer() noexcept override { return FF_OK(); }
};

} // namespace

TEST(FIXSessionTests, DuplicateLogonTriggersLogoutAndDisconnect) {
    TestApp app;
    CapturingConnection connection;

    falconfix::SessionConfig cfg;
    cfg.protocol.version = falconfix::ProtocolVersion::FIX44;
    cfg.connection.heartBtIntMs = 30000;
    cfg.connection.validateSendingTime = false;
    cfg.sessionID = falconfix::SessionID(
        app_test_support::beginString(),
        "SERVER_DUP",
        "CLIENT_DUP"
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

    session->bindConnection(connection);

    FFStatus rc = session->onConnected();
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);
    ASSERT_EQ(connection.writes.size(), 1U);

    const falconfix::SessionID clientSid(app_test_support::beginString(), "CLIENT_DUP", "SERVER_DUP");

    fix44::messages::Logon firstLogon;
    firstLogon.setEncryptMethod(0);
    firstLogon.setHeartBtInt(static_cast<fix44::types::INT>(30));

    auto firstRaw = encodeInboundMessage(firstLogon, session->nextTargetSeqNumForTest(), clientSid);
    rc = session->onReceive(firstRaw.data(), firstRaw.size());
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    EXPECT_EQ(app.onLogonCount.load(), 1);
    ASSERT_EQ(connection.writes.size(), 1U);

    fix44::messages::Logon duplicateLogon;
    duplicateLogon.setEncryptMethod(0);
    duplicateLogon.setHeartBtInt(static_cast<fix44::types::INT>(30));

    auto duplicateRaw = encodeInboundMessage(duplicateLogon, session->nextTargetSeqNumForTest(), clientSid);
    rc = session->onReceive(duplicateRaw.data(), duplicateRaw.size());

    EXPECT_FALSE(rc.ok());
    EXPECT_TRUE(connection.closeCalled);
    ASSERT_EQ(connection.writes.size(), 2U);
    EXPECT_NE(connection.writes[1].find("35=5"), std::string::npos);
    EXPECT_NE(connection.writes[1].find("duplicate logon"), std::string::npos);
    EXPECT_EQ(app.onLogonCount.load(), 1);
    EXPECT_EQ(app.onLogoutCount.load(), 0);
}
