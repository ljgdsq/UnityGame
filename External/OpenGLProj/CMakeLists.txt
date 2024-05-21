cmake_minimum_required(VERSION 3.00)
project(OpenGLProj)

set(CMAKE_CXX_STANDARD 17)
include_directories(${PROJECT_SOURCE_DIR}/include)

# 添加 GLFW
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE BOOL "Generate installation target")

# 添加 GLAD
add_library(glad external/glad/src/gl.c)
target_include_directories(glad PUBLIC external/glad/include)
# 将 glad 放置到 glad 文件夹下
set_target_properties(glad PROPERTIES FOLDER "glad")
# 查找 OpenGL 库
find_package(OpenGL REQUIRED)

# 添加源文件
file(GLOB SOURCES "src/*.cpp")

add_executable(OpenGLProj ${SOURCES})

add_subdirectory(external/glfw)

add_subdirectory(external/lua)

target_link_libraries(OpenGLProj PUBLIC glfw glad liblua ${OPENGL_LIBRARIES})



# 设置包含目录
target_include_directories(OpenGLProj PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}/include"
        "external/glad/include"
        "external/glfw/include"
        "external/lua/src"
        )
# 生成包含资源路径的头文件
configure_file(res_path.h.in ${PROJECT_BINARY_DIR}/res_path.h @ONLY)

# 复制资源文件到构建目录
add_custom_command(TARGET OpenGLProj POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:OpenGLProj>/res
        )

