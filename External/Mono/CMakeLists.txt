# CMakeLists.txt

cmake_minimum_required(VERSION 3.10)
project(MonoDemo)
# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)

# 设置 Mono 的路径
set(MONO_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/mono/include")
set(MONO_LIB_DIR "${CMAKE_SOURCE_DIR}/mono/lib")

# 添加 Mono 的头文件目录
include_directories(${MONO_INCLUDE_DIR})

## 查找 Mono 库，静态链接
#find_library(MONO_LIBRARY monosgen-2.0 HINTS ${MONO_LIB_DIR})
#
## 如果没有找到库，报错
#if(NOT MONO_LIBRARY)
#    message(FATAL_ERROR "Could not find Mono library!")
#endif()
link_directories(D:/GameEngine/MonoDemo/mono/lib)
# 添加可执行文件
add_executable(MonoDemo main.cpp)

# 将 Mono 库链接到 C++ 程序
target_link_libraries(MonoDemo
        ws2_32.lib
        winmm.lib
        Bcrypt.lib
        zlibstatic.lib
        mono-component-debugger-static.lib
        mono-component-debugger-stub-static.lib
        mono-component-diagnostics_tracing-static.lib
        mono-component-diagnostics_tracing-stub-static.lib
        mono-component-hot_reload-static.lib
        mono-component-hot_reload-stub-static.lib
        mono-component-marshal-ilgen-static.lib
        mono-component-marshal-ilgen-stub-static.lib
        mono-profiler-aot.lib
        monosgen-2.0.lib
        )

# 如果需要其他设置（例如调试符号等），可以在这里添加
# target_compile_options(MonoDemo PRIVATE -g)
