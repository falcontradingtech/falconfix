# SPDX-License-Identifier: MIT
# Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
add_test_custom("socket_tests" socket falconfix_config falconfix_session falconfix_core)

add_custom_command(TARGET socket_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/tests/socket/certs/server.crt
        $<TARGET_FILE_DIR:socket_tests>

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/tests/socket/certs/server.key
        $<TARGET_FILE_DIR:socket_tests>
)