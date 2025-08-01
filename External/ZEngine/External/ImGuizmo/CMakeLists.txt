cmake_minimum_required(VERSION 3.10)
project(ImGuizmo)

set(IMGUIZMO_SRC
    ImGuizmo.h
    ImGuizmo.cpp
)

add_library(ImGuizmo STATIC ${IMGUIZMO_SRC})

target_include_directories(ImGuizmo PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    $<TARGET_PROPERTY:imgui,INTERFACE_INCLUDE_DIRECTORIES>
)

target_link_libraries(ImGuizmo PUBLIC imgui)

set_target_properties(ImGuizmo PROPERTIES FOLDER "ImGuizmo")
