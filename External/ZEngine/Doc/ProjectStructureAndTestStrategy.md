# 项目结构与测试管理推荐方案

## 1. 推荐目录结构

```
ZEngine/
├── Framework/           # 引擎核心代码
├── External/            # 第三方库
├── Assets/              # 导入后的资源
├── Res/                 # 原始资源
├── Editor/              # 编辑器相关代码
├── App/                 # 主要应用入口（如游戏/编辑器/工具）
│   ├── GameApp/         # 游戏主程序
│   ├── EditorApp/       # 编辑器主程序
│   └── ...              # 其他独立应用
├── Tests/               # 单元测试、集成测试
│   ├── AssetSystem/     # 资源系统相关测试
│   ├── Render/          # 渲染相关测试
│   ├── Editor/          # 编辑器相关测试
│   └── ...              # 其他模块测试
├── Scripts/             # 构建/运行脚本
├── Doc/                 # 文档
└── main.cpp             # 入口（可根据需要拆分）
```

## 2. 测试与应用加载策略

### 方案一：多入口可执行文件（推荐）
- `App/EditorApp/main.cpp` → 生成 `EditorApp.exe`
- `App/GameApp/main.cpp` → 生成 `GameApp.exe`
- `Tests/AssetSystem/main.cpp` → 生成 `AssetSystemTest.exe`
- `Tests/Render/main.cpp` → 生成 `RenderTest.exe`
- CMakeLists.txt 用 `add_executable()` 为每个入口生成独立可执行文件

**优点**：每个测试/应用独立编译、运行，互不干扰，易于自动化和调试。

### 方案二：统一测试入口 + 命令行参数
- 在 `Tests/main.cpp` 里注册所有测试
- 通过命令行参数选择要运行的测试

```cpp
int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "用法: test.exe [TestName]" << std::endl;
        return 1;
    }
    std::string testName = argv[1];
    if (testName == "AssetSystem") {
        framework::AssetSystemTest::RunTests();
    } else if (testName == "Render") {
        framework::RenderTest::RunTests();
    } // ...更多测试
    else {
        std::cout << "未知测试: " << testName << std::endl;
        return 1;
    }
    return 0;
}
```

**优点**：只需一个可执行文件，方便批量测试和集成。

### 方案三：测试框架（如 GoogleTest）
- 用 GoogleTest、Catch2 等测试框架，自动发现和运行所有测试
- 只需编写测试用例，框架自动管理入口和报告

## 3. CMake 配置建议

```cmake
add_executable(EditorApp App/EditorApp/main.cpp ...)
add_executable(GameApp App/GameApp/main.cpp ...)
add_executable(AssetSystemTest Tests/AssetSystem/main.cpp ...)
add_executable(RenderTest Tests/Render/main.cpp ...)
```

## 4. 总结
- 推荐每个测试/应用单独一个入口和可执行文件，CMake独立管理，结构清晰。
- 如需统一入口，可用命令行参数选择测试。
- 如需自动化和报告，建议用测试框架。
- 这样项目结构清晰，易于扩展和维护，适合团队协作和持续集成。
