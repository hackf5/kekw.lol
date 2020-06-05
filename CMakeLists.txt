cmake_minimum_required(VERSION 3.0.0)

# project
project(kekw 
    VERSION 0.1.0
    DESCRIPTION "An example project with CMake")
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a;.dll.a")
SET(BUILD_SHARED_LIBS OFF)
SET(CMAKE_EXE_LINKER_FLAGS "-static")

# compiler flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -mwindows") # prevents a console window from being spawned

# output paths
set(OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

# local input paths
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/inc)

# external input paths - need to add to variables
include_directories(C:/dev/cpplibs/usr/include)
link_directories(C:/dev/cpplibs/usr/lib)

# main executable
add_executable(${CMAKE_PROJECT_NAME} "")
add_compile_definitions(STB_IMAGE_IMPLEMENTATION)

# add external libraries
find_package(glfw3 3.3 REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} glfw)

find_package(spdlog REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} spdlog::spdlog)

find_library(FRUIT_LIBRARY REQUIRED NAMES fruit libfruit)
target_link_libraries(${CMAKE_PROJECT_NAME} ${FRUIT_LIBRARY})

# look for more CMake files.
add_subdirectory(src)
add_subdirectory(inc)