---
description: Change generated FIX code via falcongen and regenerate
agent: agent
---
Goal: ${input:change:what must change in generated engine code}.

Never edit `src/engine/**` or `tests/engine/**`.
1. Find the template in `falcongen/code/generator_*.py` (or `falcongen/config/*.yaml`) that emits the affected code.
2. Apply the minimal change; keep output deterministic.
3. Run `python falcongen/falcon-gen.py` (background), then build preset `windows-debug` and `ctest -R engine`.
4. Summarize: generator file(s) changed, count of regenerated files, test result.
