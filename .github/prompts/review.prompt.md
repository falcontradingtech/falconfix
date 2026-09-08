---
description: Review a diff against FalconFIX rules
agent: ask
---
Review ${input:scope:staged changes / file / PR} against `.github/copilot-instructions.md` and the matching `.github/instructions/*.md`.

Check only: edits under `src/engine/`; raw `new`/`delete`/owning pointers; blocking I/O in connection/session; missing tests (happy/error/edge); naming (`camelCase` methods, `m_` members); SPDX header; CMake macro use; layer violations (`connection` → `session`).
Output a table `file:line | rule | fix` — findings only, no praise, no summary of what the code does.
