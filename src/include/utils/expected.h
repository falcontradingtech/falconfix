// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
#pragma once

#if __cplusplus <= 202002L
#pragma once
#include <variant>
#include <type_traits>
#include <stdexcept>
#include <utility>

namespace utils {

    // unexpected
    template <typename E>
    struct unexpected {
        E value;
        explicit unexpected(const E& e) : value(e) {}
        explicit unexpected(E&& e) : value(std::move(e)) {}
    };

    // primary template: T != void
    template <typename T, typename E>
    class expected {
        std::variant<T, unexpected<E>> storage;

        bool holds_value() const noexcept { return std::holds_alternative<T>(storage); }

    public:
        // Constructors
        expected() : storage(T{}) {}
        expected(const T& value) : storage(value) {}
        expected(T&& value) : storage(std::move(value)) {}
        expected(const unexpected<E>& e) : storage(e) {}
        expected(unexpected<E>&& e) : storage(std::move(e)) {}

        expected(const expected&) = default;
        expected(expected&&) = default;
        expected& operator=(const expected&) = default;
        expected& operator=(expected&&) = default;

               // Observers
        bool has_value() const noexcept { return holds_value(); }

        T& value() & {
            if (!holds_value()) throw std::runtime_error("Bad expected access");
            return std::get<T>(storage);
        }
        const T& value() const & {
            if (!holds_value()) throw std::runtime_error("Bad expected access");
            return std::get<T>(storage);
        }
        T&& value() && {
            if (!holds_value()) throw std::runtime_error("Bad expected access");
            return std::get<T>(std::move(storage));
        }

        E& error() & {
            if (holds_value()) throw std::runtime_error("No error in expected");
            return std::get<unexpected<E>>(storage).value;
        }
        const E& error() const & {
            if (holds_value()) throw std::runtime_error("No error in expected");
            return std::get<unexpected<E>>(storage).value;
        }
        E&& error() && {
            if (holds_value()) throw std::runtime_error("No error in expected");
            return std::get<unexpected<E>>(std::move(storage)).value;
        }

               // Dereference
        T& operator*() & { return value(); }
        const T& operator*() const & { return value(); }
        T* operator->() { return &value(); }
        const T* operator->() const { return &value(); }

               // Swap
        void swap(expected& other) noexcept { storage.swap(other.storage); }

               // value_or
        T value_or(T&& default_value) const & { return has_value() ? value() : default_value; }
        T value_or(T&& default_value) && { return has_value() ? std::move(value()) : default_value; }
    };

    // specialization: T = void
    template <typename E>
    class expected<void, E> {
        std::variant<std::monostate, unexpected<E>> storage;

        bool holds_value() const noexcept { return std::holds_alternative<std::monostate>(storage); }

    public:
        expected() : storage(std::monostate{}) {}
        expected(const unexpected<E>& e) : storage(e) {}
        expected(unexpected<E>&& e) : storage(std::move(e)) {}

        expected(const expected&) = default;
        expected(expected&&) = default;
        expected& operator=(const expected&) = default;
        expected& operator=(expected&&) = default;

        bool has_value() const noexcept { return holds_value(); }

        void value() const {
            if (!holds_value()) throw std::runtime_error("Bad expected<void> access");
        }

        E& error() & {
            if (holds_value()) throw std::runtime_error("No error in expected<void>");
            return std::get<unexpected<E>>(storage).value;
        }

        const E& error() const & {
            if (holds_value()) throw std::runtime_error("No error in expected<void>");
            return std::get<unexpected<E>>(storage).value;
        }

        E&& error() && {
            if (holds_value()) throw std::runtime_error("No error in expected<void>");
            return std::get<unexpected<E>>(std::move(storage)).value;
        }

        void swap(expected& other) noexcept { storage.swap(other.storage); }

        explicit operator bool() const noexcept {
            return has_value();
        }
    };

} // namespace std


#else
#   include <expected>
#endif
