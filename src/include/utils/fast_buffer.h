// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <string_view>
#include <string>
#include <charconv>
#include <assert.h>

namespace falconfix {

class FastBuffer {
    char *m_data{nullptr};
    char *m_read{nullptr};
    char *m_write{nullptr};
    char *m_cap{nullptr};

public:
    using size_type = std::size_t;

    FastBuffer() noexcept = default;
    explicit FastBuffer(size_type cap) { reserve(cap); }

    FastBuffer(const FastBuffer&) = delete;
    FastBuffer& operator=(const FastBuffer&) = delete;

    FastBuffer(FastBuffer&& o) noexcept { steal(o); }
    FastBuffer& operator=(FastBuffer&& o) noexcept {
        if (this!=&o){ release(); steal(o); }
        return *this;
    }

    ~FastBuffer(){ release(); }

    constexpr char& operator[](size_type i) noexcept { return m_data[i]; }
    constexpr const char& operator[](size_type i) const noexcept { return m_data[i]; }

    size_type size() const noexcept { return m_write - m_read; }
    size_type capacity() const noexcept { return m_cap - m_data; }
    bool empty() const noexcept { return m_read == m_write; }
    const char *data() const noexcept { return m_read; }
    char *data() noexcept { return m_read; }
    std::string_view view() const noexcept { return {m_read, size()}; }
    operator std::string_view() const noexcept { return view(); }

    void clear() noexcept {
        m_read = m_write = m_data;
        if (m_write) {
            *m_write = '\0';
        }
    }

    void consume(size_type n) noexcept {
        const size_type sz = size();
        if (n >= sz) {
            clear();
            return;
        }
        m_read += n;
    }

    void finalize() noexcept { *m_write = '\0'; }
    void commit(size_type n) noexcept { m_write += n; *m_write = '\0'; }

    void reserve(size_type n) {
        const size_type required = n + 1;
        if (required > capacity()) {
            grow(required);
        }
    }

    bool reserve_noexcept(size_type n) noexcept {
        const size_type required = n + 1;
        if (required > capacity()) {
            return grow_noexcept(required);
        }
        return true;
    }

    // =============================================================
    // APPEND METHODS (like std::string)
    // =============================================================
    inline FastBuffer& append(const char *cpp, size_type n) noexcept {
        ensureWritable(n);
        std::memcpy(m_write,cpp,n);
        m_write+=n;
        return *this;
    }

    inline FastBuffer& append(std::string_view sv) noexcept {
        return append(sv.data(),sv.size());
    }

    inline FastBuffer& append(char c) noexcept {
        ensureWritable(1);
        *m_write++=c;
        return *this;
    }

    template<size_type N>
    FastBuffer &append(const char(&lit)[N]) noexcept {
        return append(lit,N-1);
    }

    inline FastBuffer &append(size_type n, char c) noexcept {
        ensureWritable(n);
        std::memset(m_write,c,n);
        m_write+=n;
        return *this;
    }

    // =============================================================
    // REPLACE METHODS (in-place, like std::string::replace)
    // =============================================================
    // replace(pos, len, "newtext") â€” same size // â†’ becomes just memcpy (zero overhead)
    /*constexpr */FastBuffer &replace(size_t pos, size_t len, const char *s) noexcept {
        assert(pos + len <= capacity());
        std::memcpy(m_write + pos, s, len);
        return *this;
    }

    // literal â€” compiler infers size â†’ 100% safe and fast
    template <size_t N>
    constexpr FastBuffer& replace(size_t pos, size_t len, const char (&lit)[N]) noexcept {
        constexpr size_t literal_len = N - 1;
        assert(len <= literal_len);
        assert(pos + len <= capacity());
        std::memcpy(m_write + pos, lit, len);
        return *this;
    }

    // string_view â€” you already know the size
    /*constexpr */ FastBuffer &replace(size_t pos, size_t len, std::string_view sv) noexcept {
        assert(sv.size() == len);                 // optional, but helps catch misuse
        assert(pos + len <= capacity());          // <-- REQUIRED
        std::memcpy(m_write + pos, sv.data(), len);
        return *this;
    }

private:
    void ensureWritable(size_type n) {
        const size_type tail = m_cap - m_write;
        if (tail >= n + 1) {
            return;
        }
        const size_type sz = size();
        const size_type head = m_read - m_data;
        if (head + tail >= n + 1) {
            std::memmove(m_data, m_read, sz);
            m_read = m_data;
            m_write = m_data + sz;
            *m_write = '\0';
            return;
        }
        grow(sz + n + 1);
    }

    void grow(size_type min_capacity = 0) {
        static constexpr size_type page = 4096;

        const size_type used = size();
        size_type new_cap = capacity();

        if (new_cap < 512) new_cap = 512;
        while (new_cap < min_capacity) new_cap <<= 1;

        new_cap = (new_cap + page - 1) & ~(page - 1);

        char *p = static_cast<char *>(std::malloc(new_cap));
        if (!p) [[unlikely]] throw std::bad_alloc();

        if (used > 0) std::memcpy(p, m_read, used);
        std::free(m_data);

        m_data = p;
        m_read = p;
        m_write = p + used;
        m_cap = p + new_cap;
        *m_write = '\0';
    }

    bool grow_noexcept(size_type min_capacity = 0) noexcept {
        static constexpr size_type page = 4096;

        const size_type used = size();
        size_type new_cap = capacity();

        if (new_cap < 512) new_cap = 512;
        while (new_cap < min_capacity) new_cap <<= 1;

        new_cap = (new_cap + page - 1) & ~(page - 1);

        char *p = static_cast<char *>(std::malloc(new_cap));
        if (!p) [[unlikely]] return false;

        if (used > 0) std::memcpy(p, m_read, used);
        std::free(m_data);

        m_data = p;
        m_read = p;
        m_write = p + used;
        m_cap = p + new_cap;
        *m_write = '\0';

        return true;
    }

    void release() {
        if (m_data) {
            std::free(m_data);
        }

        m_data = nullptr;
        m_read = nullptr;
        m_write = nullptr;
        m_cap = nullptr;
    }

    void steal(FastBuffer &o) {
        m_data = o.m_data;
        m_read = o.m_read;
        m_write = o.m_write;
        m_cap = o.m_cap;

        o.m_data = nullptr;
        o.m_read = nullptr;
        o.m_write = nullptr;
        o.m_cap = nullptr;
    }
};

} // namespace falconfix
