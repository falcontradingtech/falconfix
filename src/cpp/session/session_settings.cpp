// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <session/session_settings.h>

#include <algorithm>
#include <array>
#include <stdexcept>

namespace falconfix {

namespace {

inline std::string getOr(const SessionConfig::Params &p,
                         std::string_view key,
                         std::string fallback = {}) {
    auto it = p.find(key);
    if (it == p.end()) return fallback;
    return it->second;
}

inline std::string getOrAlias(const SessionConfig::Params &p,
                              std::string_view canonicalKey,
                              std::string_view aliasKey,
                              std::string fallback = {}) {
    auto it = p.find(canonicalKey);
    if (it != p.end()) {
        return it->second;
    }

    it = p.find(aliasKey);
    if (it != p.end()) {
        return it->second;
    }

    return fallback;
}

inline int32_t getIntOr(const SessionConfig::Params &p,
                        std::string_view key,
                        int32_t fallback) {
    auto it = p.find(key);
    if (it == p.end()) return fallback;

    int32_t out = fallback;
    const char *b = it->second.data();
    const char *e = b + it->second.size();

    const auto rc = std::from_chars(b, e, out);
    if (rc.ec != std::errc{} || rc.ptr != e) return fallback;

    return out;
}

inline int32_t getIntOrAlias(const SessionConfig::Params &p,
                             std::string_view canonicalKey,
                             std::string_view aliasKey,
                             int32_t fallback) {
    auto it = p.find(canonicalKey);
    if (it != p.end()) {
        int32_t out = fallback;
        const char *b = it->second.data();
        const char *e = b + it->second.size();
        const auto rc = std::from_chars(b, e, out);
        return (rc.ec == std::errc{} && rc.ptr == e) ? out : fallback;
    }

    it = p.find(aliasKey);
    if (it != p.end()) {
        int32_t out = fallback;
        const char *b = it->second.data();
        const char *e = b + it->second.size();
        const auto rc = std::from_chars(b, e, out);
        return (rc.ec == std::errc{} && rc.ptr == e) ? out : fallback;
    }

    return fallback;
}

inline bool iequals(std::string_view a, std::string_view b) noexcept {
    if (a.size() != b.size()) return false;

    for (std::size_t i = 0; i < a.size(); ++i) {
        const auto ca = static_cast<char>(std::toupper(static_cast<unsigned char>(a[i])));
        const auto cb = static_cast<char>(std::toupper(static_cast<unsigned char>(b[i])));
        if (ca != cb) return false;
    }

    return true;
}

inline bool getBoolOr(const SessionConfig::Params &p,
                      std::string_view key,
                      bool fallback) {
    auto it = p.find(key);
    if (it == p.end()) return fallback;

    const std::string_view v = it->second;

    if (iequals(v, "Y") || iequals(v, "YES") || iequals(v, "TRUE") || v == "1") return true;
    if (iequals(v, "N") || iequals(v, "NO") || iequals(v, "FALSE") || v == "0") return false;

    return fallback;
}

inline bool getBoolOrAlias(const SessionConfig::Params &p,
                           std::string_view canonicalKey,
                           std::string_view aliasKey,
                           bool fallback) {
    auto it = p.find(canonicalKey);
    if (it == p.end()) {
        it = p.find(aliasKey);
        if (it == p.end()) {
            return fallback;
        }
    }

    const std::string_view v = it->second;

    if (iequals(v, "Y") || iequals(v, "YES") || iequals(v, "TRUE") || v == "1") return true;
    if (iequals(v, "N") || iequals(v, "NO") || iequals(v, "FALSE") || v == "0") return false;

    return fallback;
}

inline ConnectionType parseConnectionType(std::string_view v) noexcept {
    if (iequals(v, "initiator")) return ConnectionType::Initiator;
    if (iequals(v, "acceptor")) return ConnectionType::Acceptor;
    return ConnectionType::Initiator;
}

inline StoreType parseStoreType(std::string_view v) noexcept {
    if (iequals(v, "memory")) return StoreType::Memory;
    if (iequals(v, "file")) return StoreType::File;
    if (iequals(v, "none")) return StoreType::None;
    return StoreType::None;
}

inline LogType parseLogType(std::string_view v) noexcept {
    if (iequals(v, "screen")) return LogType::Screen;
    if (iequals(v, "file")) return LogType::File;
    if (iequals(v, "none")) return LogType::None;
    return LogType::None;
}

inline LogLevel parseLogLevel(std::string_view v) noexcept {
    if (iequals(v, "trace")) return LogLevel::Trace;
    if (iequals(v, "debug")) return LogLevel::Debug;
    if (iequals(v, "info")) return LogLevel::Info;
    if (iequals(v, "warn") || iequals(v, "warning")) return LogLevel::Warn;
    if (iequals(v, "error")) return LogLevel::Error;
    if (iequals(v, "critical")) return LogLevel::Critical;
    if (iequals(v, "off")) return LogLevel::Off;
    return LogLevel::Info;
}

inline int64_t parseTimeHMS(std::string_view s) noexcept {
    if (s.size() != 8 ||
        s[2] != ':' ||
        s[5] != ':') [[unlikely]] {
        return 0;
    }

    const int32_t hh =
        (s[0] - '0') * 10 +
        (s[1] - '0');

    const int32_t mm =
        (s[3] - '0') * 10 +
        (s[4] - '0');

    const int32_t ss =
        (s[6] - '0') * 10 +
        (s[7] - '0');

    if (hh < 0 || hh > 24 ||
        mm < 0 || mm > 59 ||
        ss < 0 || ss > 59) [[unlikely]] {
        return 0;
    }

    return (hh * 3600) + (mm * 60) + ss;
}

int parseWeekdayToken(std::string_view token) {
    std::string lowered;
    lowered.reserve(token.size());
    for (const unsigned char ch : token) {
        if (!std::isspace(ch)) {
            lowered.push_back(static_cast<char>(std::tolower(ch)));
        }
    }

    if (lowered == "su" || lowered == "sun" || lowered == "sunday") return 0;
    if (lowered == "mo" || lowered == "mon" || lowered == "monday") return 1;
    if (lowered == "tu" || lowered == "tue" || lowered == "tues" || lowered == "tuesday") return 2;
    if (lowered == "we" || lowered == "wed" || lowered == "wednesday") return 3;
    if (lowered == "th" || lowered == "thu" || lowered == "thur" || lowered == "thurs" || lowered == "thursday") return 4;
    if (lowered == "fr" || lowered == "fri" || lowered == "friday") return 5;
    if (lowered == "sa" || lowered == "sat" || lowered == "saturday") return 6;

    throw std::runtime_error("invalid Weekdays token: " + std::string(token));
}

std::string trimAscii(std::string_view text) {
    std::size_t begin = 0;
    while (begin < text.size() && std::isspace(static_cast<unsigned char>(text[begin]))) {
        ++begin;
    }

    std::size_t end = text.size();
    while (end > begin && std::isspace(static_cast<unsigned char>(text[end - 1]))) {
        --end;
    }

    return std::string(text.substr(begin, end - begin));
}

uint8_t parseWeekdaysMask(std::string_view weekdays) {
    uint8_t mask = 0;
    std::size_t start = 0;

    while (start < weekdays.size()) {
        const std::size_t comma = weekdays.find(',', start);
        const auto token = weekdays.substr(
            start,
            comma == std::string_view::npos ? std::string_view::npos : comma - start
        );

        const int weekday = parseWeekdayToken(token);
        mask = static_cast<uint8_t>(mask | (1u << weekday));

        if (comma == std::string_view::npos) {
            break;
        }
        start = comma + 1;
    }

    return mask;
}

std::vector<SessionConfig::Connection::Endpoint>
parseSocketConnectEndpoints(const SessionConfig::Params &parameters) {
    using Endpoint = SessionConfig::Connection::Endpoint;

    std::vector<int32_t> alternateIndices;
    for (const auto &[key, _] : parameters) {
        constexpr std::array<std::string_view, 2> prefixes{
            "SocketConnectHost",
            "SocketConnectPort"
        };

        for (const auto prefix : prefixes) {
            if (key.size() <= prefix.size() || key.compare(0, prefix.size(), prefix) != 0) {
                continue;
            }

            const std::string_view suffix(key.data() + prefix.size(), key.size() - prefix.size());
            int32_t index = 0;
            const auto *begin = suffix.data();
            const auto *end = begin + suffix.size();
            const auto rc = std::from_chars(begin, end, index);
            if (rc.ec == std::errc{} && rc.ptr == end && index > 0) {
                alternateIndices.push_back(index);
            }
        }
    }

    std::sort(alternateIndices.begin(), alternateIndices.end());
    alternateIndices.erase(std::unique(alternateIndices.begin(), alternateIndices.end()), alternateIndices.end());

    for (std::size_t i = 0; i < alternateIndices.size(); ++i) {
        const int32_t expected = static_cast<int32_t>(i + 1);
        if (alternateIndices[i] != expected) {
            throw std::runtime_error("SocketConnectHost<n>/SocketConnectPort<n> must be consecutive starting at 1");
        }
    }

    std::vector<Endpoint> endpoints;

    const auto primaryHost = getOr(parameters, "SocketConnectHost");
    const auto primaryPort = getIntOr(parameters, "SocketConnectPort", 0);
    if (!primaryHost.empty() || primaryPort > 0) {
        endpoints.push_back({primaryHost, primaryPort});
    }

    for (const int32_t index : alternateIndices) {
        const auto hostKey = "SocketConnectHost" + std::to_string(index);
        const auto portKey = "SocketConnectPort" + std::to_string(index);

        const auto host = getOr(parameters, hostKey);
        const auto port = getIntOr(parameters, portKey, 0);

        if (host.empty() || port <= 0) {
            throw std::runtime_error("SocketConnectHost<n> and SocketConnectPort<n> must be provided together");
        }

        endpoints.push_back({host, port});
    }

    return endpoints;
}

} // namespace




SessionConfig::Dictionary
SessionConfig::Dictionary::fromParams(const Params &parameters) {
    Dictionary d;
    d.dataDictionary = getOr(parameters, "DataDictionary");
    d.transportDataDictionary = getOr(parameters, "TransportDataDictionary");
    d.appDataDictionary = getOr(parameters, "AppDataDictionary");
    d.defaultApplVerID = getOr(parameters, "DefaultApplVerID");
    return d;
}

bool SessionConfig::Connection::valid() const noexcept {
    if (connectionType == ConnectionType::Initiator) {
        if (!socketConnectEndpoints.empty()) {
            for (const auto &endpoint : socketConnectEndpoints) {
                if (!endpoint.valid()) {
                    return false;
                }
            }
            return true;
        }

        return !socketConnectHost.empty() && socketConnectPort > 0;
    }

    return socketAcceptPort > 0;
}

SessionConfig::Connection
SessionConfig::Connection::fromParams(const Params &parameters) {
    Connection c;

    c.connectionType = parseConnectionType(getOr(parameters, "ConnectionType", "initiator"));

    c.socketConnectHost = getOr(parameters, "SocketConnectHost");
    c.socketConnectPort = getIntOr(parameters, "SocketConnectPort", 0);
    c.socketConnectEndpoints = parseSocketConnectEndpoints(parameters);
    if (!c.socketConnectEndpoints.empty()) {
        c.socketConnectHost = c.socketConnectEndpoints.front().host;
        c.socketConnectPort = c.socketConnectEndpoints.front().port;
    }

    c.socketAcceptHost = getOr(parameters, "SocketAcceptHost");
    c.socketAcceptPort = getIntOr(parameters, "SocketAcceptPort", 0);

    c.heartBtIntMs = static_cast<uint64_t>(getIntOr(parameters, "HeartBtInt", 30)) * 1000ULL;
    const bool hasReconnectSeconds = parameters.find("ReconnectInterval") != parameters.end();
    const bool hasReconnectMs = parameters.find("ReconnectIntervalMs") != parameters.end();
    c.reconnectIntervalMs = hasReconnectSeconds
        ? static_cast<uint64_t>(getIntOr(parameters, "ReconnectInterval", 30)) * 1000ULL
        : static_cast<uint64_t>(getIntOrAlias(parameters, "ReconnectInterval", "ReconnectIntervalMs", 30000));

    const bool hasLogoutSeconds = parameters.find("LogoutTimeout") != parameters.end();
    const bool hasLogoutMs = parameters.find("LogoutTimeoutMs") != parameters.end();
    c.logoutTimeoutMs = hasLogoutSeconds
        ? static_cast<uint64_t>(getIntOr(parameters, "LogoutTimeout", 5)) * 1000ULL
        : static_cast<uint64_t>(getIntOrAlias(parameters, "LogoutTimeout", "LogoutTimeoutMs", 5000));

    c.disableHeartbeat = getBoolOr(parameters, "DisableHeartbeat", false);
    c.ignoreTestRequest = getBoolOr(parameters, "IgnoreTestRequest", false);
    c.enableResendRequest = getBoolOr(parameters, "EnableResendRequest", true);
    c.sendRedundantResendRequests = getBoolOr(parameters, "SendRedundantResendRequests", false);

    c.resetOnLogon = getBoolOr(parameters, "ResetOnLogon", false);
    c.resetOnLogout = getBoolOr(parameters, "ResetOnLogout", false);
    c.resetOnDisconnect = getBoolOr(parameters, "ResetOnDisconnect", false);
    c.tcpNoDelay = getBoolOr(parameters, "SocketTcpNoDelay", true);
    c.socketSendBufferSize = getIntOr(parameters, "SocketSendBufferSize", 0);
    c.socketRecvBufferSize = getIntOr(parameters, "SocketRecvBufferSize", 0);
    c.socketConnectTimeoutMs = getIntOr(parameters, "SocketConnectTimeoutMs", 0);
    c.socketMaxWriteChunkSize = static_cast<std::size_t>(getIntOr(parameters, "SocketMaxWriteChunkSize", 64 * 1024));

    (void)hasReconnectMs;
    (void)hasLogoutMs;

    c.useSSL = getBoolOrAlias(parameters, "UseSSL", "SocketUseSSL", false);
    c.sslCertFile = getOr(parameters, "SSLCertFile");
    c.sslKeyFile = getOr(parameters, "SSLKeyFile");
    c.sslCaFile = getOrAlias(parameters, "SSLCAFile", "SSLCaFile");
    c.sslServerName = getOr(parameters, "SSLServerName");
    c.sslVerifyPeer = getBoolOr(parameters, "SSLVerifyPeer", true);

    c.validateSendingTime = getBoolOr(parameters, "ValidateSendingTime", false);
    c.maxLatencyMs = getIntOr(parameters, "MaxLatencyMs", 120000);
    c.millisecondsInTimeStamp = getBoolOr(parameters, "MillisecondsInTimeStamp", true);

    return c;
}

SessionConfig::Storage
SessionConfig::Storage::fromParams(const Params &parameters) {
    Storage s;
    s.storeType = parseStoreType(getOr(parameters, "StoreType", "none"));
    s.path = getOr(parameters, "FileStorePath");
    s.persistMessages = getBoolOr(parameters, "PersistMessages", true);

    return s;
}

SessionConfig::Logging
SessionConfig::Logging::fromParams(const Params &parameters) {
    Logging l;

    l.logType = parseLogType(getOr(parameters, "LogType", "none"));

    l.level = parseLogLevel(getOr(parameters, "LogLevel", "info"));
    l.flushLevel = parseLogLevel(getOr(parameters, "LogFlushLevel", "info"));

    l.screen.pattern = getOr(
        parameters,
        "ScreenLogPattern",
        "[%Y-%m-%d %H:%M:%S.%e] [%l] %v"
    );

    l.screen.humanReadableFIX = getBoolOr(
        parameters,
        "ScreenLogHumanReadableFIX",
        true
    );

    l.file.pattern = getOr(
        parameters,
        "FileLogPattern",
        "[%Y-%m-%d %H:%M:%S.%e] %v"
    );

    l.file.path = getOr(parameters, "FileLogPath");

    l.file.rotationMaxDays = getIntOr(parameters, "FileLogRotationMaxDays", 0);
    l.file.rotationMaxSizeBytes = getIntOr(parameters, "FileLogRotationMaxSizeBytes", 0);
    l.file.keepFiles = getIntOr(parameters, "FileLogKeepFiles", 0);

    return l;
}

SessionConfig::Protocol
SessionConfig::Protocol::fromParams(const Params &parameters) {
    Protocol p;
    p.version = SessionSettings::parseProtocolVersion(getOr(parameters, "BeginString"));
    return p;
}

SessionConfig::SessionSchedule
SessionConfig::SessionSchedule::fromParams(const Params &parameters) {
    SessionSchedule s;

    s.nonStopSession = getBoolOr(parameters, "NonStopSession", false);
    s.useLocalTime = getBoolOr(parameters, "UseLocalTime", false);
    s.timeZone = trimAscii(getOr(parameters, "TimeZone"));
    falconfix::session_time::validateTimeZone(s.timeZone, s.useLocalTime);

    const bool hasStartDay = parameters.find("StartDay") != parameters.end();
    const bool hasEndDay = parameters.find("EndDay") != parameters.end();
    if (hasStartDay != hasEndDay) {
        throw std::runtime_error("StartDay and EndDay must be provided together");
    }

    if (const auto weekdays = getOr(parameters, "Weekdays"); !weekdays.empty()) {
        if (hasStartDay || hasEndDay) {
            throw std::runtime_error("Weekdays is not compatible with StartDay/EndDay");
        }
        s.useWeekdays = true;
        s.weekdaysMask = parseWeekdaysMask(weekdays);
    }

    if (hasStartDay && hasEndDay) {
        s.useWeeklyWindow = true;
        s.startDay = static_cast<uint8_t>(parseWeekdayToken(getOr(parameters, "StartDay")));
        s.endDay = static_cast<uint8_t>(parseWeekdayToken(getOr(parameters, "EndDay")));
    }

    s.startSeconds = parseTimeHMS(getOr(parameters, "StartTime", "00:00:00"));
    s.endSeconds = parseTimeHMS(getOr(parameters, "EndTime", "24:00:00"));
    s.resetSeqNumsDaily = getBoolOr(parameters, "ResetSeqNumsDaily", false);

    s.enabled = !s.nonStopSession && (s.startSeconds != 0 || s.endSeconds != 86400);
    return s;
}

SessionID SessionSettings::buildSessionID(const SessionConfig::Params &parameters) {
    return SessionID(
        getOr(parameters, "BeginString"),
        getOr(parameters, "SenderCompID"),
        getOr(parameters, "TargetCompID")
    );
}

ProtocolVersion SessionSettings::parseProtocolVersion(std::string_view beginString) noexcept {
    if (beginString == "FIX4.0") return ProtocolVersion::FIX40;
    if (beginString == "FIX4.1") return ProtocolVersion::FIX41;
    if (beginString == "FIX4.2") return ProtocolVersion::FIX42;
    if (beginString == "FIX4.3") return ProtocolVersion::FIX43;
    if (beginString == "FIX4.4") return ProtocolVersion::FIX44;
    if (beginString == "FIX5.0") return ProtocolVersion::FIX50;
    if (beginString == "FIX5.0SP1") return ProtocolVersion::FIX50SP1;
    if (beginString == "FIX5.0SP2") return ProtocolVersion::FIX50SP2;
    if (beginString == "FIXT1.1") return ProtocolVersion::FIXT11;
    return ProtocolVersion::Unknown;
}

std::string_view SessionSettings::protocolVersionName(ProtocolVersion version) noexcept {
    switch (version) {
    case ProtocolVersion::FIX40: return "FIX4.0";
    case ProtocolVersion::FIX41: return "FIX4.1";
    case ProtocolVersion::FIX42: return "FIX4.2";
    case ProtocolVersion::FIX43: return "FIX4.3";
    case ProtocolVersion::FIX44: return "FIX4.4";
    case ProtocolVersion::FIX50: return "FIX5.0";
    case ProtocolVersion::FIX50SP1: return "FIX5.0SP1";
    case ProtocolVersion::FIX50SP2: return "FIX5.0SP2";
    case ProtocolVersion::FIXT11: return "FIXT1.1";
    default: return "UNKNOWN";
    }
}

SessionSettings SessionSettings::fromConfig(const FIXConfig &cfg) {
    SessionSettings settings;

    settings.m_sessions.reserve(cfg.sectionCount());

    for (std::size_t i = 0; i < cfg.sectionCount(); ++i) {
        SessionConfig sc;

        for (const auto &[k, v] : cfg.defaults()) {
            sc.parameters.emplace(k, v);
        }

        for (const auto &[k, v] : cfg.section(i)) {
            sc.parameters[k] = v;
        }

        sc.sessionID = buildSessionID(sc.parameters);
        sc.dictionary = SessionConfig::Dictionary::fromParams(sc.parameters);
        sc.connection = SessionConfig::Connection::fromParams(sc.parameters);
        sc.storage = SessionConfig::Storage::fromParams(sc.parameters);
        sc.logging = SessionConfig::Logging::fromParams(sc.parameters);
        sc.protocol = SessionConfig::Protocol::fromParams(sc.parameters);
        sc.schedule = SessionConfig::SessionSchedule::fromParams(sc.parameters);

        settings.m_sessions.emplace_back(std::move(sc));
    }

    return settings;
}

SessionSettings SessionSettings::fromFile(std::string_view filePath) {
    return fromConfig(FIXConfig::parseFile(filePath));
}

} // namespace falconfix
