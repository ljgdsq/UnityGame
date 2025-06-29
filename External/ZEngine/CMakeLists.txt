cmake_minimum_required(VERSION 3.24)
project(zengine)

set(CMAKE_CXX_STANDARD 17)

# 设置编译选项，使用UTF-8编码
if(MSVC)
    add_compile_options(/utf-8)
endif()

# Set up only the minimal required include paths
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Add external dependencies as subdirectories
add_subdirectory(External/glm)
add_subdirectory(External/glfw)
add_subdirectory(External/rapidjson)
add_subdirectory(External/spdlog)

# 添加 GLAD
add_library(glad External/glad/src/gl.c )
target_include_directories(glad PUBLIC External/glad/include)
# 将 glad 放置到 glad 文件夹下
set_target_properties(glad PROPERTIES FOLDER "glad")

# 查找 OpenGL 库
find_package(OpenGL REQUIRED)

# Create an interface library for RapidJSON (since it's header-only)
add_library(rapidjson INTERFACE)
target_include_directories(rapidjson INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/External/rapidjson/include
)

# Collect all framework source files
file(GLOB_RECURSE FRAMEWORK_SOURCES
    "Framework/*.cpp"
)

# Collect all test scenes source files
file(GLOB_RECURSE TEST_SOURCES
    "TestScenes/*.cpp"
)

# Add executable with all source files
add_executable(zengine 
    ${FRAMEWORK_SOURCES}
    ${TEST_SOURCES}
    main.cpp
)

# Link libraries and set include directories via target properties
target_include_directories(zengine PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link all required libraries
target_link_libraries(zengine PRIVATE 
    glfw 
    glad 
    glm 
    ${OPENGL_LIBRARIES}
    rapidjson
    spdlog::spdlog
)

# Copy resource directory to output directory
add_custom_command(TARGET zengine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/Res
    $<TARGET_FILE_DIR:zengine>/Res
    COMMENT "Copying resource files to output directory"
)

