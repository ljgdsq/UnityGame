# Hierarchy Widget - Rename 功能实现

## 功能概述
为 Hierarchy Widget 添加了完整的 GameObject 重命名功能，支持通过右键菜单快速重命名对象。

## 实现细节

### 1. 新增状态变量
```cpp
static bool showRenamePopup = false;  // 重命名弹窗标志
static char renameBuffer[256] = "";   // 重命名输入缓冲区
```

### 2. 重命名流程
1. **触发重命名**：右键点击 GameObject → 选择 "Rename"
2. **准备数据**：将当前 GameObject 名称复制到输入缓冲区
3. **显示对话框**：弹出模态对话框供用户输入新名称
4. **应用更改**：确认后调用 `GameObject::SetName()` 更新名称

### 3. 用户交互
- **输入框自动聚焦**：弹窗打开时光标自动定位到输入框
- **快捷键支持**：
  - `Enter` - 确认重命名
  - `Escape` - 取消重命名
- **按钮操作**：
  - `OK` - 确认重命名
  - `Cancel` - 取消重命名

### 4. 数据验证
- **非空检查**：只有输入非空字符串才会应用重命名
- **边界检查**：确保索引有效且 GameObject 存在
- **缓冲区安全**：使用 `strncpy` 防止缓冲区溢出

## 代码特点

### 安全性
```cpp
// 安全的字符串复制
strncpy(renameBuffer, currentName.c_str(), sizeof(renameBuffer) - 1);
renameBuffer[sizeof(renameBuffer) - 1] = '\0';

// 边界检查
if (index < childs.size() && strlen(renameBuffer) > 0) {
    childs[index]->SetName(std::string(renameBuffer));
}
```

### 用户体验
```cpp
// 自动聚焦输入框
ImGui::SetKeyboardFocusHere();

// 支持 Enter 键确认
bool enterPressed = ImGui::InputText("##rename", renameBuffer, sizeof(renameBuffer),
                                   ImGuiInputTextFlags_EnterReturnsTrue);

// ESC 键取消
if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
    ImGui::CloseCurrentPopup();
}
```

### 状态管理
```cpp
// 避免重复调用 OpenPopup
if (showRenamePopup)
{
    ImGui::OpenPopup("Rename GameObject");
    showRenamePopup = false; // 只在第一帧打开
}
```

## 使用方法

1. **启动重命名**：
   - 在 Hierarchy 中右键点击任意 GameObject
   - 从上下文菜单中选择 "Rename"

2. **输入新名称**：
   - 在弹出的对话框中输入新的名称
   - 原名称会自动填充到输入框中

3. **确认操作**：
   - 点击 "OK" 按钮或按 `Enter` 键确认
   - 点击 "Cancel" 按钮或按 `Escape` 键取消

## 技术要点

### 模态对话框
使用 `ImGui::BeginPopupModal()` 创建模态对话框，确保：
- 对话框显示时阻止与其他 UI 的交互
- 用户必须明确选择确认或取消
- 提供良好的用户体验

### 内存安全
- 使用固定大小的字符数组避免动态内存分配
- `strncpy` 防止缓冲区溢出
- 明确设置字符串终止符

### 键盘交互
- `ImGui::SetKeyboardFocusHere()` - 自动聚焦
- `ImGuiInputTextFlags_EnterReturnsTrue` - Enter 键确认
- `ImGui::IsKeyPressed(ImGuiKey_Escape)` - ESC 键取消

## 扩展建议

1. **输入验证**：
   - 禁止空名称或只包含空格的名称
   - 检查名称长度限制
   - 验证特殊字符

2. **重名检测**：
   - 检查场景中是否已存在相同名称的对象
   - 提供重名警告或自动添加后缀

3. **撤销功能**：
   - 保存重命名前的状态
   - 支持撤销重命名操作

4. **批量重命名**：
   - 支持选择多个对象进行批量重命名
   - 提供模式匹配和替换功能
