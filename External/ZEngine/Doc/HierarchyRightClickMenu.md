# Hierarchy Widget 右键菜单功能

## 新增功能说明

已为 Hierarchy Widget 添加了两种右键菜单功能：

### 1. 选中物体的右键菜单
当你在 Hierarchy 中右键点击某个 GameObject 时，会显示：
- **Delete** - 删除选中的 GameObject
- **Duplicate** - 复制 GameObject（待实现）
- **Rename** - 重命名 GameObject（待实现）

### 2. 空白区域的右键菜单
当你在 Hierarchy 的空白区域右键点击时，会显示创建菜单：
- **Empty GameObject** - 创建空的 GameObject
- **Cube** - 创建立方体 GameObject
- **Sphere** - 创建球体 GameObject
- **Plane** - 创建平面 GameObject

## 技术实现

### 文件修改
1. **Hierarchy.h** - 添加了 `emptyAreaContextMenu` 成员变量
2. **Hierarchy.cpp** - 实现了双菜单系统和空白区域检测
3. **Scene.h** - 添加了编辑器接口方法 `AddGameObjectFromEditor` 和 `RemoveGameObjectFromEditor`

### 关键特性
- **互斥菜单**: 确保同时只显示一个右键菜单
- **精确检测**: 区分物体点击和空白区域点击
- **编辑器集成**: 通过 Scene 的公共接口安全地添加/删除 GameObject

### 使用方法
1. **创建新物体**: 在 Hierarchy 空白处右键 → 选择要创建的物体类型
2. **操作现有物体**: 右键点击具体的 GameObject → 选择操作（删除/复制/重命名）

## 待实现功能
- Duplicate（复制）功能的具体实现
- Rename（重命名）功能的 UI 和逻辑
- 为创建的几何体自动添加相应的组件（MeshRenderer, MeshFilter 等）
- 撤销/重做功能
- 拖拽排序功能

## 注意事项
- 删除操作会立即生效，请确保有适当的确认机制
- 新创建的 GameObject 目前只有基础的 Transform 组件
- 几何体的具体网格数据需要从资源系统加载
