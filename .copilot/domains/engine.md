# Generated Protocol Layer

⚠️ **DO NOT EDIT FILES IN `src/engine/`**

## Why?
The entire `src/engine/` directory is **auto-generated** from FIX specs.
Manual edits will be overwritten on next generation.

## Generated Artifacts
- `src/engine/messages/` - FIX message classes
- `src/engine/fields/` - FIX field types
- `src/engine/tags/` - FIX tag definitions
- `src/engine/codecs/` - Encode/decode logic

## What IS Editable
- Handler implementations (session/runtime)
- Custom message processing (runtime)
- Tests using generated messages

## Workflow
```
1. Update FIX spec XML
2. Run code generator
3. Rebuild project
4. Add handlers for new messages
5. Write tests
```

## Using Generated Messages
```cpp
#include <engine/messages/NewOrderSingle.h>

NewOrderSingle order;
order.ClOrdID = "123";
order.Symbol = "AAPL";
// Fields are type-safe, auto-generated
```

## Test Pattern
```cpp
TEST(EngineTest, CanCreateMessages) {
  NewOrderSingle order;
  EXPECT_TRUE(order.IsValid());
  auto encoded = codec.Encode(order);
  EXPECT_FALSE(encoded.empty());
}
```
