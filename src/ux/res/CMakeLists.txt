target_sources(
    ${UX_LIB_TARGET}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/app.rc)

add_custom_target(
    copy-dir-56ec ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/img 
            ${OUTPUT_DIRECTORY}/res/img)

add_dependencies(${UX_LIB_TARGET} copy-dir-56ec)