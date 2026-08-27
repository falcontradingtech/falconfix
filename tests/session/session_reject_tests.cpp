// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <gtest/gtest.h>

#include <atomic>
#include <string>
#include <vector>
#include <app/fix_application.h>
#include <runtime/protocol_factory.h>

#include <FIX4.4/engine/outbound_prepare.h>
#include <FIX4.4/messages/BusinessMessageReject.h>
#include <FIX4.4/messages/Reject.h>
#include <connection/i_connection.h>
#include <logger/message_log.h>
#include <session/session_id.h>
#include <session/session_settings.h>
#include <store/memory_message_store.h>
#include <utils/fast_buffer.h>

#include <support/app_test_support.h>

namespace {

class TestApp final : public falconfix::FIXApplication {
public:
    std::atomic<int> onLogonCount{0};
    std::atomic<int> onLogoutCount{0};
    std::atomic<int> rejectInCount{0};
    std::atomic<int> businessRejectInCount{0};

    void onCreate(const falconfix::SessionID &) override {}
    void onLogon(const falconfix::SessionID &) override { ++onLogonCount; }
    void onLogout(const falconfix::SessionID &) override { ++onLogoutCount; }

    FFStatus toAdmin(falconfix::FIXMessageRef, const falconfix::SessionID &) noexcept override { return FF_OK(); }
    FFStatus fromAdmin(falconfix::FIXMessageRef msg, const falconfix::SessionID &) noexcept override {
        if (msg.is<fix44::messages::Reject>()) {
            ++rejectInCount;
        }
        if (msg.is<fix44::messages::BusinessMessageReject>()) {
            ++businessRejectInCount;
        }
        return FF_OK();
    }
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

std::string buildRawInboundMessage(std::initializer_list<std::pair<int, std::string_view>> fields) {
    constexpr char soh = '\x01';

    std::string body;
    for (const auto &[tag, value] : fields) {
        body += std::to_string(tag);
        body += '=';
        body.append(value.data(), value.size());
        body.push_back(soh);
    }

    std::string raw = "8=" + app_test_support::beginString();
    raw.push_back(soh);
    raw += "9=" + std::to_string(body.size());
    raw.push_back(soh);
    raw += body;
    raw += "10=000";
    raw.push_back(soh);

    return raw;
}

class TestConnection final : public falconfix::IConnection {
public:
    std::vector<std::string> writes;

