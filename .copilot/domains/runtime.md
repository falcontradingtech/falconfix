# Runtime Module

## Responsibility
Message dispatch, routing, and execution of FIX message handlers.

## Key Classes
- `MessageRouter`: Routes incoming messages to handlers
- `Handler<T>`: Template-based message handler
- `Dispatcher`: Executes handlers

## Message Flow
```
Connection → Decode → Router → Handler → Session State Update
```

## DO
✓ Route messages by MsgType
✓ Execute handlers asynchronously
✓ Validate message format before routing
✓ Log routing decisions
✓ Handle unknown message types gracefully
✓ Test with generated protocol messages

## DON'T
✗ Block on handler execution
✗ Route messages to wrong session
✗ Modify session state directly in handler
✗ Assume message is valid (validate first)
✗ Mix routing with business logic

## Handler Registration
```cpp
router->Register<NewOrderSingle>([](const NewOrderSingle& msg) {
  // Handle order
});
```

## Test Pattern
```cpp
TEST_F(RuntimeTest, RoutesCorrectly) {
  NewOrderSingle order = CreateTestOrder();
  router->Route(order);
  EXPECT_CALL(handler, OnNewOrder(_));
}
```
