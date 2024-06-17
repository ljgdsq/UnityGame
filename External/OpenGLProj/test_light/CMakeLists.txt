
# 查找项目中的头文件
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/external/glad/include)
include_directories(${PROJECT_SOURCE_DIR}/external/glfw/include)
include_directories(${PROJECT_SOURCE_DIR}/external/lua/src)
include_directories(${PROJECT_SOURCE_DIR}/external/glm)


# 包含其他目录中的所有文件
file(GLOB_RECURSE OTHER_SOURCES
        #        "${PROJECT_SOURCE_DIR}/src/lua/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/core/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/utils/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/ui/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/platform/*.cpp"
        )

# 添加测试源文件
file(GLOB TEST_SOURCES OTHER_SOURCES "main.cpp")

# 合并所有源文件
set(SOURCES ${TEST_SOURCES} ${OTHER_SOURCES} )
# 创建可执行文件
add_executable(TestLight ${SOURCES})

# 链接库
target_link_libraries(TestLight PUBLIC glfw glad liblua glm ${OPENGL_LIBRARIES})

# 设置包含目录
target_include_directories(TestLight PUBLIC
        "${PROJECT_BINARY_DIR}"
        "${PROJECT_SOURCE_DIR}/include"
        "${PROJECT_SOURCE_DIR}/src"
        )