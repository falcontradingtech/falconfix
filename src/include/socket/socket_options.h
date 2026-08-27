// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

namespace falconfix::socket {

    struct SocketOptions {
        bool m_noDelay{true};
        bool m_fastOpen{false};
        bool m_cork{false};

        int32_t m_sendBufferSize{0};   // 0 = OS default
        int32_t m_recvBufferSize{0};   // 0 = OS default

        int32_t m_connectTimeoutMs{0}; // 0 = no timeout
        int32_t m_sendTimeoutMs{0};    // 0 = no timeout
        int32_t m_recvTimeoutMs{0};    // 0 = no timeout

        std::size_t m_maxWriteChunkSize{64 * 1024};    // 64KB default

        std::string m_tlsCertFile{};
        std::string m_tlsKeyFile{};
        std::string m_tlsCaFile{};
        std::string m_tlsServerName{};
        bool m_tlsVerifyPeer{false};
    };

} // namespace falconfix::socket
