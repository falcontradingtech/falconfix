macro(add_app_custom app_name)
    set(APP_DIR ${CMAKE_CURRENT_SOURCE_DIR})

    file(GLOB_RECURSE APP_SOURCES
        "${APP_DIR}/*.cpp"
        "${APP_DIR}/*.c"
    )

    message(STATUS "Adding app '${app_name}' with dependencies: ${ARGN}")

    add_executable(${app_name} ${APP_SOURCES})

    if(NOT "${ARGN}" STREQUAL "")
        target_link_libraries(${app_name} PRIVATE ${ARGN})
    endif()

    target_include_directories(${app_name} PUBLIC ${APP_DIR})

endmacro()
