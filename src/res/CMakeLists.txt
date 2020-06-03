target_sources(
    ${CMAKE_PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/app.rc)

add_custom_target(
    copy-files ALL
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/app-icon.png 
            ${CMAKE_BINARY_DIR}/app-icon.png)