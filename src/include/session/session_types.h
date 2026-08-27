// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <cstdint>

namespace falconfix {

enum class ConnectionType : uint8_t {
    Initiator = 0,
    Acceptor  = 1,
};

enum class StoreType : uint8_t {
    None = 0,
    Memory,
    File,
};

enum class LogType : uint8_t {
    None = 0,
    Screen,
    File,
};

enum class LogLevel : uint8_t {
    Trace,
    Debug,
    Info,
    Warn,
    Error,
    Critical,
    Off
};

} // namespace falconfix