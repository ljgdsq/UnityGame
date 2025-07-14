# Hierarchy 右键菜单问题修复

## 问题描述
空白区域右键菜单显示"Create GameObject"但是左键右键都没有响应。

## 问题原因
1. **空白区域检测逻辑错误**: 原来的检测逻辑在 `BeginListBox` 和 `EndListBox` 之间，导致检测不准确
2. **ContextMenu 类不适用**: `ContextMenu` 使用 `ImGui::BeginPopupContextWindow`，这是自动弹出的，不适合手动控制
3. **菜单状态管理混乱**: 菜单开关状态和实际显示状态不同步

## 修复方案

### 1. 优化空白区域检测
```cpp
// 将检测逻辑移到 EndListBox 之后，使用正确的检测方法
if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right) && !clickedOnItem)
{
    showEmptyAreaPop = true;
    showpop = false;
}
```

### 2. 直接使用 ImGui 原生弹出菜单
不再使用 `ContextMenu` 类，直接使用：
- `ImGui::OpenPopup()` - 打开弹出菜单
- `ImGui::BeginPopup()` - 开始弹出菜单内容
- `ImGui::MenuItem()` - 菜单项
- `ImGui::EndPopup()` - 结束弹出菜单

### 3. 简化状态管理
```cpp
// 简化的状态检查
if (!ImGui::IsPopupOpen("HierarchyContextMenu"))
{
    showpop = false;
}
if (!ImGui::IsPopupOpen("EmptyAreaContextMenu"))
{
    showEmptyAreaPop = false;
}
```

## 修复后的功能

### 选中物体右键菜单
- ✅ Delete - 删除选中的 GameObject
- ✅ Duplicate - 复制 GameObject（待实现逻辑）
- ✅ Rename - 重命名 GameObject（待实现逻辑）

### 空白区域右键菜单
- ✅ Empty GameObject - 创建空的 GameObject
- ✅ Cube - 创建立方体 GameObject
- ✅ Sphere - 创建球体 GameObject
- ✅ Plane - 创建平面 GameObject

## 测试方法
1. **测试空白区域右键**: 在 Hierarchy 列表的空白处右键，应该显示创建菜单
2. **测试物体右键**: 右键点击具体的 GameObject，应该显示操作菜单
3. **测试菜单互斥**: 两种菜单不会同时显示
4. **测试菜单响应**: 点击菜单项应该有相应的效果

## 注意事项
- 菜单点击后会自动关闭
- 点击菜单外部区域也会关闭菜单
- 创建的 GameObject 会立即添加到场景中
- 删除操作会立即生效，暂无撤销功能

## 后续优化建议
1. 为几何体对象自动添加相应的组件（MeshRenderer, MeshFilter）
2. 实现 Duplicate 和 Rename 功能
3. 添加撤销/重做功能
4. 优化菜单的视觉效果和分组
