target_sources(${CMAKE_PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/shader.cpp
)

add_custom_target(
    copy-files-51a6 ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/impl
            ${OUTPUT_DIRECTORY}/glsl)

add_dependencies(${CMAKE_PROJECT_NAME} copy-files-51a6)