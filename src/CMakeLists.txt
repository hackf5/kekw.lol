target_sources("${MAIN_EXE_TARGET}"
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

add_subdirectory(mod)
add_subdirectory(res)
add_subdirectory(util)
add_subdirectory(ux)