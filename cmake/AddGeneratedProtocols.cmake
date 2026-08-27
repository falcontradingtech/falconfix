function(falconfix_add_generated_protocols target_name)
    set(FALCONFIX_GENERATED_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/src/engine/include")
    set(FALCONFIX_GENERATED_CPP_DIR "${CMAKE_SOURCE_DIR}/src/engine/cpp")

    target_include_directories(${target_name}
        INTERFACE
            "${FALCONFIX_GENERATED_INCLUDE_DIR}"
    )

    if (EXISTS "${FALCONFIX_GENERATED_INCLUDE_DIR}/FIX4.0")
        target_compile_definitions(${target_name} INTERFACE FALCONFIX_ENABLE_FIX40)
    endif ()

    if (EXISTS "${FALCONFIX_GENERATED_INCLUDE_DIR}/FIX4.1")
        target_compile_definitions(${target_name} INTERFACE FALCONFIX_ENABLE_FIX41)
    endif ()

    if (EXISTS "${FALCONFIX_GENERATED_INCLUDE_DIR}/FIX4.2")
        target_compile_definitions(${target_name} INTERFACE FALCONFIX_ENABLE_FIX42)
    endif ()

    if (EXISTS "${FALCONFIX_GENERATED_INCLUDE_DIR}/FIX4.3")
        target_compile_definitions(${target_name} INTERFACE FALCONFIX_ENABLE_FIX43)
    endif ()

    if (EXISTS "${FALCONFIX_GENERATED_INCLUDE_DIR}/FIX4.4")
        target_compile_definitions(${target_name} INTERFACE FALCONFIX_ENABLE_FIX44)
    endif ()

    if (EXISTS "${FALCONFIX_GENERATED_INCLUDE_DIR}/FIX5.0")
        target_compile_definitions(${target_name} INTERFACE FALCONFIX_ENABLE_FIX50)
    endif ()

    if (EXISTS "${FALCONFIX_GENERATED_INCLUDE_DIR}/FIX5.0SP1")
        target_compile_definitions(${target_name} INTERFACE FALCONFIX_ENABLE_FIX50SP1)
    endif ()

    if (EXISTS "${FALCONFIX_GENERATED_INCLUDE_DIR}/FIX5.0SP2")
        target_compile_definitions(${target_name} INTERFACE FALCONFIX_ENABLE_FIX50SP2)
    endif ()

    if (EXISTS "${FALCONFIX_GENERATED_INCLUDE_DIR}/FIXT1.1")
        target_compile_definitions(${target_name} INTERFACE FALCONFIX_ENABLE_FIXT11)
    endif ()

    if (EXISTS "${FALCONFIX_GENERATED_CPP_DIR}")
        file(GLOB_RECURSE FALCONFIX_GENERATED_SOURCES CONFIGURE_DEPENDS
            "${FALCONFIX_GENERATED_CPP_DIR}/*.cpp"
        )

        if (FALCONFIX_GENERATED_SOURCES)
            if (NOT TARGET falconfix_generated_protocols)
                add_library(falconfix_generated_protocols STATIC ${FALCONFIX_GENERATED_SOURCES})

                target_include_directories(falconfix_generated_protocols
                    PUBLIC
                        "${FALCONFIX_GENERATED_INCLUDE_DIR}"
                        "${CMAKE_SOURCE_DIR}/src/include"
                        "${CMAKE_SOURCE_DIR}/src"
                )

                target_link_libraries(falconfix_generated_protocols
                    PUBLIC
                        spdlog::spdlog
                        fmt::fmt
                )
            endif ()

            target_link_libraries(${target_name}
                INTERFACE
                    falconfix_generated_protocols
            )
        endif ()
    endif ()
endfunction()
