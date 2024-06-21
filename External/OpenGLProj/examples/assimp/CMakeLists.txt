

include_directories(${PROJECT_BINARY_DIR}/external/assimp/include)
project(examples_assimp)
# 包含其他目录中的所有文件

# 添加测试源文件
file(GLOB TEST_SOURCES "main.cpp")

# 合并所有源文件
set(SOURCES ${TEST_SOURCES} ${CORE_SRC} assimp_include.h)
# 创建可执行文件
add_executable(examples_assimp ${SOURCES} )
# 链接库
target_link_libraries(examples_assimp PUBLIC glfw glad liblua assimp glm ${OPENGL_LIBRARIES})
