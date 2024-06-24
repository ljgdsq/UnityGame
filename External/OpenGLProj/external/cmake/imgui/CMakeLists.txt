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