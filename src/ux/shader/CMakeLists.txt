target_sources(${UX_LIB_TARGET}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/shader.cpp
)

mirror_directory(
    ux_shaders
    ${CMAKE_CURRENT_SOURCE_DIR}/impl
    ${OUTPUT_DIRECTORY}/glsl)