target_sources(${UX_LIB_TARGET}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/window-manager.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/gl-window-layer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ux-window-layer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/recruit-window-layer.cpp
)

add_subdirectory(widgets)