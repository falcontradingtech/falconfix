// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <app/fix_message_ref.h>

#include <config/fix_config.h>

#include <session/session_id.h>
#include <session/session_settings.h>

#include <utils/error_codes.h>

namespace falconfix {

class FIXApplication {
public:
    virtual ~FIXApplication() = default;

    virtual void onCreate(const SessionID &sessionID) = 0;
    virtual void onLogon(const SessionID &sessionID) = 0;
    virtual void onLogout(const SessionID &sessionID) = 0;

    virtual FFStatus toAdmin(FIXMessageRef message, const SessionID &sessionID) noexcept = 0;
    virtual FFStatus fromAdmin(FIXMessageRef message, const SessionID &sessionID) noexcept = 0;

    virtual FFStatus toApp(FIXMessageRef message, const SessionID &sessionID) noexcept = 0;
    virtual FFStatus fromApp(FIXMessageRef message, const SessionID &sessionID) noexcept = 0;
};

} // namespace falconfix
