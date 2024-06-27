cmake_minimum_required(VERSION 3.00)
project(OpenGLProj)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_CXX_STANDARD 17)
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_BINARY_DIR})
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/external/glad/include)
include_directories(${PROJECT_SOURCE_DIR}/external/glfw/include)
include_directories(${PROJECT_SOURCE_DIR}/external/assimp/include)
include_directories(${PROJECT_SOURCE_DIR}/external/imgui)
include_directories(${PROJECT_SOURCE_DIR}/external/imgui/backends)
include_directories(${PROJECT_SOURCE_DIR}/external/imgui/misc)
include_directories(${PROJECT_SOURCE_DIR}/external/lua/src)
include_directories(${PROJECT_SOURCE_DIR}/external/ImGuiFileDialog)
include_directories(${PROJECT_SOURCE_DIR}/external/rapidjson/include)
include_directories(${PROJECT_SOURCE_DIR}/external/spdlog/include)



#option(RAPIDJSON_BUILD_DOC "Build rapidjson documentation." OFF)
#option(RAPIDJSON_BUILD_EXAMPLES "Build rapidjson examples." OFF)
#option(RAPIDJSON_BUILD_TESTS "Build rapidjson perftests and unittests." OFF)
set(RAPIDJSON_BUILD_EXAMPLES OFF CACHE BOOL "Build rapidjson examples.")
set(RAPIDJSON_BUILD_DOC OFF CACHE BOOL "Build rapidjson examples.")
set(RAPIDJSON_BUILD_TESTS OFF CACHE BOOL "Build rapidjson examples.")
# 添加 GLFW
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE BOOL "Generate installation target")

# 添加 GLAD
add_library(glad external/glad/src/gl.c )
target_include_directories(glad PUBLIC external/glad/include)
# 将 glad 放置到 glad 文件夹下
set_target_properties(glad PROPERTIES FOLDER "glad")
# 查找 OpenGL 库
find_package(OpenGL REQUIRED)

# 添加源文件
file(GLOB_RECURSE  SOURCES "src/*.cpp" )
# 排除 src/main.cpp 文件
list(REMOVE_ITEM SOURCES "${PROJECT_SOURCE_DIR}/src/main.cpp")

set(CORE_SRC ${SOURCES})
add_executable(OpenGLProj ${SOURCES} src/main.cpp src/core/GameObject.h src/core/Component.h src/core/Transform.h)
add_subdirectory(external/glfw)


if(WIN32)
set(LUA_BUILD_AS_DLL ON CACHE BOOL "Build Lua library as Dll")
endif()


add_subdirectory(external/lua)
add_subdirectory(external/glm)

target_link_libraries(OpenGLProj PUBLIC glfw glad liblua  glm ${OPENGL_LIBRARIES})


#if (LUA_BUILD_AS_DLL)
#    add_custom_command(TARGET OpenGLProj POST_BUILD
#            COMMAND ${CMAKE_COMMAND} -E copy
#            $<TARGET_FILE:liblua>
#            $<TARGET_FILE_DIR:OpenGLProj>)
#endif ()
# 创建一个自定义目标来执行拷贝操作
if (LUA_BUILD_AS_DLL)
    add_custom_target(copy_liblua ALL
            COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_FILE:liblua>
            ${CMAKE_CURRENT_BINARY_DIR}
            DEPENDS liblua
            COMMENT "Copying lua.dll to output directory"
            )
endif()
# 设置包含目录
target_include_directories(OpenGLProj PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_BINARY_DIR}/src"
        "${PROJECT_SOURCE_DIR}/include"
        "external/glad/include"
        "external/glfw/include"
        "external/lua/src"
        )

# 生成包含资源路径的头文件
configure_file(res_path.h.in ${PROJECT_BINARY_DIR}/res_path.h @ONLY)

## 复制资源文件到构建目录
#add_custom_command(TARGET OpenGLProj POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy_directory
#        ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:OpenGLProj>/res
#        )

# 定义一个全局变量，包含所有资源文件
file(GLOB_RECURSE RES_FILES "${CMAKE_SOURCE_DIR}/res/*")

# 创建一个自定义命令来复制资源文件
add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/res
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res ${CMAKE_BINARY_DIR}/res
        DEPENDS ${RES_FILES}  # 使该命令依赖于所有资源文件
        COMMENT "Copying resources..."
)

add_executable(aaa_fake "src/main.cpp" src/core/Common.h src/core/component/ImageComponent.h src/core/Canvas.h)

# 创建一个自定义目标，执行资源复制命令
add_custom_target(copy_res ALL DEPENDS ${CMAKE_BINARY_DIR}/res)

# 使 OpenGLProj 依赖于 copy_res 目标
# 这样在构建 OpenGLProj 之前，资源文件会被复制
add_dependencies(OpenGLProj copy_res)


#add_subdirectory(examples/light)
#add_subdirectory(examples/light5)
add_subdirectory(examples/spotlight)
add_subdirectory(examples/multilight)


#config assimp
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build package with shared libraries")
set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE BOOL "Inject debug postfix in .a/.so/.dll lib names")
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "Treat all warnings as errors.")
set(ASSIMP_INSTALL OFF CACHE BOOL "Disable this if you want to use assimp as a submodule.")
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "If the test suite for Assimp is built in addition to the library.")


#if (WIN32)
#    set(USE_STATIC_CRT ON CACHE BOOL "Link against the static runtime libraries.")
#endif ()

add_subdirectory(external/assimp)
add_subdirectory(external/spdlog)
add_subdirectory(external/imgui)
add_subdirectory(external/ImGuiFileDialog)
add_subdirectory(external/rapidjson)

#add_subdirectory(examples/assimp)
#add_subdirectory(examples/advance)

add_subdirectory(examples/imgui)
#add_subdirectory(examples/josntest)
add_subdirectory(examples/logtest)


