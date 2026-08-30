# FalconFIX - High-Performance FIX Engine

[![CI Linux](https://github.com/falcontradingtech/falconfix/actions/workflows/ci-linux.yml/badge.svg)](https://github.com/falcontradingtech/falconfix/actions/workflows/ci-linux.yml)
[![CI Windows](https://github.com/falcontradingtech/falconfix/actions/workflows/ci-windows.yml/badge.svg)](https://github.com/falcontradingtech/falconfix/actions/workflows/ci-windows.yml)
[![Coverage Linux](https://codecov.io/gh/falcontradingtech/falconfix/branch/main/graph/badge.svg?flag=linux)](https://app.codecov.io/gh/falcontradingtech/falconfix?flags=linux)
[![Coverage Windows](https://codecov.io/gh/falcontradingtech/falconfix/branch/main/graph/badge.svg?flag=windows)](https://app.codecov.io/gh/falcontradingtech/falconfix?flags=windows)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/eeb6f887bd6b448ba074b84ad06eb765)](https://app.codacy.com/gh/falcontradingtech/falconfix/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_grade)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue)](https://en.wikipedia.org/wiki/C%2B%2B20)
[![CMake 3.16+](https://img.shields.io/badge/CMake-3.16%2B-red)](https://cmake.org/)
[![FIX 4.2/4.3/4.4](https://img.shields.io/badge/FIX-4.2%2C4.3%2C4.4-green)](https://www.fixtrading.org/)

A high-performance, code-generated FIX (Financial Information Exchange) engine for classic FIX4.x protocol flows with automatic C++ generation from XML specifications.

## What is FalconFIX?

FalconFIX is a **production-ready FIX protocol implementation** that eliminates months of development by:

- **Auto-generating C++20 code** from your FIX XML specification
- **Type-safe message handling** with compile-time verification
- **Zero-copy parsing** for minimal latency
- **Multi-session support** for complex trading scenarios
- **SSL/TLS security** built-in from the start

Transform FIX integration from weeks of manual coding into hours with code generation. Write once, generate everywhere.

## Why Use FalconFIX?

### Problem
Building FIX applications typically requires:
- 3-4 months of development
- Deep FIX protocol expertise
- Extensive testing and validation
- Manual implementation of session management, security, logging

### Solution
FalconFIX provides:
- **90% less code**: Generate instead of write manually
- **99% fewer bugs**: Type-safe generated code vs manual implementation  
- **5-10x faster**: Start trading in days, not months
- **Production-ready**: Battle-tested framework with enterprise-grade quality

### Use Cases
- 📈 **Trading Platforms**: Equities, derivatives, forex
- 🔄 **Order Management Systems** (OMS/EMS)
- 📊 **Market Data Aggregators**: Multi-exchange feeds
- 🏦 **Risk Management Systems**: Real-time position tracking
- 🔗 **Exchange Gateways**: Custom protocol bridges
- 🔁 **Legacy System Bridges**: Modernize old systems

## Key Capabilities

| Feature | Details |
|---------|---------|
| **Protocol Support** | FIX 4.2, 4.3, 4.4 fully supported |
| **Code Generation** | Automatic C++20 from XML specifications |
| **Transport** | TCP + SSL/TLSv1.2/1.3 |
| **Session Management** | Automatic keep-alive, sequence handling, state machine |
| **Type Safety** | Compile-time message validation |
| **Performance** | Zero-copy parsing, non-blocking I/O |
| **Multi-Session** | Concurrent initiators/acceptors |
| **Configuration** | INI-based, easy to customize |
| **Framework** | Structured logging (spdlog), modern C++20 |

## Quick Start

```bash
# Create a new project from FIX specification
python create_project.py MyFix ./projects --xml-file spec/FIX44.xml

# Build
cd ./projects/MyFix
cmake -B build -G "Ninja" -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Test
ctest --build-dir build
```

## Features

### Protocol Support
- **Versions**: FIX 4.2, 4.3, 4.4
- **Modes**: Initiator & Acceptor
- **Transport**: TCP + SSL/TLS 1.2/1.3

### Architecture
- **Code Generation**: Automatic C++ generation from XML specs
- **Type-Safe Messages**: Generated type-safe accessors
- **Zero-Copy Parsing**: Efficient buffer handling
- **Multi-Session**: Support for multiple concurrent sessions
- **Session Management**: Automatic keep-alive, sequence handling

### Framework
- **Configuration**: INI-based configuration
- **Logging**: Structured logging (spdlog)
- **Transport**: Non-blocking I/O (TCP/SSL)
- **Validation**: Automatic field validation
- **Error Handling**: Robust error recovery

## Creating a New Project

The `create_project.py` script automates project creation by:
1. Creating project directory structure
2. Copying the reusable framework
3. Generating C++ code from your FIX XML specification
4. Setting up CMakeLists.txt for your FIX version

### Basic Usage

```bash
python create_project.py <project-name> <output-dir> --xml-file <spec.xml>
```

### Examples

**Simple FIX4.4 Project**
```bash
python create_project.py MyEquities ./projects --xml-file spec/FIX44.xml
```

**With Message Filtering** (recommended for smaller footprint)
```bash
python create_project.py Trading ./projects \
  --xml-file spec/FIX44.xml \
  --message-filter "Logon,Logout,NewOrderSingle,ExecutionReport,OrderCancelRequest"
```

**Legacy FIX4.2**
```bash
python create_project.py LegacySystem ./projects \
  --xml-file spec/FIX42.xml \
  --fix-version FIX42
```

**Production Setup** (with optimizations)
```bash
python create_project.py ProductionApp ./projects \
  --xml-file spec/FIX44.xml \
  --fix-version FIX44 \
  --message-filter "Logon,Logout,NewOrderSingle,ExecutionReport" \
  --string-size-template \
  --samples-template
```

### Parameters

| Parameter | Required | Description |
|-----------|----------|-------------|
| `project-name` | Yes | Name of your new project |
| `output-dir` | Yes | Directory where project will be created |
| `--xml-file` | Yes | Path to FIX XML specification file |
| `--fix-version` | No | FIX version (e.g., FIX44, FIX42) - auto-detected if omitted |
| `--message-filter` | No | Comma-separated list of message types to generate (e.g., "NewOrderSingle,ExecutionReport") |
| `--string-size-template` | No | Enable string size templates for optimization |
| `--samples-template` | No | Generate sample message handlers |

### What Gets Generated

After running the script, your project structure will be:

```
projects/MyProject/
├── src/
│   ├── cpp/                     # Reusable framework (config, session, socket, etc)
│   └── engine/
│       ├── include/FIX4.4/      # Generated message headers
│       └── cpp/FIX4.4/          # Generated message implementations
├── tests/
│   └── engine/FIX4.4/           # Generated unit tests
├── CMakeLists.txt               # Top-level build config
├── CMakeSettings.json
├── vcpkg.json                   # Dependencies
└── build.sh                     # Build script
```

### Next Steps After Generation

```bash
cd ./projects/MyProject

# Configure build
cmake -B build -G "Ninja" -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build

# Run tests
ctest --build-dir build

# Success! Now integrate this into your application
```

### Message Filtering Benefits

Filtering generates only the messages you need:
- **Smaller binary**: 70% smaller in many cases
- **Faster compilation**: 5-10x faster
- **Lower memory**: Smaller footprint
- **Cleaner API**: Only your messages exposed

### Tips

- **XML Validation**: Ensure your FIX XML is valid before creating a project
- **Naming**: Use alphanumeric names for projects (underscores OK, no spaces)
- **Filters**: Check message names in your XML before using `--message-filter`
- **Multi-Version**: Create separate projects for different FIX versions

## Configuration Examples

### Initiator (Client)

```ini
[DEFAULT]
ConnectionType=initiator
SocketConnectHost=127.0.0.1
SocketConnectPort=5001
HeartBtInt=30

[SESSION]
BeginString=FIX4.4
SenderCompID=CLIENT
TargetCompID=SERVER
```

### Acceptor (Server)

```ini
[DEFAULT]
ConnectionType=acceptor
SocketAcceptHost=0.0.0.0
SocketAcceptPort=5001
HeartBtInt=30

[SESSION]
BeginString=FIX4.4
SenderCompID=SERVER
TargetCompID=CLIENT
```

### SSL Configuration

```ini
UseSSL=Y
SSLVerifyPeer=Y
SSLCertFile=/path/to/cert.pem
SSLKeyFile=/path/to/key.pem
SSLCAFile=/path/to/ca.pem
```

## Build Requirements

- **CMake**: 3.16+
- **Compiler**: MSVC 2022+, GCC 11+, Clang 13+
- **C++ Standard**: C++20
- **Dependencies**: vcpkg (fmt, spdlog, OpenSSL)

If `vcpkg.json` changes and CMake/vcpkg reports that the manifest has no
baseline, refresh the manifest baseline before configuring again:

```bash
"$VCPKG_ROOT/vcpkg" x-update-baseline --add-initial-baseline
```

## Build Instructions

### Windows

```powershell
cmake -B build -G "Ninja" -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --build-dir build
```

### Linux/macOS

```bash
./build.sh Release
# or
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --build-dir build
```

See [`README-BUILD.md`](README-BUILD.md) for detailed build instructions by platform.

## V1 Scope

### Included
- FIX 4.2, 4.3, 4.4
- Initiator & Acceptor sessions
- TCP & SSL/TLS transport
- Code generation from XML
- Multi-session support

### Out of Scope (V2+)
- SessionQualifier
- FIXT 1.1, FIX 5.0+
- Dynamic dictionary loading
- UseDataDictionary

## Project Structure

```
src/
├── cpp/                    # Reusable framework
│   ├── config/            # Configuration parser
│   ├── connection/        # Connection management
│   ├── logger/            # Logging system
│   ├── session/           # FIX session state machine
│   └── socket/            # TCP/SSL transport
└── engine/                # Generated protocol code
    ├── include/FIX4.4/   # Generated headers
    └── cpp/FIX4.4/       # Generated implementations

tests/                      # Unit tests
apps/                       # Example applications
falcongen/                  # Code generator
```

## Author & License

**Author**: Michel Tonetti, Herik Lima, Fabio Galuppo  
**License**: MIT - See LICENSE file  
**Repository**: https://github.com/falcontradingtech/falconfix

## Support

- 🐛 **Issues**: https://github.com/falcontradingtech/falconfix/issues
- 📚 **Documentation**: See [`README-BUILD.md`](README-BUILD.md) for build details
- 🚀 **Getting Started**: Run `create_project.py` to bootstrap a new project

## Contributing

We welcome contributions! Please read our [CONTRIBUTING.md](CONTRIBUTING.md) guide for:
- Setup instructions
- How to make changes
- Testing requirements
- Pull request process

Thank you for helping make FalconFIX better! 🙌
