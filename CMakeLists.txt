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
set(WLD_LIB_TARGET wld_lib)
set(MAIN_EXE_TARGET main_exe)

if (WIN32)
  SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a;.dll.a")
  SET(BUILD_SHARED_LIBS OFF)
  SET(CMAKE_EXE_LINKER_FLAGS "-static")
endif()

# compiler flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -DGLM_FORCE_SWIZZLE") # prevents a console window from being spawned

if(WIN32)
  set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -mwindows") # prevents a console window from being spawned
endif()

# output paths
set(OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

# mod lib
add_library(${MOD_LIB_TARGET} "")
add_library(kekw::mod ALIAS ${MOD_LIB_TARGET})
set_target_properties(${MOD_LIB_TARGET} PROPERTIES OUTPUT_NAME "kekwmod")
target_link_libraries(${MOD_LIB_TARGET} spdlog::spdlog)
target_link_libraries(${MOD_LIB_TARGET} lua::lua)

# wld lib
add_library(${WLD_LIB_TARGET} "")
add_library(kekw::wld ALIAS ${WLD_LIB_TARGET})
set_target_properties(${WLD_LIB_TARGET} PROPERTIES OUTPUT_NAME "kekwwld")
target_link_libraries(${WLD_LIB_TARGET} spdlog::spdlog)

# ux lib
add_library(${UX_LIB_TARGET} "")
add_library(kekw::ux ALIAS ${UX_LIB_TARGET})
set_target_properties(${UX_LIB_TARGET} PROPERTIES OUTPUT_NAME "kekwux")
target_link_libraries(${UX_LIB_TARGET} glfw)
target_link_libraries(${UX_LIB_TARGET} vivid::vivid)
target_link_libraries(${UX_LIB_TARGET} tweeny)
target_link_libraries(${UX_LIB_TARGET} kekw::mod)
target_link_libraries(${UX_LIB_TARGET} kekw::wld)

# main executable
add_executable(${MAIN_EXE_TARGET} "")
add_executable(kekw::kekw ALIAS ${MAIN_EXE_TARGET})
set_target_properties(${MAIN_EXE_TARGET} PROPERTIES OUTPUT_NAME "kekw")
target_link_libraries(${MAIN_EXE_TARGET} kekw::ux)

# local input paths
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/inc)
include_directories(${PROJECT_SOURCE_DIR}/extern/glfw/include)
include_directories(${PROJECT_SOURCE_DIR}/extern)
include_directories(${PROJECT_SOURCE_DIR}/extern/imgui)
include_directories(${PROJECT_SOURCE_DIR}/extern/glm)
include_directories(${PROJECT_SOURCE_DIR}/extern/vivid/include)
include_directories(${PROJECT_SOURCE_DIR}/extern/lua/include)

# external input paths - need converting to submodules

#include_directories($ENV{CXX_INCLUDE_DIR})
include_directories(C:/dev/cpplibs/usr/include)

# utility macros
macro(mirror_directory desc src_dir dest_dir)
  set (remove_target kekw_removedir_${desc})
  add_custom_target(
      ${remove_target} ALL
      COMMAND ${CMAKE_COMMAND} -E remove_directory ${dest_dir})

  add_custom_target(
      kekw_copydir_${desc} ALL
      COMMAND ${CMAKE_COMMAND} -E copy_directory ${src_dir} ${dest_dir}
      DEPENDS ${remove_target})
endmacro(mirror_directory)

# look for more CMake files.
add_subdirectory(extern)
add_subdirectory(src)
add_subdirectory(inc)

enable_testing()
include(GoogleTest)
add_subdirectory(tests)