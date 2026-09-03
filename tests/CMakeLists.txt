# SPDX-License-Identifier: MIT
# Copyright (c) 2026 Michel Tonetti, Herik Lima, and Fabio Galuppo
enable_testing()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(app)
add_subdirectory(config)
add_subdirectory(connection)
add_subdirectory(engine/FIX4.2)
add_subdirectory(engine/FIX4.3)
add_subdirectory(engine/FIX4.4)
add_subdirectory(logger)
add_subdirectory(multiversion)
add_subdirectory(session)
add_subdirectory(socket)
add_subdirectory(store)

add_test_custom("all_tests" falconfix_connection falconfix_logger socket falconfix_config falconfix_session falconfix_core)

add_custom_command(TARGET all_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/tests/socket/certs/server.crt
        $<TARGET_FILE_DIR:all_tests>

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/tests/socket/certs/server.key
        $<TARGET_FILE_DIR:all_tests>
)
