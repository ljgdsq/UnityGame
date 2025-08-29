# Modern CMake + C++17 + GoogleTest Hello World Project

This project demonstrates a minimal setup for a C++17 application using modern CMake (>=3.14) and GoogleTest for unit testing.

## 目录结构
GTestProj/
├── CMakeLists.txt
├── Engine/           # 引擎核心模块
│   ├── CMakeLists.txt
│   ├── include/Engine/
│   └── src/
├── Editor/           # 编辑器（工具/可视化）
│   ├── CMakeLists.txt
│   ├── include/Editor/
│   └── src/
├── Launcher/         # 启动器/演示/样例（推荐替代 Sandbox）
│   ├── CMakeLists.txt
│   └── src/
├── Tests/            # 单元测试
│   ├── CMakeLists.txt
│   └── ...
├── ThirdParty/       # 第三方库
│   └── ...
└── Docs/             # 文档
    └── ...

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



##
