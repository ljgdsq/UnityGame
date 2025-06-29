# 解决 VSCode 状态栏构建时中文乱码的问题

如果您使用 VSCode 状态栏的 "Build" 按钮构建项目时遇到中文乱码问题，本文档提供了几种解决方案。

## 已经实施的解决方案

我们已经为您的项目实施了以下几种解决方案，应该能解决大多数情况下的中文乱码问题：

1. **更新了 `.vscode/tasks.json`**:
   - 使用 PowerShell 作为 shell，并在命令中设置了 UTF-8 编码 (`chcp 65001`)
   - 创建了几个不同的任务：Build、Run with UTF-8、Clean Build
   - 设置 "Build" 任务为默认构建任务

2. **创建了 `.vscode/launch.json`**:
   - 配置了调试设置，确保在外部终端中使用 UTF-8 编码
   - 在启动前执行 Build 任务

3. **创建了 `CMakePresets.json`**:
   - 设置 CMake 构建选项，包括 `/utf-8` 编译标志

## 如何使用

### 方法 1: 使用 VSCode 任务

1. 按 `Ctrl+Shift+B` 执行默认构建任务
2. 或者按 `Ctrl+Shift+P`，输入 "Tasks: Run Task"，然后选择以下任务之一：
   - "Build" - 构建项目
   - "Run with UTF-8" - 在 UTF-8 模式下运行项目
   - "Clean Build" - 清理并重新构建项目

### 方法 2: 使用 CMake 扩展

如果您安装了 VSCode 的 CMake 扩展，可以：

1. 在 VSCode 的左侧活动栏中点击 CMake 图标
2. 选择 "windows-default" 配置
3. 点击 "Build" 按钮构建项目
4. 点击 "Debug" 按钮调试项目

### 方法 3: 使用命令行

您也可以从终端运行以下命令：

```bash
# 切换到项目目录
cd C:\Users\zsl\Desktop\Projects\UnityGame\External\ZEngine

# 设置终端编码为 UTF-8
chcp 65001

# 构建项目
mkdir -p build
cd build
cmake ..
cmake --build . --config Debug

# 运行项目
cd Debug
.\zengine.exe
```

## 其他可能的解决方案

如果上述方法仍然无法解决中文乱码问题，可以尝试以下方法：

1. **使用 Windows Terminal**:
   - 安装 Windows Terminal（参见 `windows_terminal_guide.md`）
   - 在 Windows Terminal 中设置默认编码为 UTF-8
   - 在 Windows Terminal 中执行构建和运行命令

2. **修改 Windows 系统默认编码**:
   - 控制面板 -> 区域 -> 管理 -> 更改系统区域设置
   - 勾选 "使用 Unicode UTF-8 提供全球语言支持"
   - 重启计算机

3. **在 CMake 构建脚本中添加编码设置**:
   - 在项目的每个 `CMakeLists.txt` 文件中添加以下代码：
     ```cmake
     if(MSVC)
       set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /utf-8")
       set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")
     endif()
     ```

4. **使用 UTF-8 启动终端脚本**:
   - 使用项目根目录下的 `utf8_terminal.bat` 脚本启动一个 UTF-8 编码的终端
   - 在该终端中执行构建和运行命令
