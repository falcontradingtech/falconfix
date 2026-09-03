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

    # NOTE: this macro is also used to build the aggregate "all_tests"
    # binary, whose source glob recursively picks up every test source
    # already compiled into the individual per-module test binaries
    # (e.g. session_tests, multiversion_tests, ...). Registering those
    # same tests with CTest a second time via gtest_discover_tests would
    # make the whole suite run twice per CI job, doubling CPU load and
    # causing timing-sensitive tests to flake/timeout under contention.
    if(NOT "${test_name}" STREQUAL "all_tests")
        include(GoogleTest)
        gtest_discover_tests(${test_name})
    endif()
endmacro()
