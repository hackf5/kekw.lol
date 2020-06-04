target_sources(
    ${CMAKE_PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/app.rc)

add_custom_target(
    copy-files-56ec ALL
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/app-icon.png 
            ${OUTPUT_DIRECTORY}/app-icon.png)

add_dependencies(${CMAKE_PROJECT_NAME} copy-files-56ec)