    FFStatus start() noexcept override { return FF_OK(); }
    FFStatus write(const char *data, std::size_t size) noexcept override {
        writes.emplace_back(data, size);
        return FF_OK();
    }
    FFStatus writeAsync(const char *data, std::size_t size) noexcept override {
        writes.emplace_back(data, size);
        return FF_OK();
    }
    void close() noexcept override {}
    void detach() noexcept override {}
    FFStatus onTimer() noexcept override {  return FF_OK(); }
};

} // namespace

TEST(FIXSessionTests, HandlesRejectInbound) {
    TestApp app;
    TestConnection connection;

    falconfix::SessionConfig cfg;
    cfg.protocol.version = falconfix::ProtocolVersion::FIX44;
    cfg.connection.heartBtIntMs = 30000;
    cfg.connection.validateSendingTime = false;
    cfg.sessionID = falconfix::SessionID(
        app_test_support::beginString(),
        "SERVER_RJ",
        "CLIENT_RJ"
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

    const falconfix::SessionID clientSid(app_test_support::beginString(), "CLIENT_RJ", "SERVER_RJ");
    const int64_t expectedSeqNum = session->nextTargetSeqNumForTest();

    fix44::messages::Reject msg;
    msg.setRefSeqNum(static_cast<fix44::types::INT>(1));

    auto raw = encodeInboundMessage(msg, expectedSeqNum, clientSid);
    rc = session->onReceive(raw.data(), raw.size());

    EXPECT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);
    EXPECT_EQ(app.rejectInCount.load(), 1);
    EXPECT_EQ(app.businessRejectInCount.load(), 0);
    EXPECT_EQ(app.onLogoutCount.load(), 0);
    EXPECT_EQ(session->nextTargetSeqNumForTest(), expectedSeqNum + 1);
}

TEST(FIXSessionTests, HandlesBusinessMessageRejectInbound) {
    TestApp app;
    TestConnection connection;

    falconfix::SessionConfig cfg;
    cfg.protocol.version = falconfix::ProtocolVersion::FIX44;
    cfg.connection.heartBtIntMs = 30000;
    cfg.connection.validateSendingTime = false;
    cfg.sessionID = falconfix::SessionID(
        app_test_support::beginString(),
        "SERVER_BRJ",
        "CLIENT_BRJ"
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

    const falconfix::SessionID clientSid(app_test_support::beginString(), "CLIENT_BRJ", "SERVER_BRJ");
    const int64_t expectedSeqNum = session->nextTargetSeqNumForTest();

    fix44::messages::BusinessMessageReject msg;
    fix44::types::STRING<32> refMsgType;
    refMsgType.assign("D", 1);
    msg.setRefMsgType(refMsgType);
    msg.setBusinessRejectReason(static_cast<fix44::types::INT>(0));

    auto raw = encodeInboundMessage(msg, expectedSeqNum, clientSid);
    rc = session->onReceive(raw.data(), raw.size());

    EXPECT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);
    EXPECT_EQ(app.rejectInCount.load(), 0);
    EXPECT_EQ(app.businessRejectInCount.load(), 1);
    EXPECT_EQ(app.onLogoutCount.load(), 0);
    EXPECT_EQ(session->nextTargetSeqNumForTest(), expectedSeqNum + 1);
}

TEST(FIXSessionTests, UnsupportedMsgTypeTriggersSessionReject) {
    TestApp app;
    TestConnection connection;

    falconfix::SessionConfig cfg;
    cfg.protocol.version = falconfix::ProtocolVersion::FIX44;
    cfg.connection.heartBtIntMs = 30000;
    cfg.connection.validateSendingTime = false;
    cfg.sessionID = falconfix::SessionID(
        app_test_support::beginString(),
        "SERVER_BADMT",
        "CLIENT_BADMT"
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

    auto raw = buildRawInboundMessage({
        {35, "!"},
        {49, "CLIENT_BADMT"},
        {56, "SERVER_BADMT"},
        {34, "1"}
    });

    rc = session->onReceive(raw.data(), raw.size());

    EXPECT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    ASSERT_EQ(connection.writes.size(), 2U);

    fix44::messages::Reject reject;
    const char *p = connection.writes[1].data();
    const char *end = p + connection.writes[1].size();
    FFStatus decodeRc = reject.setString(p, end);
    ASSERT_TRUE(decodeRc.ok()) << falconfix::errors::format_error(decodeRc);

    ASSERT_TRUE(reject.hasRefSeqNum());
    EXPECT_EQ(reject.getRefSeqNum(), 1);
    ASSERT_TRUE(reject.hasRefTagID());
    EXPECT_EQ(reject.getRefTagID(), fix44::field_id::MsgType);
    ASSERT_TRUE(reject.hasSessionRejectReason());
    EXPECT_EQ(
        reject.getSessionRejectReason(),
        static_cast<fix44::types::INT>(fix44::enums::SessionRejectReason::INVALID_MSGTYPE)
    );
    ASSERT_TRUE(reject.hasText());
    EXPECT_EQ(reject.getText().view(), "unsupported message type");
}

TEST(FIXSessionTests, InvalidTagNumberTriggersSessionReject) {
    TestApp app;
    TestConnection connection;

    falconfix::SessionConfig cfg;
    cfg.protocol.version = falconfix::ProtocolVersion::FIX44;
    cfg.connection.heartBtIntMs = 30000;
    cfg.connection.validateSendingTime = false;
    cfg.sessionID = falconfix::SessionID(
        app_test_support::beginString(),
        "SERVER_BADTAG",
        "CLIENT_BADTAG"
    );

    auto session = falconfix::ProtocolFactory::createSession(
        app,
        cfg,
        std::make_unique<falconfix::NullMessageLog>(),
        std::make_unique<falconfix::MemoryMessageStore>()
    );
    ASSERT_NE(session, nullptr);

    session->bindConnection(connection);
    FFStatus rc = session->onConnected();
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);
    ASSERT_EQ(connection.writes.size(), 1U);

    auto raw = buildRawInboundMessage({
        {35, "0"},
        {49, "CLIENT_BADTAG"},
        {56, "SERVER_BADTAG"},
        {34, "1"},
        {9999, "BOOM"}
    });

    rc = session->onReceive(raw.data(), raw.size());

    EXPECT_FALSE(rc.ok());
    EXPECT_EQ(rc.domain, falconfix::errors::Domain::fix_parse);
    EXPECT_EQ(rc.code, falconfix::errors::code::fix::parse::UNKNOWN_FIELD_ID);
    EXPECT_EQ(rc.tag, 9999);

    ASSERT_EQ(connection.writes.size(), 2U);

    fix44::messages::Reject reject;
    const char *p = connection.writes[1].data();
    const char *end = p + connection.writes[1].size();
    FFStatus decodeRc = reject.setString(p, end);
    ASSERT_TRUE(decodeRc.ok()) << falconfix::errors::format_error(decodeRc);

    EXPECT_EQ(reject.getRefSeqNum(), 1);
    EXPECT_EQ(reject.getRefTagID(), 9999);
    EXPECT_EQ(
        reject.getSessionRejectReason(),
        static_cast<fix44::types::INT>(fix44::enums::SessionRejectReason::INVALID_TAG_NUMBER)
    );
    EXPECT_NE(reject.getText().view().find("code=-4000"), std::string_view::npos);
}

TEST(FIXSessionTests, RequiredTagMissingTriggersSessionReject) {
    TestApp app;
    TestConnection connection;

    falconfix::SessionConfig cfg;
    cfg.protocol.version = falconfix::ProtocolVersion::FIX44;
    cfg.connection.heartBtIntMs = 30000;
    cfg.connection.validateSendingTime = false;
    cfg.sessionID = falconfix::SessionID(
        app_test_support::beginString(),
        "SERVER_MISSREQ",
        "CLIENT_MISSREQ"
    );

    auto session = falconfix::ProtocolFactory::createSession(
        app,
        cfg,
        std::make_unique<falconfix::NullMessageLog>(),
        std::make_unique<falconfix::MemoryMessageStore>()
    );
    ASSERT_NE(session, nullptr);

    session->bindConnection(connection);
    FFStatus rc = session->onConnected();
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);
    ASSERT_EQ(connection.writes.size(), 1U);

