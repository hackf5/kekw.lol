target_sources(${UX_LIB_TARGET}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/debug-overlay-widget.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/camera-widget.hpp
)

add_subdirectory(imgui)