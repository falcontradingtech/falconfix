// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <session/session_id.h>

#if defined(FALCONFIX_ENABLE_FIX44)
#include <FIX4.4/engine/outbound_prepare.h>
#include <FIX4.4/engine/protocol.h>
#include <FIX4.4/messages/MarketDataIncrementalRefresh.h>
namespace falconfix::bench {
using DefaultProtocol = fix44::Protocol;
struct DefaultProtocolTraits {
    static constexpr auto beginString = fix44::Protocol::BEGIN_STRING;
    using IncrementalRefresh = fix44::messages::MarketDataIncrementalRefresh;
    using String32 = fix44::types::STRING<32>;

    static FFStatus prepareOutbound(IncrementalRefresh &message, int64_t seqNum, const falconfix::SessionID &sid) noexcept {
        return fix44::engine::prepareOutbound(message, seqNum, sid);
    }
};
} // namespace falconfix::bench
#elif defined(FALCONFIX_ENABLE_FIX43)
#include <FIX4.3/engine/outbound_prepare.h>
#include <FIX4.3/engine/protocol.h>
#include <FIX4.3/messages/MarketDataIncrementalRefresh.h>
namespace falconfix::bench {
using DefaultProtocol = fix43::Protocol;
struct DefaultProtocolTraits {
    static constexpr auto beginString = fix43::Protocol::BEGIN_STRING;
    using IncrementalRefresh = fix43::messages::MarketDataIncrementalRefresh;
    using String32 = fix43::types::STRING<32>;

    static FFStatus prepareOutbound(IncrementalRefresh &message, int64_t seqNum, const falconfix::SessionID &sid) noexcept {
        return fix43::engine::prepareOutbound(message, seqNum, sid);
    }
};
} // namespace falconfix::bench
#elif defined(FALCONFIX_ENABLE_FIX42)
#include <FIX4.2/engine/outbound_prepare.h>
#include <FIX4.2/engine/protocol.h>
#include <FIX4.2/messages/MarketDataIncrementalRefresh.h>
namespace falconfix::bench {
using DefaultProtocol = fix42::Protocol;
struct DefaultProtocolTraits {
    static constexpr auto beginString = fix42::Protocol::BEGIN_STRING;
    using IncrementalRefresh = fix42::messages::MarketDataIncrementalRefresh;
    using String32 = fix42::types::STRING<32>;

    static FFStatus prepareOutbound(IncrementalRefresh &message, int64_t seqNum, const falconfix::SessionID &sid) noexcept {
        return fix42::engine::prepareOutbound(message, seqNum, sid);
    }
};
} // namespace falconfix::bench
#elif defined(FALCONFIX_ENABLE_FIX41)
#include <FIX4.1/engine/outbound_prepare.h>
#include <FIX4.1/engine/protocol.h>
#include <FIX4.1/messages/MarketDataIncrementalRefresh.h>
namespace falconfix::bench {
using DefaultProtocol = fix41::Protocol;
struct DefaultProtocolTraits {
    static constexpr auto beginString = fix41::Protocol::BEGIN_STRING;
    using IncrementalRefresh = fix41::messages::MarketDataIncrementalRefresh;
    using String32 = fix41::types::STRING<32>;

    static FFStatus prepareOutbound(IncrementalRefresh &message, int64_t seqNum, const falconfix::SessionID &sid) noexcept {
        return fix41::engine::prepareOutbound(message, seqNum, sid);
    }
};
} // namespace falconfix::bench
#elif defined(FALCONFIX_ENABLE_FIX40)
#include <FIX4.0/engine/outbound_prepare.h>
#include <FIX4.0/engine/protocol.h>
#include <FIX4.0/messages/MarketDataIncrementalRefresh.h>
namespace falconfix::bench {
using DefaultProtocol = fix40::Protocol;
struct DefaultProtocolTraits {
    static constexpr auto beginString = fix40::Protocol::BEGIN_STRING;
    using IncrementalRefresh = fix40::messages::MarketDataIncrementalRefresh;
    using String32 = fix40::types::STRING<32>;

    static FFStatus prepareOutbound(IncrementalRefresh &message, int64_t seqNum, const falconfix::SessionID &sid) noexcept {
        return fix40::engine::prepareOutbound(message, seqNum, sid);
    }
};
} // namespace falconfix::bench
#elif defined(FALCONFIX_ENABLE_FIX50)
#include <FIX5.0/engine/outbound_prepare.h>
#include <FIX5.0/engine/protocol.h>
#include <FIX5.0/messages/MarketDataIncrementalRefresh.h>
namespace falconfix::bench {
using DefaultProtocol = fix50::Protocol;
struct DefaultProtocolTraits {
    static constexpr auto beginString = fix50::Protocol::BEGIN_STRING;
    using IncrementalRefresh = fix50::messages::MarketDataIncrementalRefresh;
    using String32 = fix50::types::STRING<32>;

    static FFStatus prepareOutbound(IncrementalRefresh &message, int64_t seqNum, const falconfix::SessionID &sid) noexcept {
        return fix50::engine::prepareOutbound(message, seqNum, sid);
    }
};
} // namespace falconfix::bench
#elif defined(FALCONFIX_ENABLE_FIX50SP1)
#include <FIX5.0SP1/engine/outbound_prepare.h>
#include <FIX5.0SP1/engine/protocol.h>
#include <FIX5.0SP1/messages/MarketDataIncrementalRefresh.h>
namespace falconfix::bench {
using DefaultProtocol = fix50::Protocol;
struct DefaultProtocolTraits {
    static constexpr auto beginString = fix50::Protocol::BEGIN_STRING;
    using IncrementalRefresh = fix50::messages::MarketDataIncrementalRefresh;
    using String32 = fix50::types::STRING<32>;

    static FFStatus prepareOutbound(IncrementalRefresh &message, int64_t seqNum, const falconfix::SessionID &sid) noexcept {
        return fix50::engine::prepareOutbound(message, seqNum, sid);
    }
};
} // namespace falconfix::bench
#elif defined(FALCONFIX_ENABLE_FIX50SP2)
#include <FIX5.0SP2/engine/outbound_prepare.h>
#include <FIX5.0SP2/engine/protocol.h>
#include <FIX5.0SP2/messages/MarketDataIncrementalRefresh.h>
namespace falconfix::bench {
using DefaultProtocol = fix50::Protocol;
struct DefaultProtocolTraits {
    static constexpr auto beginString = fix50::Protocol::BEGIN_STRING;
    using IncrementalRefresh = fix50::messages::MarketDataIncrementalRefresh;
    using String32 = fix50::types::STRING<32>;

    static FFStatus prepareOutbound(IncrementalRefresh &message, int64_t seqNum, const falconfix::SessionID &sid) noexcept {
        return fix50::engine::prepareOutbound(message, seqNum, sid);
    }
};
} // namespace falconfix::bench
#else
#error "No supported FIX version enabled for benchmarks"
#endif
