macro(add_test_custom test_name)
    set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR})

    file(GLOB_RECURSE TEST_SOURCES
        "${TEST_DIR}/*.c"
        "${TEST_DIR}/*.cpp"
    )

    add_executable(${test_name} ${TEST_SOURCES})

    if(NOT "${ARGN}" STREQUAL "")
        target_link_libraries(${test_name} PRIVATE GTest::gtest_main ${ARGN})
    else()
        target_link_libraries(${test_name} PRIVATE GTest::gtest_main)
    endif()

    include(GoogleTest)
    gtest_discover_tests(${test_name})
endmacro()
