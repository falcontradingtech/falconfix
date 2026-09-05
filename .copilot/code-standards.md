# FalconFIX C++20 Code Standards (Compressed)

---

## 📋 Naming Conventions

| Element | Style | Example |
|---------|-------|---------|
| **Files** | PascalCase | `SessionManager.h`, `SessionManager.cpp` |
| **Classes** | PascalCase | `class SessionManager { }` |
| **Structs** | PascalCase | `struct ConfigOptions { }` |
| **Functions** | snake_case | `void send_message()` |
| **Methods** | snake_case | `obj.process_data()` |
| **Variables** | snake_case | `int message_count` |
| **Member vars** | snake_case_ | `std::string session_id_` |
| **Constants** | UPPER_CASE | `constexpr int MAX_SIZE = 100` |
| **Namespaces** | snake_case | `namespace falcon_fix::session` |
| **Templates** | Descriptive | `template<typename MessageType>` |
| **Enum class** | PascalCase | `enum class Status { Active, Inactive }` |
| **Enum values** | PascalCase | `Status::Active` |

---

## 📝 File Structure

### Header Files (`src/include/module/ClassName.h`)

```cpp
#ifndef FALCON_FIX_MODULE_CLASS_NAME_H
#define FALCON_FIX_MODULE_CLASS_NAME_H

// SPDX-License-Identifier: MIT
// Copyright (c) 2026 [Author]

// 1. System includes (alphabetical)
#include <algorithm>
#include <memory>
#include <string>

// 2. External includes (alphabetical)
#include <fmt/format.h>
#include <spdlog/spdlog.h>

// 3. Project includes (alphabetical)
#include "logger/Logger.h"
#include "module/OtherClass.h"

namespace falcon_fix::module {

/// Brief one-liner description.
/// \details Longer explanation of what this class does,
/// any important behaviors, and usage patterns.
/// \see RelatedClass
class ClassName {
public:
    // Type aliases
    using value_type = int;
    using iterator = std::vector<int>::iterator;

    // Constructors (always explicit unless copy constructor)
    explicit ClassName(std::string_view name, int size);

    // Destructor
    ~ClassName();

    // Deleted copy operations (move-only by default)
    ClassName(const ClassName&) = delete;
    ClassName& operator=(const ClassName&) = delete;

    // Default move operations
    ClassName(ClassName&&) = default;
    ClassName& operator=(ClassName&&) = default;

    // Core operations
    /// Does something important.
    /// \param value Input value
    /// \return Result of operation
    /// \throws std::invalid_argument if value is negative
    int do_work(int value);

    // Query methods
    /// Returns true if object is in active state.
    bool is_active() const;

    /// Returns current size.
    size_t size() const;

private:
    // Private implementation
    void validate_input(int value);

    // Member variables
    std::string name_;
    std::vector<int> data_;
    int size_{0};
};

}  // namespace falcon_fix::module

#endif
````

### Implementation Files (`src/cpp/module/ClassName.cpp`)

```cpp
#include "module/ClassName.h"

#include <algorithm>
#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include "logger/Logger.h"

namespace falcon_fix::module {

ClassName::ClassName(std::string_view name, int size)
    : name_(name), size_(size) {
    auto logger = logger::GetLogger("ClassName");
    logger->debug("Created with name={}, size={}", name, size);
    data_.reserve(size);  // Pre-allocate
}

ClassName::~ClassName() = default;

int ClassName::do_work(int value) {
    validate_input(value);
    // Implementation
    return result;
}

}  // namespace falcon_fix::module
```

---

## 🔤 Class Design Patterns

### Constructor Style

```cpp
class MyClass {
public:
    // Always explicit (prevents implicit conversions)
    explicit MyClass(int value);

    // No default constructor (unless meaningful)
    MyClass() = delete;  // Or don't declare

    // Explicitly delete copy (most classes should be move-only)
    MyClass(const MyClass&) = delete;
    MyClass& operator=(const MyClass&) = delete;

    // Allow move (generates optimized code)
    MyClass(MyClass&&) = default;
    MyClass& operator=(MyClass&&) = default;

    // Defaulted destructor (unless cleanup needed)
    ~MyClass() = default;
};
```

### Method Organization

```cpp
class EventHandler {
public:
    // Type aliases first
    using EventCallback = std::function<void(const Event&)>;

    // Constructors
    explicit EventHandler(std::string_view name);

    // Core operations (grouped by functionality)
    void register_listener(EventCallback callback);
    void unregister_listener(EventCallback callback);
    void dispatch_event(const Event& event);

