// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <gtest/gtest.h>

#include <atomic>
#include <FIX4.4/core/enums.h>
#include <FIX4.4/engine/outbound_prepare.h>
#include <FIX4.4/engine/tcp_frame_view.h>
#include <FIX4.4/messages/Logon.h>
#include <app/fix_application.h>
#include <connection/i_connection.h>
#include <logger/message_log.h>
#include <runtime/protocol_factory.h>
#include <store/memory_message_store.h>
#include <support/app_test_support.h>
#include <utils/fast_buffer.h>

#include <string>

namespace {

constexpr char kSOH = '\x01';

std::string makeConcatenatedFrames() {
    std::string out;

    out += "8=FIX4.4";
    out += kSOH;
    out += "9=000063";
    out += kSOH;
    out += "35=A";
    out += kSOH;
    out += "49=CLIENT";
    out += kSOH;
    out += "56=SERVER";
    out += kSOH;
    out += "34=1";
    out += kSOH;
    out += "52=20260511181904346";
    out += kSOH;
    out += "98=0";
    out += kSOH;
    out += "108=60";
    out += kSOH;
    out += "10=096";
    out += kSOH;

    out += "8=FIX4.4";
    out += kSOH;
    out += "9=000073";
    out += kSOH;
    out += "35=x";
    out += kSOH;
    out += "49=CLIENT";
    out += kSOH;
    out += "56=SERVER";
    out += kSOH;
    out += "34=2";
    out += kSOH;
    out += "52=20260511181904346";
    out += kSOH;
    out += "320=SECLIST_REQ";
    out += kSOH;
    out += "559=4";
    out += kSOH;
    out += "10=195";
    out += kSOH;

    return out;
}

class TestApp final : public falconfix::FIXApplication {
public:
    std::atomic<int> onLogonCount{0};

    void onCreate(const falconfix::SessionID &) override {}
    void onLogon(const falconfix::SessionID &) override { ++onLogonCount; }
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

std::string encodeInboundLogon(int64_t seqNum, const falconfix::SessionID &sid) {
    fix44::messages::Logon msg;
    msg.setEncryptMethod(0);
    msg.setHeartBtInt(30);

    FFStatus rc = fix44::engine::prepareOutbound(msg, seqNum, sid);
    EXPECT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    falconfix::FastBuffer out;
    rc = msg.toString(out);
    EXPECT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    return std::string(out.data(), out.size());
}

} // namespace

TEST(FIXSessionTests, TCPFrameViewParsesConcatenatedFramesSequentially) {
    const std::string data = makeConcatenatedFrames();

    fix44::engine::TCPFrameView first;
    FFStatus rc = fix44::engine::parseTCPFrame(data.data(), data.size(), first);

    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);
    EXPECT_EQ(first.begin, data.data());
    EXPECT_LT(first.end, data.data() + data.size());
    EXPECT_EQ(first.msgTypeEnum, fix44::enums::MsgType::LOGON);
    EXPECT_EQ(first.msgSeqNum, 1);
    EXPECT_EQ(*first.end, '8');

    fix44::engine::TCPFrameView second;
    const char *secondBegin = first.end;
    const std::size_t secondSize = static_cast<std::size_t>((data.data() + data.size()) - secondBegin);
    rc = fix44::engine::parseTCPFrame(secondBegin, secondSize, second);

    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);
    EXPECT_EQ(second.begin, first.end);
    EXPECT_EQ(second.end, data.data() + data.size());
    EXPECT_EQ(second.msgTypeEnum, fix44::enums::MsgType::SECURITY_LIST_REQUEST);
    EXPECT_EQ(second.msgSeqNum, 2);
}

TEST(FIXSessionTests, TCPFrameViewReportsIncompleteFrameWithoutParseError) {
    const std::string data = makeConcatenatedFrames();
    const std::size_t split = data.find("10=096");
    ASSERT_NE(split, std::string::npos);

    fix44::engine::TCPFrameView frame;
    FFStatus rc = fix44::engine::parseTCPFrame(data.data(), split, frame);

    EXPECT_TRUE(fix44::engine::isIncompleteTCPFrame(rc));
    EXPECT_EQ(rc.domain, falconfix::errors::Domain::socket);
    EXPECT_EQ(rc.code, falconfix::errors::code::socket::BUFFER_UNDERFLOW);
}

TEST(FIXSessionTests, SessionAccumulatesFragmentedTCPFrame) {
    TestApp app;
    TestConnection connection;

    falconfix::SessionConfig cfg;
    cfg.protocol.version = falconfix::ProtocolVersion::FIX44;
    cfg.connection.heartBtIntMs = 30000;
    cfg.connection.validateSendingTime = false;
    cfg.sessionID = falconfix::SessionID(app_test_support::beginString(), "SERVER_FRAG", "CLIENT_FRAG");

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

    const falconfix::SessionID clientSid(app_test_support::beginString(), "CLIENT_FRAG", "SERVER_FRAG");
    const int64_t expectedSeqNum = session->nextTargetSeqNumForTest();
    const std::string raw = encodeInboundLogon(expectedSeqNum, clientSid);
    const std::size_t split = raw.size() / 2;

    rc = session->onReceive(raw.data(), split);
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);
    EXPECT_EQ(app.onLogonCount.load(), 0);
    EXPECT_EQ(session->nextTargetSeqNumForTest(), expectedSeqNum);

    rc = session->onReceive(raw.data() + split, raw.size() - split);
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);
    EXPECT_EQ(app.onLogonCount.load(), 1);
    EXPECT_EQ(session->nextTargetSeqNumForTest(), expectedSeqNum + 1);
}
