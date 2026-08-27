# SPDX-License-Identifier: MIT
# Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
add_lib_custom(falconfix_core STATIC spdlog::spdlog fmt::fmt)

# ------------------------------------------------------------------------------
# FalconFix generated protocols
# ------------------------------------------------------------------------------

falconfix_add_generated_protocols(falconfix_core src/engine/include/session/session_types.h)


if(FALCONFIX_BUILD_TESTS)
    target_compile_definitions(falconfix_core PUBLIC FALCONFIX_ENABLE_TEST_API=1)
    message(STATUS "Building with FALCONFIX_ENABLE_TEST_API enabled")
endif()
