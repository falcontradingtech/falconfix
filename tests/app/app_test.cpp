// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <gtest/gtest.h>

#include <app/fix_application.h>

#include "app_test_support.h"

class MyApplication final : public falconfix::FIXApplication {
public:
    bool calledToApp{false};

    void onCreate(const falconfix::SessionID &sid) override {
        //GTEST_LOG_(INFO) << "[APP] onCreate: " << sid.toString();
    }

    void onLogon(const falconfix::SessionID &sid) override {
        //GTEST_LOG_(INFO) << "[APP] onLogon: " << sid.toString();
    }

    void onLogout(const falconfix::SessionID &sid) override {
        //GTEST_LOG_(INFO) << "[APP] onLogout: " << sid.toString();
    }

    FFStatus toAdmin(falconfix::FIXMessageRef msg,
                     const falconfix::SessionID &sid) noexcept override {
       // GTEST_LOG_(INFO) << "[APP] toAdmin: "
       //                  << sid.toString()
       //                   << " MsgType=" << msg.msgType;

        return FF_OK();
    }

    FFStatus fromAdmin(falconfix::FIXMessageRef msg,
                       const falconfix::SessionID &sid) noexcept override {
        // GTEST_LOG_(INFO) << "[APP] fromAdmin: "
        //                 << sid.toString()
        //                 << " MsgType=" << msg.msgType;

        return FF_OK();
    }

    FFStatus toApp(falconfix::FIXMessageRef msg,
                   const falconfix::SessionID &sid) noexcept override {
        // GTEST_LOG_(INFO) << "[APP] toApp: "
        //                 << sid.toString()
        //                 << " MsgType=" << msg.msgType;

        if (msg.is<app_test_support::TestMessage>()) {
            auto &typed = msg.as<app_test_support::TestMessage>();
            (void)typed;

            calledToApp = true;
        }

        return FF_OK();
    }

    FFStatus fromApp(falconfix::FIXMessageRef msg,
                     const falconfix::SessionID &sid) noexcept override {
        // GTEST_LOG_(INFO) << "[APP] fromApp: "
        //                 << sid.toString()
        //                 << " MsgType=" << msg.msgType;

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
