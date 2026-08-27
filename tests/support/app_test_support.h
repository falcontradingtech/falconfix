// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <string>

#if defined(FALCONFIX_ENABLE_FIX44)
#include <FIX4.4/messages/MarketDataIncrementalRefresh.h>
namespace app_test_support
{
    using TestProtocol = fix44::Protocol;
    using TestMessage = fix44::messages::MarketDataIncrementalRefresh;
} // namespace app_test_support
#elif defined(FALCONFIX_ENABLE_FIX43)
#include <FIX4.3/messages/MarketDataIncrementalRefresh.h>
namespace app_test_support
{
    using TestProtocol = fix43::Protocol;
    using TestMessage = fix43::messages::MarketDataIncrementalRefresh;
} // namespace app_test_support
#elif defined(FALCONFIX_ENABLE_FIX42)
#include <FIX4.2/messages/MarketDataIncrementalRefresh.h>
namespace app_test_support
{
    using TestProtocol = fix42::Protocol;
    using TestMessage = fix42::messages::MarketDataIncrementalRefresh;
} // namespace app_test_support
#elif defined(FALCONFIX_ENABLE_FIX41)
#include <FIX4.1/messages/MarketDataIncrementalRefresh.h>
namespace app_test_support
{
    using TestProtocol = fix41::Protocol;
    using TestMessage = fix41::messages::MarketDataIncrementalRefresh;
} // namespace app_test_support
#elif defined(FALCONFIX_ENABLE_FIX40)
#include <FIX4.0/messages/MarketDataIncrementalRefresh.h>
namespace app_test_support
{
    using TestProtocol = fix40::Protocol;
    using TestMessage = fix40::messages::MarketDataIncrementalRefresh;
} // namespace app_test_support
#elif defined(FALCONFIX_ENABLE_FIX50)
#include <FIX5.0/messages/MarketDataIncrementalRefresh.h>
namespace app_test_support
{
    using TestProtocol = fix50::Protocol;
    using TestMessage = fix50::messages::MarketDataIncrementalRefresh;
} // namespace app_test_support
#elif defined(FALCONFIX_ENABLE_FIX50SP1)
#include <FIX5.0SP1/messages/MarketDataIncrementalRefresh.h>
namespace app_test_support
{
    using TestProtocol = fix50::Protocol;
    using TestMessage = fix50::messages::MarketDataIncrementalRefresh;
} // namespace app_test_support
#elif defined(FALCONFIX_ENABLE_FIX50SP2)
#include <FIX5.0SP2/messages/MarketDataIncrementalRefresh.h>
namespace app_test_support
{
    using TestProtocol = fix50::Protocol;
    using TestMessage = fix50::messages::MarketDataIncrementalRefresh;
} // namespace app_test_support
#else
#error "No supported FIX version enabled for app_test"
#endif

namespace app_test_support
{

    inline std::string beginString()
    {
        return std::string(TestProtocol::BEGIN_STRING);
    }

} // namespace app_test_support
