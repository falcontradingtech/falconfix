---
description: Add a Google Benchmark for a component
agent: agent
---
Benchmark ${input:target:class/operation}.

1. `benchmark/${input:module:module}/` — `<name>_benchmark.cpp`, include `<benchmark/benchmark.h>`, SPDX header, reuse `benchmark/support/` harness.
2. `CMakeLists.txt`: `add_benchmark_custom(${input:module}_benchmark falconfix_${input:module})`; `add_subdirectory` in `benchmark/CMakeLists.txt`.
3. Measure steady-state only (setup outside the `for (auto _ : state)` loop); no I/O to real network unless using the existing socket harness.
4. Build release preset and run the benchmark target; report ns/op.
