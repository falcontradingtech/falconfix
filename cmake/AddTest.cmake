macro(add_test_custom test_name)
    set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR})

    file(GLOB_RECURSE TEST_SOURCES
        "${TEST_DIR}/*.c"
        "${TEST_DIR}/*.cpp"
    )

    # Add the main.cpp from tests directory if it exists
    set(TEST_MAIN_FILE "${CMAKE_SOURCE_DIR}/tests/main.cpp")
    if(EXISTS "${TEST_MAIN_FILE}")
        list(APPEND TEST_SOURCES "${TEST_MAIN_FILE}")
    endif()

    add_executable(${test_name} ${TEST_SOURCES})

    if(NOT "${ARGN}" STREQUAL "")
        target_link_libraries(${test_name} PRIVATE GTest::gtest ${ARGN})
    else()
        target_link_libraries(${test_name} PRIVATE GTest::gtest)
    endif()

    include(GoogleTest)
    gtest_discover_tests(${test_name})
endmacro()
