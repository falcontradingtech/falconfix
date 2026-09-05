# Session Module

## Responsibility
Manage FIX session lifecycle, state transitions, and session-level messaging.

## Key Classes
- `Session`: Core session state machine
- `SessionManager`: Manages collection of sessions
- `SessionState`: Enum for session states

## State Machine
```
Idle → LoggingOn → Connected → LoggedOn → LoggingOut → Disconnected → Idle
```

## DO
✓ Handle state transitions atomically
✓ Emit session events on state changes
✓ Validate heartbeat intervals
✓ Use thread-safe session lookups
✓ Test all state transitions

## DON'T
✗ Create sessions without manager
✗ Modify session state from runtime directly
✗ Block on session operations
✗ Ignore connection failures
✗ Share session ownership

## Test Pattern
```cpp
TEST_F(SessionTest, TransitionsCorrectly) {
  auto session = manager->Create(sessionId);
  EXPECT_TRUE(session->LogOn(logonData));
  EXPECT_EQ(session->State(), SessionState::Connected);
}
```
