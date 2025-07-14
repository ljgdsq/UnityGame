# VS Code IntelliJ IDEA 风格快捷键配置

## 解决 Shift+Shift 无效的问题

我们已经为你的 VS Code 配置了 IntelliJ IDEA 风格的快捷键。以下是主要配置和使用说明：

## 核心搜索快捷键

### Shift+Shift - 搜索所有内容
- **功能**: 模拟 IntelliJ IDEA 的 "Search Everywhere" 功能
- **VS Code 映射**: `workbench.action.quickOpen` (快速打开文件)
- **用途**: 快速搜索和打开项目中的文件

### 其他搜索快捷键
- `Ctrl+Shift+F`: 在文件中搜索
- `Ctrl+Shift+R`: 打开最近的文件/项目
- `Ctrl+G`: 跳转到指定行号

## 文件和导航

| 快捷键 | 功能 | IntelliJ 对应 |
|--------|------|---------------|
| `Ctrl+Shift+N` | 新建文件 | New File |
| `Ctrl+B` | 跳转到定义 | Go to Declaration |
| `Ctrl+Alt+B` | 跳转到实现 | Go to Implementation |
| `Ctrl+U` | 跳转到声明 | Go to Declaration |
| `Ctrl+Shift+I` | 查看定义（预览） | Quick Definition |
| `Alt+F7` | 查找引用 | Find Usages |
| `Ctrl+Alt+F7` | 查找引用（详细） | Find Usages |

## 编辑和重构

| 快捷键 | 功能 | IntelliJ 对应 |
|--------|------|---------------|
| `Ctrl+D` | 复制选中行/选区 | Duplicate Line |
| `Ctrl+Y` | 删除当前行 | Delete Line |
| `Ctrl+Shift+↑` | 向上移动行 | Move Line Up |
| `Ctrl+Shift+↓` | 向下移动行 | Move Line Down |
| `Ctrl+W` | 扩展选择 | Extend Selection |
| `Ctrl+Shift+W` | 缩小选择 | Shrink Selection |
| `Ctrl+Alt+L` | 格式化代码 | Reformat Code |
| `F2` | 重命名 | Rename |
| `Ctrl+Alt+V` | 重构菜单 | Refactor This |

## 调试和运行

| 快捷键 | 功能 | IntelliJ 对应 |
|--------|------|---------------|
| `Shift+F10` | 运行程序 | Run |
| `Ctrl+Shift+F10` | 调试程序 | Debug |

## 故障排除

### 如果 Shift+Shift 仍然无效：

1. **重启 VS Code**: 配置更改后需要重启才能生效
2. **检查扩展冲突**: 某些扩展可能会覆盖快捷键
3. **手动测试**: 在命令面板（`Ctrl+Shift+P`）中输入 "Open Keyboard Shortcuts" 来检查配置

### 安装推荐扩展

我们已经在 `.vscode/extensions.json` 中配置了推荐扩展，包括：
- `IntelliJ IDEA Keybindings`: 官方 IntelliJ 键位映射
- `C++ Extension Pack`: C++ 开发工具集
- `GitLens`: 增强的 Git 功能

### 手动安装 IntelliJ 键位扩展

如果自动推荐不起作用，可以手动安装：
1. 按 `Ctrl+Shift+X` 打开扩展面板
2. 搜索 "IntelliJ IDEA Keybindings"
3. 安装 by k--kato 的扩展

## 配置文件说明

### 已创建的配置文件：
- `.vscode/keybindings.json`: 自定义快捷键映射
- `.vscode/settings.json`: 编辑器设置（已更新）
- `.vscode/extensions.json`: 推荐扩展列表

### 重要设置说明：
- `keyboard.dispatch: "keyCode"`: 确保快捷键正确识别
- `workbench.quickOpen.closeOnFocusLost: false`: 搜索框不会自动关闭
- `editor.multiCursorModifier: "ctrlCmd"`: 多光标修饰键设置

## 验证配置

重启 VS Code 后，尝试以下操作验证配置是否生效：
1. 按 `Shift+Shift` 应该打开快速搜索框
2. 按 `Ctrl+Shift+F` 应该打开全局搜索
3. 按 `Ctrl+D` 应该复制当前行

如果仍有问题，请检查：
- VS Code 版本是否为最新
- 是否有其他键位绑定扩展冲突
- Windows 系统是否有全局快捷键冲突
