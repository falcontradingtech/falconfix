// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <array>
#include <charconv>
#include <cstdint>
#include <cstring>
#include <string>
#include <string_view>
#include <ostream>
#include <algorithm>
#include <assert.h>

namespace falconfix {

template <std::size_t N>
struct FixedString {
    static_assert(N > 0 && N <= 65536, "FixedString size must be 1-65536");

    std::array<char, N> m_data{};
    uint16_t            m_size{0};

    // =============================================================
    // CTORs (constexpr + noexcept)
    // =============================================================
    constexpr FixedString() noexcept = default;

    // literal (compile-time, zero runtime cost)
    constexpr FixedString(const char *s, std::size_t size) noexcept
        : m_size(static_cast<uint16_t>(size)) {
        if (m_size >= N) m_size = N - 1;
        std::memcpy(m_data.data(), s, m_size);
        m_data[m_size] = '\0';
    }

    // Deduction for literal strings
    template <std::size_t M>
    constexpr FixedString(const char (&s)[M]) noexcept
        : FixedString(s, M-1) {
        static_assert(M-1 <= N, "Literal too long for this FixedString");
    }

    // Mutable runtime buffers should use strlen-based sizing, not M-1,
    // otherwise work buffers like char tmp[32] copy trailing garbage.
    template <std::size_t M>
    constexpr FixedString(char (&s)[M]) noexcept
        : FixedString(static_cast<const char *>(s)) {}

    constexpr FixedString(std::string_view sv) noexcept
        : FixedString(sv.data(), sv.size()) {}

    constexpr FixedString(const std::string &s) noexcept
        : FixedString(s.data(), s.size()) {}

    constexpr FixedString(const char *s) noexcept;

    constexpr void assign(FixedString<N> other) noexcept {
        m_size = other.size();
        std::memcpy(m_data.data(), other.data(), m_size);
        m_data[m_size] = '\0';
    }
    constexpr void assign(const char *s) noexcept {
        if (!s) return;
        std::size_t len = std::strlen(s);
        len = std::min(len, N-1);
        m_size = static_cast<uint16_t>(len);
        std::memcpy(m_data.data(), s, len);
        m_data[m_size] = '\0';
    }
    constexpr void assign(const char *s, std::size_t count) noexcept {
        count = std::min(count, N-1);
        m_size = static_cast<uint16_t>(count);
        std::memcpy(m_data.data(), s, m_size);
        m_data[m_size] = '\0';
    }
    constexpr void assign(std::string_view sv) noexcept { assign(sv.data(), sv.size()); }
    constexpr void assign(const std::string &s) noexcept { assign(s.data(), s.size()); }

    constexpr void clear() noexcept { m_data[0] = '\0'; m_size = 0; }
    [[nodiscard]] constexpr bool empty() const noexcept { return m_size == 0; }
    constexpr std::size_t size() const noexcept  { return m_size; }
    constexpr std::size_t length() const noexcept { return m_size; }
    constexpr std::size_t capacity() const noexcept { return N-1; }
    constexpr std::size_t max_size() const noexcept { return N-1; }

    constexpr const char *c_str() const noexcept { return m_data.data(); }
    constexpr const char *data() const noexcept { return m_data.data(); }
    constexpr char *data() noexcept { return m_data.data(); }

    // std::string_view
    constexpr std::string_view view() const noexcept { return std::string_view(m_data.data(), m_size); }
    constexpr operator std::string_view() const noexcept { return std::string_view(m_data.data(), m_size); }

    // =============================================================
    // Operators
    // =============================================================
    constexpr FixedString &operator=(const char *s) noexcept { assign(s); return *this; }
    constexpr FixedString &operator=(std::string_view sv) noexcept { assign(sv); return *this; }
    constexpr char &operator[](std::size_t i) noexcept { assert(i < m_size); return m_data[i]; }
    constexpr const char &operator[](std::size_t i) const noexcept { assert(i < m_size); return m_data[i]; }
    constexpr bool operator==(const FixedString &o) const noexcept { if (m_size != o.m_size) return false; return std::memcmp(m_data.data(), o.m_data.data(), m_size) == 0; }
    constexpr bool operator!=(const FixedString &o) const noexcept { return !(*this == o); }
    constexpr bool operator<(const FixedString &o) const noexcept {  return std::lexicographical_compare(m_data.data(), m_data.data()+m_size, o.m_data.data(), o.m_data.data()+o.m_size); }
    constexpr bool operator==(std::string_view o) const noexcept { return view() == o; }
    constexpr bool operator!=(std::string_view o) const noexcept { return !(*this == o); }
    constexpr bool operator==(const char *s) const noexcept { if (!s) return empty(); return view() == std::string_view(s); }
    constexpr bool operator!=(const char *s) const noexcept { return !(*this == s); }

    // =============================================================
    // STL compatibility (begin/end, iterators)
    // =============================================================
    constexpr const char *begin() const noexcept { return m_data.data(); }
    constexpr const char *end() const noexcept   { return m_data.data() + m_size; }
    constexpr char *begin() noexcept             { return m_data.data(); }
    constexpr char *end() noexcept               { return m_data.data() + m_size; }

    // =============================================================
    // APPEND METHODS (like std::string)
    // =============================================================
    constexpr FixedString &append(int64_t value) noexcept {
        if (m_size >= N - 1) return *this;  // mo space

        char* start = m_data.data() + m_size;
        char* end   = m_data.data() + N - 1;  // -1 para '\0'

        auto [ptr, ec] = std::to_chars(start, end, value);

        if (ec == std::errc{}) {
            m_size = static_cast<uint16_t>(ptr - m_data.data());
            m_data[m_size] = '\0';
        }

        return *this;
    }

