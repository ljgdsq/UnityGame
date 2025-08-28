# Modern CMake + C++17 + GoogleTest Hello World Project

This project demonstrates a minimal setup for a C++17 application using modern CMake (>=3.14) and GoogleTest for unit testing.

## 目录结构
- src/hello.cpp: 主程序
- tests/test_hello.cpp: 测试用例
- CMakeLists.txt: 构建脚本

## 构建与测试
1. 安装CMake（>=3.14）和支持C++17的编译器。
2. 运行如下命令：
   ```powershell
   mkdir build
   cd build
   cmake .. -DCMAKE_BUILD_TYPE=Release
   cmake --build .
   ctest
   ```

## 依赖
- GoogleTest（自动通过CMake下载）

## 运行
构建后，主程序可执行文件在 `build/bin/hello.exe`，测试可通过 `ctest` 运行。
