target_sources(
    ${UX_LIB_TARGET}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/app.rc)

mirror_directory(
    ux_resource_images
    ${CMAKE_CURRENT_SOURCE_DIR}/img 
    ${OUTPUT_DIRECTORY}/res/img)