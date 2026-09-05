# Domain: Testing with GTest

**Quick Reference for writing unit tests in FalconFIX.**

---

## 📍 File Location

```text
src/cpp/<module>/MyClass.cpp    →    tests/<module>/MyClassTests.cpp
````

## 🏗️ Test File Template

```cpp
#include <gtest/gtest.h>
#include "module/MyClass.h"

namespace falcon_fix::tests {

class MyClassTests : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup before each test
        obj_ = std::make_unique<MyClass>("default");
    }

    void TearDown() override {
        // Cleanup after each test
        obj_.reset();
    }

    // Helper methods for this test class
    Message create_test_message() { /* ... */ }

    // Test fixtures
    std::unique_ptr<MyClass> obj_;
};

// ... tests follow

}  // namespace falcon_fix::tests
```

## ✅ Test Naming Pattern

```cpp
TEST_F(ClassName, Method_Condition_Expected)

Examples:
TEST_F(SessionTests, SendMessage_WhenConnected_Succeeds)
TEST_F(ParserTests, Parse_WithCorruptChecksum_Throws)
TEST_F(QueueTests, Dequeue_FromEmpty_ReturnsEmpty)
TEST_F(ConfigTests, Load_WithMissingFile_Returns404)
```

## 📋 Three Required Test Cases

Every function needs minimum 3:

### 1. Happy Path (Success Case)

```cpp
TEST_F(SessionTests, SendMessage_WithValidMessage_Succeeds) {
    // Arrange
    auto session = create_test_session();
    Message msg = create_order();

    // Act
    bool result = session->send_message(msg);

    // Assert
    EXPECT_TRUE(result);
    EXPECT_EQ(session->message_count(), 1);
}
```

### 2. Error/Exception Case

```cpp
TEST_F(SessionTests, SendMessage_WhenDisconnected_Throws) {
    auto session = create_test_session();
    session->close();
    Message msg = create_order();

    EXPECT_THROW(
        session->send_message(msg),
        ConnectionClosedException
    );
}
```

### 3. Edge Case (Boundary/Empty/Null)

```cpp
TEST_F(SessionTests, SendMessage_WithEmptyPayload_ReturnsFalse) {
    auto session = create_test_session();
    Message empty_msg;

    EXPECT_FALSE(session->send_message(empty_msg));
}
```

## 🎯 Assertion Quick Reference

| Assertion                 | Use For                   | Stops Test |
| ------------------------- | ------------------------- | ---------- |
| `EXPECT_EQ(a, b)`         | a == b                    | No         |
| `ASSERT_EQ(a, b)`         | a == b (precondition)     | **Yes**    |
| `EXPECT_TRUE(cond)`       | cond is true              | No         |
| `EXPECT_FALSE(cond)`      | cond is false             | No         |
| `EXPECT_THROW(stmt, Exc)` | throws Exc                | No         |
| `ASSERT_THROW(stmt, Exc)` | throws Exc (precondition) | **Yes**    |
| `EXPECT_NO_THROW(stmt)`   | doesn't throw             | No         |
| `EXPECT_NE(a, b)`         | a != b                    | No         |
| `EXPECT_LT(a, b)`         | a < b                     | No         |
| `EXPECT_LE(a, b)`         | a <= b                    | No         |

**Rule**: Use `ASSERT_*` for setup/preconditions, `EXPECT_*` for test assertions.

## 🤖 Mocking Dependencies

```cpp
#include <gmock/gmock.h>
#include "connection/Connection.h"

class MockConnection : public Connection {
public:
    MOCK_METHOD(void, send, (const Message&), (override));
    MOCK_METHOD(bool, is_connected, (), (const, override));
    MOCK_METHOD(void, close, (), (override));
};

class SessionTests : public ::testing::Test {
protected:
    MockConnection mock_conn_;
};

TEST_F(SessionTests, SendsViaConnection) {
    // Verify mock is called
    EXPECT_CALL(mock_conn_, send(::testing::_))
        .Times(1)
        .WillOnce(::testing::Return());

    Session session(&mock_conn_);
    session.send_message(Message{});
}
```

## 📊 Coverage Requirements

* **New code**: Minimum 80% coverage (REQUIRED)
* **Modified code**: Maintain or improve coverage
* **Complex logic**: Aim for 95%+
* **Public API**: 100% preferred

**Check coverage**:

```powershell
cmake --preset windows-debug
ctest --preset windows-debug
# Report: build/windows-debug/coverage/index.html
```

## 🔧 CMakeLists.txt for Tests

```cmake
# tests/mymodule/CMakeLists.txt

add_test_executable(
    TARGET MyModuleTests
    SOURCES
        MyClassTests.cpp
        FeatureTests.cpp
    DEPENDENCIES
        GTest::gtest_main     # Main test runner
        GTest::gmock          # For mocking
        falcon_fix_mymodule   # What we're testing
)
```

## 🏃 Running Tests

```powershell
# All tests
ctest --preset windows-debug

# Specific suite
ctest --preset windows-debug -R MyModuleTests

# With output
ctest --preset windows-debug --output-on-failure

# Parallel (faster)
ctest --preset windows-debug --parallel 4

# Only failed
ctest --preset windows-debug --rerun-failed
```

## ⛔ Test Anti-Patterns

**❌ DON'T**:

* Test implementation details (test behavior)
* Create multiple assertions in one test
* Make tests depend on execution order
* Use `sleep()` or time-based assertions
* Modify global state
* Hit real network/files
* Have non-deterministic results

**✅ DO**:

* Test one logical thing per test
* Use mocks for external dependencies
* Name tests descriptively
* Keep tests fast (< 1 sec each)
* Use fixtures for setup
* Clean up in `TearDown`

## 📚 Quick Links

* [GTest Primer](https://google.github.io/googletest/primer.html)
* [GMock CookBook](https://google.github.io/googletest/gmock_cook_book.html)