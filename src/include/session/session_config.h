// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <unordered_map>

#include <app/fix_message_ref.h>
#include <config/fix_config.h>
#include <session/session_id.h>
#include <session/session_types.h>

namespace falconfix {

struct SessionConfig {
    using Params = std::unordered_map<
        std::string,
        std::string,
        TransparentStringHash,
        TransparentStringEqual
    >;

    struct Dictionary {
        std::string dataDictionary;
        std::string transportDataDictionary;
        std::string appDataDictionary;
        std::string defaultApplVerID;

        static Dictionary fromParams(const Params &parameters);
    };

    struct Connection {
        struct Endpoint {
            std::string host;
            int32_t port{0};

            bool valid() const noexcept {
                return !host.empty() && port > 0;
            }
        };

        ConnectionType connectionType{ConnectionType::Initiator};

        std::string socketConnectHost;
        int32_t socketConnectPort{0};
        std::vector<Endpoint> socketConnectEndpoints;

        std::string socketAcceptHost;
        int32_t socketAcceptPort{0};

        uint64_t heartBtIntMs{30000};
        uint64_t reconnectIntervalMs{30000};
        uint64_t logoutTimeoutMs{5000};

        bool disableHeartbeat{false};
        bool ignoreTestRequest{false};
        bool enableResendRequest{true};
        bool sendRedundantResendRequests{false};

        bool resetOnLogon{false};
        bool resetOnLogout{false};
        bool resetOnDisconnect{false};
        bool tcpNoDelay{true};
        int32_t socketSendBufferSize{0};
        int32_t socketRecvBufferSize{0};
        int32_t socketConnectTimeoutMs{0};
        std::size_t socketMaxWriteChunkSize{64 * 1024};

        bool useSSL{false};
        std::string sslCertFile;
        std::string sslKeyFile;
        std::string sslCaFile;
        std::string sslServerName;
        bool sslVerifyPeer{true};

        bool validateSendingTime{false};
        uint64_t maxLatencyMs{120000};
        bool millisecondsInTimeStamp{true};

        static Connection fromParams(const Params &parameters);

        bool valid() const noexcept;
    };

    struct Storage {
        StoreType storeType{StoreType::None};
        std::string path;
        bool persistMessages{true};

        static Storage fromParams(const Params &parameters);
    };

    struct Logging {
        LogType logType{LogType::None};
        LogLevel level{LogLevel::Info};
        LogLevel flushLevel{LogLevel::Info};

        struct Screen {
            std::string pattern{"[%Y-%m-%d %H:%M:%S.%e] [%l] %v"};
            bool humanReadableFIX{true}; // if true, log FIX messages with SOH replaced by '|'
        };

        struct File {
            std::string pattern{"[%Y-%m-%d %H:%M:%S.%e] %v"};
            std::string path;
            int32_t rotationMaxDays{0};
            int64_t rotationMaxSizeBytes{0};
            int32_t keepFiles{0};
        };

        Screen screen;
        File file;
        static Logging fromParams(const Params &parameters);
    };

    struct Protocol {
        ProtocolVersion version{ProtocolVersion::Unknown};

        static Protocol fromParams(const Params &parameters);

        bool valid() const noexcept {
            return version != ProtocolVersion::Unknown;
        }
    };

    struct SessionSchedule {
        bool enabled{false};
        bool nonStopSession{false};
        bool useWeekdays{false};
        bool useWeeklyWindow{false};
        bool useLocalTime{false};
        std::string timeZone{"UTC"};
        uint8_t weekdaysMask{0};
        uint8_t startDay{0};
        uint8_t endDay{0};

        int64_t startSeconds{0};
        int64_t endSeconds{86400};

        bool resetSeqNumsDaily{false};

        static SessionSchedule fromParams(const Params &parameters);
    };

    SessionID sessionID;
    Dictionary dictionary;
    Connection connection;
    Storage storage;
    Logging logging;
    Protocol protocol;
    SessionSchedule schedule{};

    Params parameters;

    bool valid() const noexcept { return !sessionID.empty() && protocol.valid() && connection.valid(); }
};

} // namespace falconfix
