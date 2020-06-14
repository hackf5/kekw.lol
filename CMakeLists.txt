cmake_minimum_required(VERSION 3.0.0)

# project
project(kekw 
    VERSION 0.1.0
    DESCRIPTION "An example project with CMake")
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

#outputs
set(UX_LIB_TARGET ux_lib)
set(MOD_LIB_TARGET mod_lib)
set(MAIN_EXE_TARGET main_exe)

SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a;.dll.a")
SET(BUILD_SHARED_LIBS OFF)
SET(CMAKE_EXE_LINKER_FLAGS "-static")

# compiler flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -mwindows -DGLM_FORCE_SWIZZLE") # prevents a console window from being spawned

# output paths
set(OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

# local input paths
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/inc)
include_directories(${PROJECT_SOURCE_DIR}/extern/glfw/include)

# external input paths - need converting to submodules
include_directories(C:/dev/cpplibs/usr/include)

# glfw
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
set(GLFW_VULKAN_STATIC OFF)
add_subdirectory("${PROJECT_SOURCE_DIR}/extern/glfw" "extern/glfw")

# spdlog
add_subdirectory("${PROJECT_SOURCE_DIR}/extern/spdlog" "extern/spdlog")

# mod lib
add_library(${MOD_LIB_TARGET} "")
set_target_properties(${MOD_LIB_TARGET} PROPERTIES OUTPUT_NAME "kekwmod")
target_link_libraries(${MOD_LIB_TARGET} spdlog::spdlog)

# ux lib
add_library(${UX_LIB_TARGET} "")
set_target_properties(${UX_LIB_TARGET} PROPERTIES OUTPUT_NAME "kekwux")
target_link_libraries(${UX_LIB_TARGET} glfw)
target_link_libraries(${UX_LIB_TARGET} spdlog::spdlog)
target_link_libraries(${UX_LIB_TARGET} ${MOD_LIB_TARGET})


# main executable
add_executable(${MAIN_EXE_TARGET} "")
set_target_properties(${MAIN_EXE_TARGET} PROPERTIES OUTPUT_NAME "kekw")
target_link_libraries(${MAIN_EXE_TARGET} ${UX_LIB_TARGET})
target_link_libraries(${MAIN_EXE_TARGET} ${MOD_LIB_TARGET})

# look for more CMake files.
add_subdirectory(src)
add_subdirectory(inc)

enable_testing()
include(GoogleTest)
add_subdirectory(tests)