    // Query methods
    size_t listener_count() const;
    bool has_listeners() const;

private:
    // Implementation details
    std::vector<EventCallback> listeners_;
};
```

### Getters and Setters

```cpp
class Config {
public:
    // For expensive types, return const reference
    const std::string& name() const { return name_; }

    // For cheap types, return by value
    int timeout_ms() const { return timeout_ms_; }

    // Boolean queries with is_/has_ prefix
    bool is_debug_enabled() const { return debug_enabled_; }
    bool has_ssl_enabled() const { return ssl_enabled_; }

    // Setters only if necessary (prefer builder pattern for complex)
    void set_name(std::string_view new_name) { name_ = new_name; }

private:
    std::string name_;
    int timeout_ms_{5000};
    bool debug_enabled_{false};
    bool ssl_enabled_{false};
};
```

---

## 🎯 Function Guidelines

### Signature Style

```cpp
// Prefer modern return types
std::optional<Session> create_session(const Config& cfg);  // May fail
std::expected<Message, Error> parse_message(
    std::span<const uint8_t> data
);  // With error

// Exceptions for exceptional cases
try {
    auto session = create_session(cfg);
} catch (const std::exception& e) {
    logger->error("Failed: {}", e.what());
}

// Use noexcept for functions that truly never throw
void log_message(std::string_view msg) noexcept;

// Parameters: references for mutations, const references for read-only
void process_connection(Connection& conn);     // Mutable
void log_message(const Message& msg);           // Read-only
void parse_frame(std::span<const uint8_t> data); // Span instead of ptr+len

// String parameters: use std::string_view
void set_name(std::string_view name);

// Smart pointers for ownership transfer
std::unique_ptr<Session> create_session();
std::shared_ptr<Resource> get_resource();
```

### Function Length

* **Target**: ≤ 50 lines (including braces)
* **Acceptable**: ≤ 100 lines (split if possible)
* **Refactor**: > 100 lines (break into smaller functions)

---

## 🧠 Type Safety

### Smart Pointers Only

```cpp
// ✅ CORRECT
{
    auto session = std::make_unique<Session>("ID");
    session->send_message(msg);
}  // Automatically cleaned up

std::shared_ptr<Logger> logger = GetLogger();  // Shared ownership

Session& session = get_session();  // Non-owning reference

// ❌ WRONG - Never do this
Session* session = new Session("ID");  // Who owns it?
delete session;  // Possible memory leak if exception thrown
```

### Optional & Expected

```cpp
// Use optional when operation might fail without error details
std::optional<User> find_user(int id) {
    if (users_.contains(id)) {
        return users_.at(id);
    }
    return std::nullopt;
}

// Use optional result
if (auto user = find_user(123)) {
    process(*user);
} else {
    log("User not found");
}

// Use expected when you need error information
std::expected<Message, ParseError> parse_message(
    std::span<const uint8_t> data
) {
    if (data.empty()) {
        return std::unexpected(ParseError::EmptyInput);
    }
    return Message{data};
}

// Use expected result
if (auto msg = parse_message(frame)) {
    handle(*msg);
} else {
    log("Parse failed: {}", msg.error());
}
```

### Spans Over Pointers

```cpp
// ✅ GOOD - More information, safer
void process(std::span<const uint8_t> buffer) {
    for (auto byte : buffer) {
        // Can't go out of bounds
    }
}

// ❌ AVOID - Loses length information
void process(const uint8_t* buffer, size_t len) {
    // Easy to pass wrong length
}

// Use
auto data = read_frame();
process(data);
```

---

## 🔗 Error Handling

### Exceptions for Exceptional Cases

```cpp
// ✅ Use exceptions for:
// - Constructor failures
// - Validation errors
// - "Impossible" conditions

class Session {
public:
    explicit Session(std::string_view id) {
        if (id.empty()) {
            throw std::invalid_argument("Session ID cannot be empty");
        }
    }
};

// Usage
try {
    auto session = Session("");  // Throws
} catch (const std::invalid_argument& e) {
    logger->error("Invalid input: {}", e.what());
}
```

### Error Codes for Expected Alternatives

```cpp
// ✅ Use error codes for expected failures
std::optional<Message> queue_message(const Message& msg) {
    if (queue_.full()) {
        return std::nullopt;  // Expected condition
    }
    return queue_.push(msg);
}

