// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <memory>

#include <app/fix_application.h>
#include <logger/message_log.h>
#include <store/message_store.h>
#include <session/session_base.h>
#include <session/session_settings.h>

#if defined(FALCONFIX_ENABLE_FIX40)
#include <FIX4.0/engine/session.h>
#include <FIX4.0/engine/protocol.h>
#endif

#if defined(FALCONFIX_ENABLE_FIX41)
#include <FIX4.1/engine/session.h>
#include <FIX4.1/engine/protocol.h>
#endif

#if defined(FALCONFIX_ENABLE_FIX42)
#include <FIX4.2/engine/session.h>
#include <FIX4.2/engine/protocol.h>
#endif

#if defined(FALCONFIX_ENABLE_FIX43)
#include <FIX4.3/engine/session.h>
#include <FIX4.3/engine/protocol.h>
#endif

#if defined(FALCONFIX_ENABLE_FIX44)
#include <FIX4.4/engine/session.h>
#include <FIX4.4/engine/protocol.h>
#endif

#if defined(FALCONFIX_ENABLE_FIX50)
#include <FIX5.0/engine/session.h>
#include <FIX5.0/engine/protocol.h>
#endif

#if defined(FALCONFIX_ENABLE_FIX50SP1)
#include <FIX5.0SP1/engine/session.h>
#include <FIX5.0SP1/engine/protocol.h>
#endif

#if defined(FALCONFIX_ENABLE_FIX50SP2)
#include <FIX5.0SP2/engine/session.h>
#include <FIX5.0SP2/engine/protocol.h>
#endif

#if defined(FALCONFIX_ENABLE_FIXT11)
#include <FIXT1.1/engine/session.h>
#include <FIXT1.1/engine/protocol.h>
#endif

namespace falconfix {

class ProtocolFactory {
public:
    static std::unique_ptr<SessionBase>
    createSession(FIXApplication &app,
                  const SessionConfig &cfg,
                  std::unique_ptr<MessageLog> log,
                  std::unique_ptr<MessageStore> store) {

#if defined(FALCONFIX_ENABLE_FIX40)
        if (cfg.sessionID.beginString() == fix40::Protocol::BEGIN_STRING) {
            return fix40::engine::makeSession(
                app,
                cfg,
                std::move(log),
                std::move(store)
            );
        }
#endif

#if defined(FALCONFIX_ENABLE_FIX41)
        if (cfg.sessionID.beginString() == fix41::Protocol::BEGIN_STRING) {
            return fix41::engine::makeSession(
                app,
                cfg,
                std::move(log),
                std::move(store)
            );
        }
#endif

#if defined(FALCONFIX_ENABLE_FIX42)
        if (cfg.sessionID.beginString() == fix42::Protocol::BEGIN_STRING) {
            return fix42::engine::makeSession(
                app,
                cfg,
                std::move(log),
                std::move(store)
            );
        }
#endif

#if defined(FALCONFIX_ENABLE_FIX43)
        if (cfg.sessionID.beginString() == fix43::Protocol::BEGIN_STRING) {
            return fix43::engine::makeSession(
                app,
                cfg,
                std::move(log),
                std::move(store)
            );
        }
#endif

#if defined(FALCONFIX_ENABLE_FIX44)
        if (cfg.protocol.version == ProtocolVersion::FIX44) {
            return fix44::engine::makeSession(
                app,
                cfg,
                std::move(log),
                std::move(store)
            );
        }
#endif

#if defined(FALCONFIX_ENABLE_FIX50)
        if (cfg.sessionID.beginString() == fix50::Protocol::BEGIN_STRING) {
            return fix50::engine::makeSession(
                app,
                cfg,
                std::move(log),
                std::move(store)
            );
        }
#endif

#if defined(FALCONFIX_ENABLE_FIX50SP1)
        if (cfg.sessionID.beginString() == fix50sp1::Protocol::BEGIN_STRING) {
            return fix50sp1::engine::makeSession(
                app,
                cfg,
                std::move(log),
                std::move(store)
            );
        }
#endif

#if defined(FALCONFIX_ENABLE_FIX50SP2)
        if (cfg.sessionID.beginString() == fix50sp2::Protocol::BEGIN_STRING) {
            return fix50sp2::engine::makeSession(
                app,
                cfg,
                std::move(log),
                std::move(store)
            );
        }
#endif

#if defined(FALCONFIX_ENABLE_FIXT11)
        if (cfg.sessionID.beginString() == fixt11::Protocol::BEGIN_STRING) {
            return fixt11::engine::makeSession(
                app,
                cfg,
                std::move(log),
                std::move(store)
            );
        }
#endif

        return nullptr;
    }
};

} // namespace falconfix
