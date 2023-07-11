cmake_minimum_required(VERSION 3.24)
project(tinyraycaster)

set(CMAKE_CXX_STANDARD 17)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
add_subdirectory(external/SDL2)

file(GLOB SOURCES
        "${SRC_DIR}/*.h"
        "${SRC_DIR}/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} SDL2-static)
target_include_directories(${PROJECT_NAME} PRIVATE "${SRC_DIR}")