// Usage (no exception handling needed)
if (auto result = queue_message(msg)) {
    // Success
} else {
    // Queue was full (expected)
}
```

---

## 📝 Comments

### Good Comments (Explain WHY)

```cpp
// We skip malformed frames to prevent parser state corruption
// (allows recovery in next iteration)
if (frame.length < MIN_FRAME_SIZE) {
    logger->warn("Skipping malformed frame");
    continue;
}

// Boyer-Moore is faster than naive search for long patterns
// Time: O(n + m), Space: O(m) where n=text_len, m=pattern_len
// Reference: https://en.wikipedia.org/wiki/Boyer%E2%80%93Moore_string_search
auto pos = boyer_moore_search(text, pattern);
```

### Doxygen Documentation (Public API)

```cpp
/// Sends a FIX message over the network.
///
/// This method adds the message to the outbound queue.
/// The message is serialized and checksummed before transmission.
///
/// \param msg The message to send (must be valid)
/// \param timeout Timeout in milliseconds (0 = infinite)
/// \return true if queued successfully
/// \throws std::invalid_argument if msg is invalid
/// \throws std::system_error on socket failure
///
/// \see send_message_async() for non-blocking variant
/// \warning This is thread-safe
bool send_message(const Message& msg, int timeout = 0);
```

### Bad Comments (Don't Do)

```cpp
// ❌ Just repeats code
int count = messages.size();  // Get size of messages

// ❌ Stating the obvious
if (queue.empty()) {  // Check if queue is empty
    return;
}

// ❌ Outdated comments
// FIXME: This is broken  (no explanation, no issue reference)
// TODO: Add caching  (by whom? when? priority?)
```

---

## 🧪 Testing Patterns

```cpp
class MyClassTests : public ::testing::Test {
protected:
    void SetUp() override {
        obj_ = std::make_unique<MyClass>();
    }

    std::unique_ptr<MyClass> obj_;
};

// Happy path: everything works
TEST_F(MyClassTests, DoWork_WithValidInput_Succeeds) {
    auto result = obj_->do_work(42);
    EXPECT_EQ(result, expected);
}

// Error path: invalid input
TEST_F(MyClassTests, DoWork_WithNegativeValue_Throws) {
    EXPECT_THROW(
        obj_->do_work(-1),
        std::invalid_argument
    );
}

// Edge case: boundary condition
TEST_F(MyClassTests, DoWork_WithZero_ReturnsZero) {
    auto result = obj_->do_work(0);
    EXPECT_EQ(result, 0);
}
```

---

## ⚡ Modern C++20 Features

### Structured Bindings

```cpp
// ✅ GOOD - Cleaner
auto [id, status] = get_session_info();

// ❌ AVOID - Verbose
auto result = get_session_info();
std::string id = result.first;
SessionStatus status = result.second;
```

### Range-Based For

```cpp
// ✅ GOOD - Idiomatic
for (const auto& session : sessions) {
    session->process();
}

// ❌ AVOID - Index loops
for (size_t i = 0; i < sessions.size(); ++i) {
    sessions[i]->process();
}
```

### Concepts (C++20)

```cpp
template<typename T>
concept Serializable = requires(T t, std::ostream& os) {
    { os << t } -> std::convertible_to<std::ostream&>;
};

template<Serializable T>
void send_message(const T& msg) {
    // T is guaranteed to be serializable
}
```

---

## 🔄 Include Order

1. Associated header (for `.cpp` only)
2. System headers (alphabetical)
3. External libraries (alphabetical)
4. Project headers (alphabetical)
5. Blank line between groups

```cpp
#include "module/MyClass.h"           // Associated (if .cpp)

#include <algorithm>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include <boost/asio.hpp>
#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include "config/Config.h"
#include "logger/Logger.h"
#include "utils/String.h"
```

---

## ✅ Code Review Checklist

Before submitting:

* [ ] Follows naming conventions (all of them)
* [ ] Smart pointers only (no raw pointers)
* [ ] Public API has Doxygen docs (`///` comments)
* [ ] Complex code has explanatory comments (WHY, not WHAT)
* [ ] Includes organized (system → external → project)
* [ ] Functions < 50 lines (or good reason)
* [ ] Tests included (80%+ coverage minimum)
* [ ] No compiler warnings
* [ ] Error handling correct (exceptions vs. returns)
* [ ] No global mutable state
* [ ] No raw `new`/`delete`
* [ ] Memory-safe (no buffer overflows)
* [ ] Move semantics correct (allow move when possible)
* [ ] const-correct (mark immutable as const)