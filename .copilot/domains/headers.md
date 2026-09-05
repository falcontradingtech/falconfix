# Public API Headers

## Location
`src/include/` - All public interfaces

## Template
```cpp
// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2024 FalconTradingTech
#pragma once

#include <memory>
#include <string>
#include <expected>  // C++20

namespace falconfix {

class MyClass {
public:
  MyClass() = default;
  ~MyClass() = default;

  // Move semantics
  MyClass(MyClass&&) noexcept = default;
  MyClass& operator=(MyClass&&) noexcept = default;

  // Delete copy
  MyClass(const MyClass&) = delete;
  MyClass& operator=(const MyClass&) = delete;

  std::expected<void, std::string> DoSomething();

private:
  std::unique_ptr<Impl> impl_;
};

}  // namespace falconfix
```

## DO
✓ Use `#pragma once` guards
✓ Place copyright SPDX header
✓ Forward declare when possible
✓ Use `std::unique_ptr` for ownership
✓ Use `std::expected` for error handling
✓ Document public methods

## DON'T
✗ Include implementation details
✗ Use `#ifdef` guards
✗ Expose internal classes
✗ Create circular dependencies
✗ Put implementation in headers

## Checklist
- [ ] SPDX + Copyright header
- [ ] `#pragma once`
- [ ] Minimal includes
- [ ] Forward declarations
- [ ] Smart pointers only
- [ ] Move semantics
- [ ] Doxygen docs
- [ ] No implementation
