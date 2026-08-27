# SPDX-License-Identifier: MIT
# Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
cmake_minimum_required(VERSION 3.16)

# ------------------------------------------------------------------------------
# cmake policy settings
# ------------------------------------------------------------------------------

if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()

# ------------------------------------------------------------------------------
# Configure vcpkg before project()
# ------------------------------------------------------------------------------

if(DEFINED ENV{VCPKG_ROOT})
    if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
        set(CMAKE_TOOLCHAIN_FILE
            "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
            CACHE STRING "vcpkg toolchain file")
    endif()
else()
    message(FATAL_ERROR "VCPKG_ROOT environment variable is not defined.")
endif()


# Use WIN32/UNIX/APPLE instead.
if(NOT DEFINED VCPKG_TARGET_TRIPLET)
    if(WIN32)
        set(VCPKG_TARGET_TRIPLET "x64-windows"
            CACHE STRING "vcpkg target triplet")
    elseif(APPLE)
        set(VCPKG_TARGET_TRIPLET "x64-osx"
            CACHE STRING "vcpkg target triplet")
    elseif(UNIX)
        set(VCPKG_TARGET_TRIPLET "x64-linux"
            CACHE STRING "vcpkg target triplet")
    endif()
endif()

# ------------------------------------------------------------------------------
# Project definition
# ------------------------------------------------------------------------------

project(FalconFix LANGUAGES CXX)

# ------------------------------------------------------------------------------
# C++ standard
# ------------------------------------------------------------------------------

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ------------------------------------------------------------------------------
# Diagnostics
# ------------------------------------------------------------------------------

message(STATUS "CMake generator       : ${CMAKE_GENERATOR}")
message(STATUS "Compiler              : ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "CMAKE_TOOLCHAIN_FILE  : ${CMAKE_TOOLCHAIN_FILE}")
message(STATUS "VCPKG_TARGET_TRIPLET  : ${VCPKG_TARGET_TRIPLET}")

# ------------------------------------------------------------------------------
# Build options
# ------------------------------------------------------------------------------

option(FALCONFIX_BUILD_TESTS "Build unit tests" ON)
option(FALCONFIX_BUILD_BENCHMARKS "Build benchmark targets" ON)

# ------------------------------------------------------------------------------
# Testing support
# ------------------------------------------------------------------------------

include(CTest)
enable_testing()

# ------------------------------------------------------------------------------
# Release optimizations
# ------------------------------------------------------------------------------

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(MSVC)
        add_compile_options(/O2 /DNDEBUG /utf-8)
        add_link_options(/OPT:REF /OPT:ICF)
    else()
        add_compile_options(-O3 -march=native -DNDEBUG)
        add_link_options(-flto)
    endif()
else()
    if(MSVC)
        add_compile_options(/utf-8)
    endif()
endif()

# ------------------------------------------------------------------------------
# Dependencies
# ------------------------------------------------------------------------------

find_package(GTest CONFIG REQUIRED)
find_package(benchmark CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(Boost REQUIRED)
find_package(OpenSSL REQUIRED)

include(CTest)
enable_testing()

# ------------------------------------------------------------------------------
# Project modules
# ------------------------------------------------------------------------------

include(cmake/AddApp.cmake)
include(cmake/AddLib.cmake)
include(cmake/AddTest.cmake)
include(cmake/AddBenchmark.cmake)
include(cmake/AddGeneratedProtocols.cmake)

# ------------------------------------------------------------------------------
# Include Dirs
# ------------------------------------------------------------------------------

include_directories(src)
include_directories(src/include)
include_directories(src/engine/include)
include_directories(tests/support)
include_directories(benchmark/support)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${OPENSSL_INCLUDE_DIR})

# ------------------------------------------------------------------------------
# Add projects
# ------------------------------------------------------------------------------

add_subdirectory(src)
add_subdirectory(apps)
add_subdirectory(tests)
add_subdirectory(benchmark)