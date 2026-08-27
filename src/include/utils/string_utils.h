// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#include <charconv>
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace falconfix {

static constexpr double DBLZERO=0.0;
static constexpr char VZERO='0';
static constexpr char END='\0';
static constexpr char SIGNAL='-';
static constexpr char POINT='.';
static constexpr uint64_t BASE=10;
#define TO_DIGIT(ch) (static_cast<uint8_t>(ch)-VZERO)
#define INDEX_ATOI(v) TO_DIGIT(str[(size-v)])

constexpr int32_t count_digits(int64_t n) {
    /*
        2'147'483'647                   // 32 => max 10 digits
        9'223'372'036'854'775'807       // 64 => max 20 digits
    */
    if (n<0) n=(n==INT64_MIN)?INT64_MAX:-n;
    if (n<10) return 1;
    if (n<100) return 2;
    if (n<1'000) return 3;
    if (n<10'000) return 4;
    if (n<100'000) return 5;
    if (n<1'000'000) return 6;
    if (n<10'000'000) return 7;
    if (n<100'000'000) return 8;
    if (n<1'000'000'000) return 9;
    if (n<10'000'000'000) return 10;
    if (n<100'000'000'000) return 11;
    if (n<1'000'000'000'000) return 12;
    if (n<10'000'000'000'000) return 13;
    if (n<100'000'000'000'000) return 14;
    if (n<1'000'000'000'000'000) return 15;
    if (n<10'000'000'000'000'000) return 16;
    if (n<100'000'000'000'000'000) return 17;
    if (n<1'000'000'000'000'000'000) return 18;
    return 19;
}

inline std::size_t itoa(const int64_t value, char *out) {
    if (value<0) {
        int64_t tmp=-value;
        char *reverse=out;
        reverse+=count_digits(tmp)+1;
        char *end=reverse;
        do {
            *(--reverse)=static_cast<char>(VZERO+tmp%BASE);
            tmp/=BASE;
        } while (tmp!=0);
        *(--reverse)=SIGNAL;
        *(++end)=END;
        return static_cast<std::size_t>(end - out - 1);
    }
    else {
        int64_t tmp=value;
        char *reverse=out;
        reverse+=count_digits(tmp);
        char *end=reverse;
        do {
            *(--reverse)=static_cast<char>(VZERO+tmp%BASE);
            tmp/=BASE;
        } while (tmp!=0);
        *(++end)=END;
        return static_cast<std::size_t>(end - out - 1);
    }
}

inline std::size_t utoa(const std::size_t value, char *out) {
    size_t tmp=value;
    char *reverse=out;
    reverse+=count_digits(tmp);
    char *end=reverse;
    do {
        *(--reverse)=static_cast<char>(VZERO+tmp%BASE);
        tmp/=BASE;
    } while (tmp!=0);
    *(++end)=END;
    return static_cast<std::size_t>(end - out - 1);

}

inline int64_t atoi(const char *str, const char *end) {
    const bool neg=(str[0]==SIGNAL);
    int64_t size=end-str;
    if (size>19 || size==0) return 0;
    int64_t value=0;

    if (neg) {
        ++str;
        --size;
    }
    switch (size)
    {
        case 19:
            value += static_cast<int64_t>(INDEX_ATOI(19)) * 1000000000000000000LL;
            [[fallthrough]];
        case 18:
            value += static_cast<int64_t>(INDEX_ATOI(18)) * 100000000000000000LL;
            [[fallthrough]];
        case 17:
            value += static_cast<int64_t>(INDEX_ATOI(17)) * 10000000000000000LL;
            [[fallthrough]];
        case 16:
            value += static_cast<int64_t>(INDEX_ATOI(16)) * 1000000000000000LL;
            [[fallthrough]];
        case 15:
            value += static_cast<int64_t>(INDEX_ATOI(15)) * 100000000000000LL;
            [[fallthrough]];
        case 14:
            value += static_cast<int64_t>(INDEX_ATOI(14)) * 10000000000000LL;
            [[fallthrough]];
        case 13:
            value += static_cast<int64_t>(INDEX_ATOI(13)) * 1000000000000LL;
            [[fallthrough]];
        case 12:
            value += static_cast<int64_t>(INDEX_ATOI(12)) * 100000000000LL;
            [[fallthrough]];
        case 11:
            value += static_cast<int64_t>(INDEX_ATOI(11)) * 10000000000LL;
            [[fallthrough]];
        case 10:
            value += static_cast<int64_t>(INDEX_ATOI(10)) * 1000000000LL;
            [[fallthrough]];
        case 9:
            value += static_cast<int64_t>(INDEX_ATOI(9)) * 100000000LL;
            [[fallthrough]];
        case 8:
            value += static_cast<int64_t>(INDEX_ATOI(8)) * 10000000LL;
            [[fallthrough]];
        case 7:
            value += static_cast<int64_t>(INDEX_ATOI(7)) * 1000000LL;
            [[fallthrough]];
        case 6:
            value += static_cast<int64_t>(INDEX_ATOI(6)) * 100000LL;
            [[fallthrough]];
        case 5:
            value += static_cast<int64_t>(INDEX_ATOI(5)) * 10000LL;
            [[fallthrough]];
        case 4:
            value += static_cast<int64_t>(INDEX_ATOI(4)) * 1000LL;
            [[fallthrough]];
        case 3:
            value += static_cast<int64_t>(INDEX_ATOI(3)) * 100LL;
            [[fallthrough]];
        case 2:
            value += static_cast<int64_t>(INDEX_ATOI(2)) * 10LL;
            [[fallthrough]];
        case 1:
            value += static_cast<int64_t>(INDEX_ATOI(1));
        default:
            break;
    }

    if (neg) value=-value;
    return value;
}

inline bool is_digit(char c) noexcept {
    return TO_DIGIT(c) <= 9;
}

inline bool parse_uint32(const char *begin, const char *end, uint32_t &out) noexcept {
    if (begin == nullptr || end == nullptr || begin >= end) {
        return false;
    }

    const auto result = std::from_chars(begin, end, out);
    return result.ec == std::errc{} && result.ptr == end;
}

// ------------------------------------------------------------
//  FalconDtoa - HFT Ryu-based dtoa (double â†’ shortest string)
// ------------------------------------------------------------
static inline int32_t pow5Factor(uint64_t value) {
    int32_t count = 0;
    while (value % 5 == 0) {
        value /= 5;
        count++;
    }
    return count;
}

static inline bool multipleOfPowerOf5(uint64_t value, int32_t p) {
    return pow5Factor(value) >= p;
}

static inline bool multipleOfPowerOf2(uint64_t value, int32_t p) {
    return (value & ((1ull << p) - 1)) == 0;
}

static const uint64_t POW10[] = {
    1ull,
    10ull,
    100ull,
    1000ull,
    10000ull,
    100000ull,
    1000000ull,
    10000000ull,
    100000000ull,
    1000000000ull,
    10000000000ull,
    100000000000ull,
    1000000000000ull,
    10000000000000ull,
    100000000000000ull,
    1000000000000000ull,
    10000000000000000ull,
    100000000000000000ull,
    1000000000000000000ull,
};

inline std::size_t dtoa(double f, char *out, int32_t precision = 3) noexcept {
    if (f != f) [[unlikely]] {
        std::memcpy(out, "nan", 4);
        return 3;
    }

    if (f == DBLZERO) [[unlikely]] {
        out[0] = VZERO;
        out[1] = END;
        return 1;
    }

    bool neg = f < 0;
    if (neg) f = -f;

    uint64_t intpart = (uint64_t)f;
    double frac = f - (double)intpart;

    char *start = out;

    // sign
    if (neg) *out++ = SIGNAL;

    // integer part
    char *int_begin = out;
    uint64_t v = intpart;
    do {
        *out++ = static_cast<char>(VZERO + (v % 10));
        v /= 10;
    } while (v != 0);
    char *int_end = out - 1;
    while (int_begin < int_end) {
        const char c = *int_begin;
        *int_begin++ = *int_end;
        *int_end-- = c;
    }

    // fraction
    if (precision > 0) {
        char* frac_start = out;
        *out++ = POINT;

        char* frac_write = out;

        for (int32_t i = 0; i < precision; ++i) {
            frac *= 10.0;
            int32_t digit = static_cast<int32_t>(frac);
            *frac_write++ = VZERO + digit;
            frac -= digit;
        }

        // trim trailing zeros
        char* end = frac_write;
        while (end > out && *(end - 1) == VZERO) {
            --end;
        }

        // if something remains, keep it // no fractional digits left â†’ remove '.'
        out = (end > out) ? end : frac_start;
    }

    *out = END;
    return static_cast<std::size_t>(out - start);
}

} // namespace falconfix
