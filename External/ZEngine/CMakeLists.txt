cmake_minimum_required(VERSION 3.24)
project(zengine)

set(CMAKE_CXX_STANDARD 17)

# 设置编译选项，使用UTF-8编码
if(MSVC)
    add_compile_options(/utf-8)
endif()

# 针对 Clang 和 GCC 编译器禁用 RapidJSON 相关的警告
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    add_compile_options(-Wno-expansion-to-defined)
    add_compile_options(-Wno-deprecated-declarations)
    add_compile_options(-Wno-deprecated-copy-with-user-provided-copy)
    add_compile_options(-Wno-int-to-void-pointer-cast)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wno-expansion-to-defined)
    add_compile_options(-Wno-deprecated-declarations)
endif()

# 设置 RapidJSON 选项，禁用示例和测试
set(RAPIDJSON_BUILD_DOC OFF CACHE BOOL "Build RapidJSON documentation" FORCE)
set(RAPIDJSON_BUILD_EXAMPLES OFF CACHE BOOL "Build RapidJSON examples" FORCE)
set(RAPIDJSON_BUILD_TESTS OFF CACHE BOOL "Build RapidJSON tests" FORCE)
set(RAPIDJSON_BUILD_THIRDPARTY_GTEST OFF CACHE BOOL "Build RapidJSON third-party gtest" FORCE)

# Set up only the minimal required include paths
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/External/stb
)

# Add external dependencies as subdirectories
add_subdirectory(External/glm)
add_subdirectory(External/glfw)
add_subdirectory(External/rapidjson)
add_subdirectory(External/spdlog)
add_subdirectory(External/imgui)
add_subdirectory(External/ImGuiFileDialog)
add_subdirectory(External/fake)
# 添加 GLAD
add_library(glad External/glad/src/gl.c )
target_include_directories(glad PUBLIC External/glad/include)
# 将 glad 放置到 glad 文件夹下
set_target_properties(glad PROPERTIES FOLDER "glad")

# 确保 ImGuiFileDialog 能找到 imgui 头文件
target_link_libraries(ImGuiFileDialog PRIVATE imgui)

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

set_target_properties(fake PROPERTIES EXCLUDE_FROM_ALL TRUE)

# Link all required libraries
target_link_libraries(zengine PRIVATE
        glfw
        glad
        glm
        imgui
        fake
        ${OPENGL_LIBRARIES}
        rapidjson
        spdlog::spdlog
)

target_link_libraries(zengine PUBLIC
    ImGuiFileDialog
)

# Add stb include directory to target
target_include_directories(zengine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/External/stb
)

target_compile_definitions(zengine PRIVATE
    ZENGINE_EDITOR
)



# Copy resource directory to output directory
add_custom_command(TARGET zengine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/Res
    $<TARGET_FILE_DIR:zengine>/Res
    COMMENT "Copying resource files to output directory"
)

