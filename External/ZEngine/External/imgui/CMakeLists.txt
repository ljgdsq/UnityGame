project(imgui)

file(GLOB IMGUI_SOURCES
        imgui.cpp
        imgui_demo.cpp
        imgui_draw.cpp
        imgui_tables.cpp
        imgui_widgets.cpp

        backends/imgui_impl_glfw.cpp
        backends/imgui_impl_opengl3.cpp
        )

add_library(imgui STATIC ${IMGUI_SOURCES})

# 设置include目录
target_include_directories(imgui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/backends
)

# 查找OpenGL
find_package(OpenGL REQUIRED)

# 链接OpenGL
target_link_libraries(imgui PUBLIC
    OpenGL::GL
)

# 如果项目中已有glfw，链接到glfw
if(TARGET glfw)
    target_link_libraries(imgui PUBLIC glfw)
endif()
