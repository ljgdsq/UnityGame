
# 指定CMake的最低版本要求
cmake_minimum_required(VERSION 3.14)

# 定义项目名称和使用的语言
project(HelloGTest LANGUAGES CXX)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
# 设置C++标准为C++17
set(CMAKE_CXX_STANDARD 17)                # 使用C++17标准
set(CMAKE_CXX_STANDARD_REQUIRED ON)       # 必须使用指定的C++标准
set(CMAKE_CXX_EXTENSIONS OFF)             # 禁用编译器特定扩展

# 添加主程序可执行文件
add_executable(hello src/hello.cpp)       # 生成hello.exe，源码在src/hello.cpp

# 设置主程序的输出目录，兼容多配置生成器（如Visual Studio）
if (CMAKE_CONFIGURATION_TYPES)
  set_target_properties(hello PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_BINARY_DIR}/Debug/bin    # Debug模式输出路径
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release/bin  # Release模式输出路径
  )
else()
  set_target_properties(hello PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # 单配置生成器输出路径
endif()

set(CMAKE_FOLDER "thirdlib") # 这会影响所有接下来引入的目标


# 集成GoogleTest，自动下载和添加到项目
include(FetchContent)                     # 引入FetchContent模块用于下载依赖
FetchContent_Declare(
  googletest                              # 声明GoogleTest依赖
  URL https://github.com/google/googletest/archive/refs/heads/main.zip # 下载地址
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
# Windows下强制使用父项目的编译器/链接器设置，避免冲突
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)    # 下载并添加GoogleTest

FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.13.0
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(spdlog)

set(CMAKE_FOLDER "") # 重置

# 启用测试功能
enable_testing()                          # 允许添加和运行测试

# 添加测试可执行文件
add_executable(test_hello tests/test_hello.cpp) # 生成test_hello.exe，源码在tests/test_hello.cpp
target_link_libraries(test_hello PRIVATE gtest_main spdlog::spdlog) # 链接GoogleTest主库
target_link_libraries(hello PRIVATE spdlog::spdlog)
# 设置测试程序的输出目录，兼容多配置生成器
if (CMAKE_CONFIGURATION_TYPES)
  set_target_properties(test_hello PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_BINARY_DIR}/Debug      # Debug模式输出路径
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release    # Release模式输出路径
  )
else()
  set_target_properties(test_hello PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}) # 单配置生成器输出路径
endif()

# set_target_properties(spdlog PROPERTIES FOLDER "thirdlib")

# 注册测试，ctest会自动运行test_hello
add_test(NAME HelloTest COMMAND test_hello)




