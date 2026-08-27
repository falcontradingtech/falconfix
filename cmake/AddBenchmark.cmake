macro(add_benchmark_custom benchmark_name)
    set(BENCH_DIR ${CMAKE_CURRENT_SOURCE_DIR})

    file(GLOB_RECURSE BENCH_SOURCES
        "${BENCH_DIR}/*.c"
        "${BENCH_DIR}/*.cpp"
    )

    add_executable(${benchmark_name} ${BENCH_SOURCES})


    if(NOT "${ARGN}" STREQUAL "")
        target_link_libraries(${benchmark_name} PRIVATE
            benchmark::benchmark
            benchmark::benchmark_main
            ${ARGN})
    else()
        target_link_libraries(${benchmark_name} PRIVATE
            benchmark::benchmark
            benchmark::benchmark_main)
    endif()

    target_compile_definitions(${benchmark_name} PRIVATE
        FALCONFIX_ENABLE_FIX44
        FALCONFIX_ENABLE_CODEC_ERROR_PATHS=1
    )
endmacro()
