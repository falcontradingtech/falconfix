This directory stores session-heavy workload references that are intentionally
not part of the unit test suite.

`session_benchmark_flow_legacy_gtest.cpp` was moved out of `tests/session`
because its 100k incremental burst cases are highly sensitive to machine load
and can fail while the host is busy compiling, even when the transport/session
logic is correct.

It is kept here as reference material for future benchmark-oriented rewrites.