    constexpr FixedString &append(uint64_t value) noexcept {
        if (m_size >= N - 1) return *this;

        char* start = m_data.data() + m_size;
        char* end   = m_data.data() + N - 1;

        auto [ptr, ec] = std::to_chars(start, end, value);

        if (ec == std::errc{}) {
            m_size = static_cast<uint16_t>(ptr - m_data.data());
            m_data[m_size] = '\0';
        }

        return *this;
    }

    constexpr FixedString &append(double value, int32_t precision = 6) noexcept {
        if (m_size >= N - 1) return *this;

        char* start = m_data.data() + m_size;
        char* end   = m_data.data() + N - 1;

        std::chars_format fmt = (precision <= 0) ? std::chars_format::general : std::chars_format::fixed;
        auto [ptr, ec] = std::to_chars(start, end, value, fmt, precision);

        if (ec == std::errc{}) {
            m_size = static_cast<uint16_t>(ptr - m_data.data());
            m_data[m_size] = '\0';
        }

        return *this;
    }

    constexpr FixedString &append(char c) noexcept {
        if (m_size < N - 1) {
            m_data[m_size++] = c;
            m_data[m_size] = '\0';
        }
        return *this;
    }

    constexpr FixedString &append(const char *s) noexcept {
        if (!s) return *this;
        std::size_t len = std::strlen(s);
        return append(s, len);
    }

    constexpr FixedString &append(const char *s, std::size_t count) noexcept {
        std::size_t avail = N - 1 - m_size;
        std::size_t copy = std::min(count, avail);
        std::memcpy(m_data.data() + m_size, s, copy);
        m_size += static_cast<uint16_t>(copy);
        m_data[m_size] = '\0';
        return *this;
    }

    constexpr FixedString &append(std::string_view sv) noexcept {
        return append(sv.data(), sv.size());
    }

    constexpr FixedString &append(std::size_t n, char c) noexcept {
        std::size_t avail = N - 1 - m_size;
        std::size_t copy = std::min(n, avail);
        std::fill_n(m_data.data() + m_size, copy, c);
        m_size += static_cast<uint16_t>(copy);
        m_data[m_size] = '\0';
        return *this;
    }

    template <std::size_t M>
    constexpr FixedString &append(const FixedString<M> &other) noexcept {
        return append(other.data(), other.size());
    }

    // =============================================================
    // REPLACE METHODS (in-place, like std::string::replace)
    // =============================================================
    // replace(pos, len, "newtext") â€” same size // â†’ becomes just memcpy (zero overhead)
    constexpr FixedString &replace(std::size_t pos, std::size_t len, const char *s) noexcept {
        assert(pos + len <= capacity());
        std::memcpy(m_data.data() + pos, s, len);
        return *this;
    }

    // literal â€” compiler infers size â†’ 100% safe and fast
    template <std::size_t M>
    constexpr FixedString &replace(std::size_t pos, std::size_t len, const char (&lit)[M]) noexcept {
        constexpr std::size_t literal_len = M - 1;
        assert(len <= literal_len);
        assert(pos + len <= capacity());
        std::memcpy(m_data.data() + pos, lit, len);
        return *this;
    }

    // string_view â€” you already know the size
    constexpr FixedString &replace(std::size_t pos, std::size_t len, std::string_view sv) noexcept {
        assert(sv.size() == len);                 // optional, but helps catch misuse
        assert(pos + len <= capacity());          // <-- REQUIRED
        std::memcpy(m_data.data() + pos, sv.data(), len);
        return *this;
    }

    // generic version with count â€” maintains full compatibility
    constexpr FixedString &replace(std::size_t pos, std::size_t len, const char *s, std::size_t count) noexcept {
        if (count != len) {
            // Rare case: different size â†’ falls into the slow version (or UB if you want)
            // Here you can put assert or safe fallback
            // But in real HFT: this never happens â†’ you can even leave it as UB
            std::memcpy(m_data.data() + pos, s, count > len ? len : count);
        }
        else {
            std::memcpy(m_data.data() + pos, s, len);
        }
        return *this;
    }

    // =============================================================
    // Hash (only for unordered_map use)
    // =============================================================
    struct Hash {
        constexpr std::size_t operator()(const FixedString& fs) const noexcept {
            // Fast FNV-1a hash
            std::size_t h = 14695981039346656037ull;
            for (uint16_t i = 0; i < fs.m_size; ++i) {
                h ^= static_cast<std::size_t>(static_cast<unsigned char>(fs.m_data[i]));
                h *= 1099511628211ull;
            }
            return h;
        }
    };

    // =============================================================
    // Print
    // =============================================================
#ifdef FALCONFIX_ENABLE_TEST_API
    inline friend std::ostream &operator<<(std::ostream &os, const FixedString &fs) noexcept { return os.write(fs.data(), fs.size()); }
#else
    friend std::ostream &operator<<(std::ostream &os, const FixedString &fs) noexcept {
        for (std::size_t i = 0; i < fs.size(); ++i)
            os << (fs[i] == '\x01' ? '|' : fs[i]);
        return os;
    }
#endif

};

// =============================================================
// Deduction template (C++17+)
// =============================================================
template <std::size_t N>
FixedString(const char (&)[N]) -> FixedString<N-1>;

template <std::size_t N>
constexpr FixedString<N>::FixedString(const char* s) noexcept {
    assign(s);
}

} // namespace falcon
