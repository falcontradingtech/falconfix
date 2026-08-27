// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <gtest/gtest.h>

#include <app/fix_application.h>

#include "app_test_support.h"

class MyApplication final : public falconfix::FIXApplication {
public:
    bool calledToApp{false};

    void onCreate(const falconfix::SessionID &sid) override {
        std::cout << "[APP] onCreate: " << sid.toString() << '\n';
    }

    void onLogon(const falconfix::SessionID &sid) override {
        std::cout << "[APP] onLogon: " << sid.toString() << '\n';
    }

    void onLogout(const falconfix::SessionID &sid) override {
        std::cout << "[APP] onLogout: " << sid.toString() << '\n';
    }

    FFStatus toAdmin(falconfix::FIXMessageRef msg,
                     const falconfix::SessionID &sid) noexcept override {
        std::cout << "[APP] toAdmin: "
                  << sid.toString()
                  << " MsgType=" << msg.msgType
                  << '\n';

        return FF_OK();
    }

    FFStatus fromAdmin(falconfix::FIXMessageRef msg,
                       const falconfix::SessionID &sid) noexcept override {
        std::cout << "[APP] fromAdmin: "
                  << sid.toString()
                  << " MsgType=" << msg.msgType
                  << '\n';

        return FF_OK();
    }

    FFStatus toApp(falconfix::FIXMessageRef msg,
                   const falconfix::SessionID &sid) noexcept override {
        std::cout << "[APP] toApp: "
                  << sid.toString()
                  << " MsgType=" << msg.msgType
                  << '\n';

        if (msg.is<app_test_support::TestMessage>()) {
            auto &typed = msg.as<app_test_support::TestMessage>();
            (void)typed;

            calledToApp = true;

            std::cout << "[APP] typed TestMessage received in toApp\n";
        }

        return FF_OK();
    }

    FFStatus fromApp(falconfix::FIXMessageRef msg,
                     const falconfix::SessionID &sid) noexcept override {
        std::cout << "[APP] fromApp: "
                  << sid.toString()
                  << " MsgType=" << msg.msgType
                  << '\n';

        return FF_OK();
    }
};

TEST(AppModelTests, SendToTargetCallsRegisteredToAppHandler) {
    MyApplication app;

    app_test_support::TestMessage msg;

    const falconfix::SessionID sid(
        app_test_support::beginString(),
        "CLIENT1",
        "SERVER1"
    );

    falconfix::FIXMessageRef view{};
    view.ptr = &msg;
    view.msgType = app_test_support::TestMessage::MSGTYPE;
    view.protocol = falconfix::protocolVersionOf<app_test_support::TestProtocol>();

    const auto rc = app.toApp(view, sid);
    ASSERT_TRUE(rc.ok()) << falconfix::errors::format_error(rc);

    EXPECT_TRUE(app.calledToApp);
}
