// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#if defined(FALCONFIX_ENABLE_FIX44)
#include <FIX4.4/engine/protocol.h>
namespace falconfix::test {
using DefaultProtocol = fix44::Protocol;
}
#elif defined(FALCONFIX_ENABLE_FIX43)
#include <FIX4.3/engine/protocol.h>
namespace falconfix::test {
using DefaultProtocol = fix43::Protocol;
}
#elif defined(FALCONFIX_ENABLE_FIX42)
#include <FIX4.2/engine/protocol.h>
namespace falconfix::test {
using DefaultProtocol = fix42::Protocol;
}
#elif defined(FALCONFIX_ENABLE_FIX41)
#include <FIX4.1/engine/protocol.h>
namespace falconfix::test {
using DefaultProtocol = fix41::Protocol;
}
#elif defined(FALCONFIX_ENABLE_FIX40)
#include <FIX4.0/engine/protocol.h>
namespace falconfix::test {
using DefaultProtocol = fix40::Protocol;
}
#elif defined(FALCONFIX_ENABLE_FIX50)
#include <FIX5.0/engine/protocol.h>
namespace falconfix::test {
using DefaultProtocol = fix50::Protocol;
}
#elif defined(FALCONFIX_ENABLE_FIX50SP1)
#include <FIX5.0SP1/engine/protocol.h>
namespace falconfix::test {
using DefaultProtocol = fix50::Protocol;
}
#elif defined(FALCONFIX_ENABLE_FIX50SP2)
#include <FIX5.0SP2/engine/protocol.h>
namespace falconfix::test {
using DefaultProtocol = fix50::Protocol;
}
#else
#error "No supported FIX version enabled for tests"
#endif

namespace falconfix::test {

template<typename Protocol>
struct ProtocolTraits {
    static constexpr auto version = Protocol::VERSION;
    static constexpr auto beginString = Protocol::BEGIN_STRING;

    using SessionType = typename Protocol::SessionType;
};

} // namespace falconfix::test
