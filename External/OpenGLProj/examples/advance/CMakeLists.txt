
project(advance)
# 包含其他目录中的所有文件

# 添加测试源文件
file(GLOB TEST_SOURCES "main.cpp")

# 合并所有源文件
set(SOURCES ${TEST_SOURCES} ${CORE_SRC} )
# 创建可执行文件
add_executable(advance ${SOURCES} )
# 链接库
target_link_libraries(advance PUBLIC glfw glad liblua glm ${OPENGL_LIBRARIES})
