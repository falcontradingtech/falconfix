---
description: Diagnose and fix a failing build or test with minimal changes
agent: agent
---
Fix: ${input:error:paste the first error line or failing test name}.

1. Read only the first error and the file/line it points to; don't scan the tree.
2. Root-cause fix only (no suppressions, no `#pragma warning`, no test deletion).
3. Rebuild the single target: `cmake --build --preset windows-debug --target <target>` (background, poll).
4. Re-run the failing test with `ctest -R <name> --output-on-failure`.
Reply with: cause (1 line), diff summary, result.
