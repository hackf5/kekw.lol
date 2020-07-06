# glfw
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
set(GLFW_VULKAN_STATIC OFF)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/glfw")

# spdlog
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/spdlog")

# glm doesn't need building
# add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/glm")

set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/json")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vivid")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/tweeny")

# dear imgui
target_sources(${UX_LIB_TARGET}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imstb_truetype.h
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imconfig.h
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.h
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_demo.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_draw.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/examples/imgui_impl_glfw.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/examples/imgui_impl_glfw.h
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/examples/imgui_impl_opengl3.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/examples/imgui_impl_opengl3.h
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_internal.h
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui_widgets.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imstb_rectpack.h
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui/imstb_textedit.h
)