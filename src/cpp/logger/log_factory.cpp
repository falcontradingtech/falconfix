// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#include <logger/log_factory.h>
#include <logger/spdlog_engine_log.h>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <memory>
#include <string>
#include <filesystem>

namespace falconfix {

namespace {

class SpdlogMessageLog final : public MessageLog {
    std::shared_ptr<spdlog::logger> m_messageLogger;
    std::shared_ptr<spdlog::logger> m_eventLogger;
    bool m_humanReadableFIX{false};

public:
    SpdlogMessageLog(std::shared_ptr<spdlog::logger> messageLogger,
                     std::shared_ptr<spdlog::logger> eventLogger,
                     bool humanReadableFIX)
        : m_messageLogger(std::move(messageLogger)),
          m_eventLogger(std::move(eventLogger)),
          m_humanReadableFIX(humanReadableFIX) {}

    void incoming(std::string_view session, std::string_view msg) noexcept override {
        if (!m_humanReadableFIX) {
            m_messageLogger->info("[{}] [IN] {}", session, msg);
            return;
        }

        thread_local std::string tmp;
        tmp.assign(msg.data(), msg.size());
        std::replace(tmp.begin(), tmp.end(), '\x01', '|');
        m_messageLogger->info("[{}] [IN] {}", session, tmp);
    }

    void outgoing(std::string_view session, std::string_view msg) noexcept override {
        if (!m_humanReadableFIX) {
            m_messageLogger->info("[{}] [OUT] {}", session, msg);
            return;
        }

        thread_local std::string tmp;
        tmp.assign(msg.data(), msg.size());
        std::replace(tmp.begin(), tmp.end(), '\x01', '|');
        m_messageLogger->info("[{}] [OUT] {}", session, tmp);
    }

    void event(std::string_view session, std::string_view msg) noexcept override {
        m_eventLogger->info("[{}] [EVT] {}", session, msg);
    }
};

static std::string sanitize(std::string_view v) {
    std::string out;
    out.reserve(v.size());

    for (char c : v) {
        if ((c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= '0' && c <= '9') ||
            c == '-' || c == '_') {
            out.push_back(c);
        }
        else {
            out.push_back('-');
        }
    }

    return out;
}

static std::string sessionStem(const SessionID &sid) {
    return sanitize(sid.beginString()) + "-"
         + sanitize(sid.senderCompID()) + "-"
         + sanitize(sid.targetCompID());
}

static spdlog::level::level_enum toSpdlogLevel(LogLevel level) noexcept {
    switch (level) {
    case LogLevel::Trace: return spdlog::level::trace;
    case LogLevel::Debug: return spdlog::level::debug;
    case LogLevel::Info: return spdlog::level::info;
    case LogLevel::Warn: return spdlog::level::warn;
    case LogLevel::Error: return spdlog::level::err;
    case LogLevel::Critical: return spdlog::level::critical;
    case LogLevel::Off: return spdlog::level::off;
    default: return spdlog::level::info;
    }
}

static void configure(spdlog::logger &logger,
                      const std::string &pattern,
                      LogLevel level,
                      LogLevel flushLevel) {
    logger.set_pattern(pattern);
    logger.set_level(toSpdlogLevel(level));
    logger.flush_on(toSpdlogLevel(flushLevel));
}

static std::shared_ptr<spdlog::logger>
getOrCreateLogger(const std::string &name,
                  spdlog::sink_ptr sink) {
    if (auto existing = spdlog::get(name)) {
        return existing;
    }

    auto logger = std::make_shared<spdlog::logger>(name, std::move(sink));
    spdlog::register_logger(logger);
    return logger;
}

} // namespace

// SpdlogEngineLog implementations
SpdlogEngineLog::SpdlogEngineLog(std::shared_ptr<spdlog::logger> logger)
    : m_logger(std::move(logger)) {}

void SpdlogEngineLog::info(std::string_view msg) noexcept { m_logger->info("{}", msg); }
void SpdlogEngineLog::warn(std::string_view msg) noexcept { m_logger->warn("{}", msg); }
void SpdlogEngineLog::error(std::string_view msg) noexcept { m_logger->error("{}", msg); }

std::unique_ptr<EngineLog>
LogFactory::createEngineLog(const SessionSettings &) {
    return std::make_unique<NullEngineLog>();
}

std::unique_ptr<MessageLog>
LogFactory::createSessionLog(const SessionConfig &config) {
    if (config.logging.logType == LogType::None) {
        return std::make_unique<NullMessageLog>();
    }

    const std::string stem = sessionStem(config.sessionID);
    const bool fileLog = (config.logging.logType == LogType::File);

    std::shared_ptr<spdlog::logger> msgLogger;
    std::shared_ptr<spdlog::logger> evtLogger;

    if (fileLog) {
        const std::string dir = config.logging.file.path.empty()
            ? "logs"
            : config.logging.file.path;

        std::filesystem::create_directories(dir);

        const auto msgPath = std::filesystem::path(dir) / (stem + ".messages.log");
        const auto evtPath = std::filesystem::path(dir) / (stem + ".event.log");

        msgLogger = getOrCreateLogger(
            "falconfix-msg-" + stem,
            std::make_shared<spdlog::sinks::basic_file_sink_mt>(msgPath.string(), true)
        );

        evtLogger = getOrCreateLogger(
            "falconfix-evt-" + stem,
            std::make_shared<spdlog::sinks::basic_file_sink_mt>(evtPath.string(), true)
        );
    }
    else {
        auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        msgLogger = getOrCreateLogger("falconfix-msg-" + stem, sink);
        evtLogger = getOrCreateLogger("falconfix-evt-" + stem, sink);
    }

    const std::string &pattern = fileLog
        ? config.logging.file.pattern
        : config.logging.screen.pattern;

    configure(
        *msgLogger,
        pattern,
        config.logging.level,
        config.logging.flushLevel
    );

    configure(
        *evtLogger,
        pattern,
        config.logging.level,
        config.logging.flushLevel
    );

    const bool humanReadableFIX = fileLog
        ? false
        : config.logging.screen.humanReadableFIX;

    return std::make_unique<SpdlogMessageLog>(
        std::move(msgLogger),
        std::move(evtLogger),
        humanReadableFIX
    );
}

} // namespace falconfix