    auto raw = buildRawInboundMessage({
        {35, "0"},
        {56, "SERVER_MISSREQ"},
        {34, "1"}
    });

    rc = session->onReceive(raw.data(), raw.size());

    EXPECT_FALSE(rc.ok());
    EXPECT_EQ(rc.domain, falconfix::errors::Domain::fix_validation);
    EXPECT_EQ(rc.code, falconfix::errors::code::fix::validation::MISSING_REQUIRED);
    EXPECT_EQ(rc.tag, fix44::field_id::SenderCompID);

    ASSERT_EQ(connection.writes.size(), 2U);

    fix44::messages::Reject reject;
    const char *p = connection.writes[1].data();
    const char *end = p + connection.writes[1].size();
    FFStatus decodeRc = reject.setString(p, end);
    ASSERT_TRUE(decodeRc.ok()) << falconfix::errors::format_error(decodeRc);

    EXPECT_EQ(reject.getRefSeqNum(), 1);
    EXPECT_EQ(reject.getRefTagID(), fix44::field_id::SenderCompID);
    EXPECT_EQ(
        reject.getSessionRejectReason(),
        static_cast<fix44::types::INT>(fix44::enums::SessionRejectReason::REQUIRED_TAG_MISSING)
    );
    EXPECT_NE(reject.getText().view().find("code=-4200"), std::string_view::npos);
}

