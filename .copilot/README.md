# 🤖 FalconFIX AI Agent Configuration

This directory contains **centralized AI/Copilot instructions** for development on FalconFIX.

## Why This Directory?

Just like `.github/`, `.devcontainer/`, `.husky/` - this is the **standard convention** for AI development guidelines in professional projects.

## Structure

```
.copilot/
├── README.md                    ← You are here
├── instructions.md              ← Core rules (start here)
├── code-standards.md            ← C++20 style guide (compressed)
└── domains/                     ← Module-specific contexts
	├── session.md               ← Session management module
	├── connection.md            ← Connection handling
	├── runtime.md               ← Message processing
	├── testing.md               ← GTest framework rules
	├── cmake.md                 ← Build system rules
	├── headers.md               ← Public API conventions
	├── architecture.md          ← Design patterns reference
	└── engine.md                ← Generated protocol rules
```

## 🎯 How to Use

### With GitHub Copilot

**Option 1: Auto-load (VSCode)**
```json
// .vscode/settings.json automatically loads .copilot/instructions.md
// Copilot will use these instructions for ALL code in this workspace
```

**Option 2: Manual Load (any IDE)**
```
1. Open Copilot chat
2. Paste content of .copilot/instructions.md
3. Say: "Use these instructions for our entire session"
4. Copilot will follow them for all code generation
```

### With Claude / Other AI

```
1. Open AI chat
2. Upload/paste .copilot/instructions.md
3. Upload/paste relevant domain file from .copilot/domains/
4. Ask your question
5. AI will code following our standards
```

## 📋 Reading Order for Different Tasks

**Just getting started?**
1. `.copilot/instructions.md` (DO's and DON'Ts)
2. `.copilot/code-standards.md` (naming, style)
3. `.copilot/domains/session.md` (if working on sessions)

**Adding new module?**
1. `.copilot/instructions.md` (rules)
2. `.copilot/domains/cmake.md` (build setup)
3. `.copilot/domains/testing.md` (test structure)

**Debugging architecture issue?**
1. `.copilot/domains/architecture.md`
2. `.copilot/instructions.md` (principles)

**Working with generated protocol?**
1. `.copilot/domains/engine.md` (DON'T EDIT rules)
2. `.copilot/domains/testing.md` (protocol tests)

## 🔗 Links to Main Documentation

These files are **compressed references**. Full details at project root:

- **Full Architecture**: → `ARCHITECTURE.md`
- **Full Code Style**: → `CODE_STYLE.md`
- **Full Testing Guide**: → `TESTING_GUIDE.md`
- **Full Build Guide**: → `CMAKE_GUIDE.md`
- **Everything Else**: → `README_DEVELOPMENT.md`

## 💡 Token Efficiency

Each file in this directory is **optimized to minimize tokens** while keeping information complete:

- ✅ Essential rules (strict)
- ✅ Examples (just enough)
- ✅ References to full docs (when needed)
- ❌ Repeated explanations
- ❌ Padding or fluff

Goal: **200 tokens per session context → save 70% vs full docs**

## 🛡️ Absolute Rules (Non-Negotiable)

These apply **regardless of what's in domain files**:

```
DO:
✅ Write tests (80%+ coverage minimum)
✅ Use smart pointers
✅ Honor module boundaries
✅ Update .md docs with changes

DON'T:
❌ Edit src/engine/* (auto-generated)
❌ Modify CMakeLists.txt without discussion
❌ Use raw pointers
❌ Commit code without tests
```

## 🚀 For Repository Maintainers

Update these files when:
- Design patterns change
- New modules are added
- Testing requirements evolve
- CMake configuration shifts

**Workflow**:
1. Update relevant `.copilot/domains/*.md`
2. Create PR with explanation
3. Developers will use new rules immediately

## 📊 Coverage

| Domain | Covered By |
|--------|-----------|
| Session management | `domains/session.md` |
| Network/Connection | `domains/connection.md` |
| Message processing | `domains/runtime.md` |
| Unit testing | `domains/testing.md` |
| Build/CMake | `domains/cmake.md` |
| API design | `domains/headers.md` |
| System design | `domains/architecture.md` |
| Protocol/FIX | `domains/engine.md` |
| C++20 style | `code-standards.md` |
| Core principles | `instructions.md` |

## ✨ What Makes This Professional

- ✅ Centralized (like `.github/`)
- ✅ Versioned in git
- ✅ Reviewable in PRs
- ✅ Scalable (add domains as team grows)
- ✅ IDE-integrated (VSCode native)
- ✅ Compressed (minimal tokens)
- ✅ Documented (this README)
- ✅ Maintainable (clear update process)

## 🔄 Workflow

```
Developer Opens Copilot
	↓
Copilot loads instructions.md automatically (VSCode)
	↓
Dev works on specific module (e.g., session/)
	↓
Dev loads relevant domain file (domains/session.md) if needed
	↓
Copilot generates code following ALL rules
	↓
Code is tested, styled, documented automatically
```

## 📞 Questions?

If Copilot/AI asks clarification:
- First check `instructions.md`
- Then check relevant `domains/*.md`
- Finally check root `.md` files (ARCHITECTURE.md, etc.)

---

**Created**: 2026  
**Format**: Markdown  
**Language**: English  
**Maintenance**: Living document in git  
**Status**: Production-ready

Start with: **Read `instructions.md` now →**
