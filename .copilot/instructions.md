# FalconFIX Core AI Instructions

**Every AI/Copilot session working on FalconFIX must follow these rules.**

---

## 🛡️ ABSOLUTE RULES (Never Break)

### ✅ DO (Required - Enforce)

1. **Tests First, Always**
   - Write tests BEFORE implementation (TDD)
   - Minimum 80% code coverage on new functions
   - Use GTest framework (Google Test)
   - Pattern: `TEST_F(ClassName, Method_Condition_Expected)`

2. **Smart Pointers Only**
   - Use `std::unique_ptr<T>` for ownership
   - Use `std::shared_ptr<T>` for shared ownership
   - Never use `new`/`delete` directly
   - Never use raw pointers (except non-owning references)

3. **Respect Architecture Boundaries**
   - Check `.copilot/domains/architecture.md` before coding
   - Don't cross module boundaries without reason
   - Session → Connection → Runtime → Socket (dependency direction)
   - No circular dependencies allowed

4. **Documentation is Mandatory**
   - Public APIs get Doxygen comments (`/// brief \details longer`)
   - Complex logic gets explanatory comments
   - Update relevant `.md` file if behavior changes
   - Commit message includes WHAT and WHY

### ❌ DON'T (Forbidden - Blocked)

1. **Never Touch src/engine/**
   - `src/engine/` is AUTO-GENERATED from FIX XML
   - Any changes vanish on next generation
   - Errors in protocol? Fix the code generator, not generated files
   - Exceptions: Code generator itself

2. **Never Modify CMakeLists.txt Without Approval**
   - Build system is carefully tuned
   - Each change can break incremental builds
   - If modification needed:
	 - Explain WHY in commit message
	 - Discuss in issue first
	 - Provide rollback plan

3. **Never Skip Tests**
   - Every implementation needs test coverage
   - Every bug fix includes regression test
   - No "TODO add tests later"
   - Use test fixtures for setup/teardown

4. **Never Use Raw Pointers**
   - No `T* ptr = new T()`
   - No `delete ptr`
   - Exception: Non-owning references (safe pointer received)

5. **Never Add Global State**
   - No global variables
   - No static mutable state (unless synchronized)
   - Use dependency injection instead

6. **Never Bypass the State Machine**
   - Session has specific state transitions
   - Don't directly set state
   - Always use transition methods
   - See `domains/session.md` for diagram

---

## 🎯 DO FIRST, ALWAYS

Before writing any code:

1. **Research**
   ```
   Search for existing implementations:
   - Use find_symbol to locate similar classes
   - Use grep_search to find patterns
   - Check if code already exists
   ```

2. **Design**
   ```
   Check ARCHITECTURE.md:
   - Which module owns this?
   - What are the dependencies?
   - What patterns apply?
   - Read relevant domains/*.md file
   ```

3. **Plan**
   ```
   Ask yourself:
   - What are the test cases?
   - What are error cases?
   - What data structures needed?
   - Will this fit incremental builds?
   ```

4. **Test First**
   ```
   Write test in tests/<module>/ BEFORE implementation
   This is NOT optional - this is TDD
   ```

5. **Implement**
   ```
   Follow CODE_STYLE.md:
   - Naming conventions (PascalCase/snake_case)
   - File organization
   - Comments explaining WHY
   ```

6. **Documentation**
   ```
   - Add /// Doxygen comments
   - Update relevant .md file
   - Include commit message explanation
   ```

---

## 📋 QUICK DECISION TREE

**"Should I create a new class?"**
→ Check ARCHITECTURE.md module descriptions first

**"Which module should this go in?"**
→ See `domains/architecture.md`

**"What's the naming convention?"**
→ See `code-standards.md`

**"How do I test this?"**
→ See `domains/testing.md`

**"Do I modify CMakeLists.txt?"**
→ Almost never. Use existing `add_lib()`, `add_app()`, `add_test_executable()`

**"What about exceptions vs. error codes?"**
→ See `code-standards.md` - Exceptions for exceptional, codes for expected

**"Should I add a new dependency?"**
→ NO. Discuss in issue first. Almost always NO.

**"Can I edit src/engine/*?"**
→ NO. Ever. Never. Not even for typos.

---

## 🔧 CODING RULES (Fast Reference)

### Naming
```cpp
// Classes: PascalCase
class SessionManager { };

// Functions/methods: snake_case
void send_message() { }

// Variables: snake_case
int message_count = 0;

// Members: snake_case with trailing underscore
std::string session_id_;

// Constants: UPPER_CASE
constexpr int MAX_SIZE = 100;

// Namespaces: snake_case, hierarchical
namespace falcon_fix::session { }
```

### Memory Management
```cpp
// ✅ CORRECT
std::unique_ptr<Message> msg{std::make_unique<Message>()};
std::shared_ptr<Logger> logger{GetLogger()};

// ❌ WRONG
Message* msg = new Message();
delete msg;
Logger* logger = new Logger();
```

### Error Handling
```cpp
// Exceptions for exceptional conditions:
throw std::invalid_argument("Invalid config");

// Optional for operations that may fail:
std::optional<Session> find_session(std::string_view id);

// Expected for detailed error info:
std::expected<Message, ParseError> parse(std::span<const uint8_t> data);
```

### Smart Pointers
```cpp
// Ownership transfer
std::unique_ptr<T> create_object() { return std::make_unique<T>(); }

// Shared ownership (rare)
std::shared_ptr<Resource> get_resource() { return resource_; }

// Non-owning reference
void use_session(Session& session) { }

// Const reference for read-only
void process_message(const Message& msg) { }
```

---

## 📊 TEST ANATOMY (Copy-Paste Template)

```cpp
#include <gtest/gtest.h>
#include "module/MyClass.h"

namespace falcon_fix::tests {

class MyClassTests : public ::testing::Test {
protected:
	void SetUp() override {
		// Called before each test
	}

	void TearDown() override {
		// Called after each test
	}

	std::unique_ptr<MyClass> obj_;
};

// Happy path
TEST_F(MyClassTests, Method_WithValidInput_Succeeds) {
	// Arrange
	// Act
	// Assert
	EXPECT_EQ(result, expected);
}

// Error case mandatory
TEST_F(MyClassTests, Method_WithInvalidInput_Throws) {
	EXPECT_THROW(
		obj_->method(invalid_input),
		std::invalid_argument
	);
}

// Edge case
TEST_F(MyClassTests, Method_WithEmpty_ReturnsEmpty) {
	auto result = obj_->method("");
	EXPECT_FALSE(result.has_value());
}

} // namespace falcon_fix::tests
```

---

## 🔍 FILE ORGANIZATION

### Header Files (src/include/module/MyClass.h)
```cpp
#ifndef FALCON_FIX_MODULE_MY_CLASS_H
#define FALCON_FIX_MODULE_MY_CLASS_H

// SPDX-License-Identifier: MIT
// Copyright (c) 2026 [Author]

#include <memory>
#include <string>

namespace falcon_fix::module {

/// Brief description.
/// \details Longer explanation.
class MyClass {
public:
	explicit MyClass(std::string_view param);  // Always explicit!

	MyClass(const MyClass&) = delete;          // No copy
	MyClass& operator=(const MyClass&) = delete;

	MyClass(MyClass&&) = default;              // Allow move
	MyClass& operator=(MyClass&&) = default;

	~MyClass();

	/// Does something important.
	void do_work(int value);

private:
	std::string param_;
};

} // namespace falcon_fix::module

#endif
```

### Implementation (src/cpp/module/MyClass.cpp)
```cpp
#include "module/MyClass.h"

#include <fmt/format.h>
#include <spdlog/spdlog.h>

namespace falcon_fix::module {

MyClass::MyClass(std::string_view param) : param_(param) {
	auto logger = logger::GetLogger("MyClass");
	logger->debug("Initialized with: {}", param);
}

MyClass::~MyClass() = default;

void MyClass::do_work(int value) {
	// Implementation
}

} // namespace falcon_fix::module
```

---

## 🚀 BUILD SYSTEM RULES

**DO:**
- Use persistent build dir: `build/windows-debug/`
- Incremental builds: never delete build/ between changes
- Test locally: `ctest --preset windows-debug`
- Build: `cmake --build build/windows-debug --config Debug`

**DON'T:**
- Modify root CMakeLists.txt (discuss first!)
- Clean build directory unnecessarily
- Create ad-hoc build directories
- Commit build/ artifacts

**If adding module:**
```cmake
# Create src/cpp/mymodule/CMakeLists.txt
add_lib(
	TARGET falcon_fix_mymodule
	SOURCES MyClass.cpp OtherClass.cpp
	HEADERS ${PROJECT_SOURCE_DIR}/src/include/mymodule/MyClass.h
	DEPENDENCIES fmt::fmt spdlog::spdlog
)
```

---

## 🧪 TESTING CHECKLIST

Before showing code to dev:

- [ ] 80%+ code coverage
- [ ] Happy path tested
- [ ] Error cases tested
- [ ] Edge cases (empty, null, boundary) tested
- [ ] Tests are fast (< 1 sec each)
- [ ] No flaky tests (don't depend on timing)
- [ ] No global state modification
- [ ] Test names are descriptive
- [ ] Mocks are used for dependencies
- [ ] All tests pass: `ctest --preset windows-debug`

---

## 📝 COMMENT RULES

**Good comments** (explain WHY):
```cpp
// We skip malformed frames to prevent parser state corruption
if (frame.length < MIN_SIZE) {
	return;
}
```

**Bad comments** (just repeat code):
```cpp
// Increment count
count++;  // DON'T DO THIS

// Check if empty
if (queue.empty()) {  // DON'T DO THIS
```

---

## 🔄 GIT WORKFLOW

```bash
# 1. Create feature branch
git checkout -b feature/my-feature

# 2. Small commits (one logical change each)
git commit -m "feat: Add send_heartbeat() with tests

- Implements FIX heartbeat protocol
- 90% code coverage
- Updated docs/ARCHITECTURE.md"

# 3. Push and create PR
git push origin feature/my-feature

# 4. PR description includes:
# - Issue reference (#123)
# - What changed and why
# - Testing notes
# - Breaking changes (if any)
```

---

## 📚 WHEN IN DOUBT

1. **Architecture question** → Read `.copilot/domains/architecture.md`
2. **Style question** → Read `code-standards.md`
3. **Testing question** → Read `.copilot/domains/testing.md`
4. **Build question** → Read `.copilot/domains/cmake.md`
5. **Module question** → Read `.copilot/domains/[module].md`
6. **Everything else** → Ask the developer

---

## ✨ GOLDEN RULES

1. **Incremental builds**: Always use same `build/windows-debug/` directory
2. **Architecture first**: Check ARCHITECTURE.md before coding
3. **Tests required**: 80%+ coverage, write tests first
4. **CMake stable**: Never modify unless necessary (discuss!)
5. **Docs mandatory**: Every change includes documentation
6. **Smart pointers**: No raw pointers (ever)
7. **No generated edits**: `src/engine/` is read-only
8. **Respect boundaries**: Module isolation is sacred

---

## 🎯 SUCCESS CRITERIA

Code is ready when:
- ✅ Tests pass (80%+ coverage)
- ✅ No compiler warnings
- ✅ Code follows style guide
- ✅ Incremental build succeeds
- ✅ Docs updated
- ✅ Commit message explains WHY
- ✅ PR includes tests + docs

---

**Questions?** Check `.copilot/domains/` for specific topics, or read root `.md` files.

**Ready to code?** Pick a domain file for your task and load it into context.