TEST(FIXSessionTests, TagNotDefinedForMessageTriggersSessionReject) {
    TestApp app;
    TestConnection connection;

    falconfix::SessionConfig cfg;
    cfg.protocol.version = falconfix::ProtocolVersion::FIX44;
    cfg.connection.heartBtIntMs = 30000;
    cfg.connection.validateSendingTime = false;
    cfg.sessionID = falconfix::SessionID(
        app_test_support::beginString(),
        "SERVER_BADFIELD",
        "CLIENT_BADFIELD"
    );

    auto session = falconfix::ProtocolFactory::createSession(
        app,
        cfg,
        std::make_unique<falconfix::NullMessageLog>(),
        std::make_unique<falconfix::MemoryMessageStore>()
    );
    ASSERT_NE(session, nullptr);

    session->bindConnection(connection);
    FFStatus rc = session->onConnected();
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);
    ASSERT_EQ(connection.writes.size(), 1U);

    auto raw = buildRawInboundMessage({
        {35, "0"},
        {49, "CLIENT_BADFIELD"},
        {56, "SERVER_BADFIELD"},
        {34, "1"},
        {98, "0"}
    });

    rc = session->onReceive(raw.data(), raw.size());

    EXPECT_FALSE(rc.ok());
    EXPECT_EQ(rc.domain, falconfix::errors::Domain::fix_parse);
    EXPECT_EQ(rc.code, falconfix::errors::code::fix::parse::UNEXPECTED_FIELD);
    EXPECT_EQ(rc.tag, fix44::field_id::EncryptMethod);

    ASSERT_EQ(connection.writes.size(), 2U);

    fix44::messages::Reject reject;
    const char *p = connection.writes[1].data();
    const char *end = p + connection.writes[1].size();
    FFStatus decodeRc = reject.setString(p, end);
    ASSERT_TRUE(decodeRc.ok()) << falconfix::errors::format_error(decodeRc);

    EXPECT_EQ(reject.getRefSeqNum(), 1);
    EXPECT_EQ(reject.getRefTagID(), fix44::field_id::EncryptMethod);
    EXPECT_EQ(
        reject.getSessionRejectReason(),
        static_cast<fix44::types::INT>(fix44::enums::SessionRejectReason::TAG_NOT_DEFINED_FOR_THIS_MESSAGE_TYPE)
    );
    EXPECT_NE(reject.getText().view().find("code=-4002"), std::string_view::npos);
}

TEST(FIXSessionTests, IncorrectDataFormatTriggersSessionReject) {
    TestApp app;
    TestConnection connection;

    falconfix::SessionConfig cfg;
    cfg.protocol.version = falconfix::ProtocolVersion::FIX44;
    cfg.connection.heartBtIntMs = 30000;
    cfg.connection.validateSendingTime = false;
    cfg.sessionID = falconfix::SessionID(
        app_test_support::beginString(),
        "SERVER_BADTIME",
        "CLIENT_BADTIME"
    );

    auto session = falconfix::ProtocolFactory::createSession(
        app,
        cfg,
        std::make_unique<falconfix::NullMessageLog>(),
        std::make_unique<falconfix::MemoryMessageStore>()
    );
    ASSERT_NE(session, nullptr);

    session->bindConnection(connection);
    FFStatus rc = session->onConnected();
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);
    ASSERT_EQ(connection.writes.size(), 1U);

    auto raw = buildRawInboundMessage({
        {35, "0"},
        {49, "CLIENT_BADTIME"},
        {56, "SERVER_BADTIME"},
        {34, "1"},
        {52, "NOT_A_TIMESTAMP"}
    });

    rc = session->onReceive(raw.data(), raw.size());

    EXPECT_FALSE(rc.ok());
    EXPECT_EQ(rc.domain, falconfix::errors::Domain::fix_parse);
    EXPECT_EQ(rc.code, falconfix::errors::code::fix::parse::DECODE_FAILED);
    EXPECT_EQ(rc.tag, fix44::field_id::SendingTime);

    ASSERT_EQ(connection.writes.size(), 2U);

    fix44::messages::Reject reject;
    const char *p = connection.writes[1].data();
    const char *end = p + connection.writes[1].size();
    FFStatus decodeRc = reject.setString(p, end);
    ASSERT_TRUE(decodeRc.ok()) << falconfix::errors::format_error(decodeRc);

    EXPECT_EQ(reject.getRefSeqNum(), 1);
    EXPECT_EQ(reject.getRefTagID(), fix44::field_id::SendingTime);
    EXPECT_EQ(
        reject.getSessionRejectReason(),
        static_cast<fix44::types::INT>(fix44::enums::SessionRejectReason::INCORRECT_DATA_FORMAT_FOR_VALUE)
    );
    EXPECT_NE(reject.getText().view().find("code=-4008"), std::string_view::npos);
}
