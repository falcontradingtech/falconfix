# Connection Module

## Responsibility
Handle TCP/TLS connections, reads, writes, and low-level network I/O.

## Key Classes
- `Connection`: Manages single TCP/TLS connection
- `ConnectionPool`: Manages multiple connections
- `ConnectionError`: Network error handling

## DO
✓ Use async I/O for reads/writes
✓ Handle disconnections gracefully
✓ Buffer incomplete messages
✓ Implement connection timeouts
✓ Track connection state separately from session
✓ Test network failures

## DON'T
✗ Block on network I/O
✗ Share raw sockets between threads
✗ Ignore partial reads/writes
✗ Mix connection logic with session logic
✗ Assume connection == authenticated

## Lifecycle
```
Created → Connecting → Connected → ReadWrite → Closed
       └─ Failed
```

## Test Pattern
```cpp
TEST_F(ConnectionTest, HandlesDisconnect) {
  conn->Connect();
  EXPECT_TRUE(conn->IsConnected());
  SimulateDisconnect();
  EXPECT_FALSE(conn->IsConnected());
}
```
