# ZEngine Asset System Implementation Plan

## 实现阶段规划

### 第一阶段：基础架构 (Phase 1)

1. **增强 Asset 基类** - 添加序列化、缩略图支持
2. **创建 AssetInfo 和 AssetRegistry** - 轻量级元数据系统
3. **实现 AssetReference 模板** - 懒加载引用系统
4. **创建 TextureAsset 类** - 统一 Texture 管理

### 第二阶段：加载系统 (Phase 2)

1. **异步加载系统** - 线程池和优先队列
2. **AssetCache** - LRU 缓存管理
3. **默认资源系统** - 防止空指针
4. **AssetManager 增强** - 支持新的 Asset 类型

### 第三阶段：组件重构 (Phase 3) - ✅ 基本完成

1. **MeshFilter 重构** - ✅ 已完成
   - 使用 AssetReference<MeshAsset> 进行资源管理
   - 支持懒加载和异步加载
   - 实现了序列化支持
   - 保持了向后兼容性

2. **Material 重构** - ✅ 已完成
   - 支持 AssetReference<TextureAsset>
   - 实现了 AssetTextureBinding 结构
   - 支持纹理资源的统一管理
   - 在 MaterialNew.h 中完全重构

3. **序列化系统更新** - ✅ 已完成
   - AssetReference 模板支持序列化/反序列化
   - 组件序列化系统已更新
   - 支持 Asset 引用的持久化

4. **组件测试** - ✅ 已完成
   - AssetManagerTestScene 验证了 MeshAsset 系统
   - 包含了完整的加载、管理和集成测试
   - 验证了 MeshFilter 的 AssetReference 功能

**完成的核心功能：**
- ✅ AssetReference<T> 模板系统
- ✅ TextureAsset 类完整实现
- ✅ MeshAsset 增强版本
- ✅ 异步加载支持
- ✅ 序列化/反序列化支持
- ✅ 组件集成测试

### 第四阶段：Editor 集成 (Phase 4) - 🚧 进行中

#### 4.1 AssetDragDropSystem - 统一拖拽框架
- [ ] 设计拖拽数据结构 (DragDropPayload)
- [ ] 实现拖拽源管理 (DragSource)
- [ ] 实现拖拽目标管理 (DropTarget)
- [ ] 添加拖拽视觉反馈系统

#### 4.2 AssetField 模板 - 通用资源选择 UI
- [ ] 创建 AssetField<T> 模板类
- [ ] 实现资源预览功能
- [ ] 添加资源选择对话框
- [ ] 集成拖拽接受功能

#### 4.3 Inspector 重构 - 支持所有 Asset 类型拖拽
- [ ] 更新 MeshRendererInspector 支持纹理拖拽
- [ ] 创建 MeshFilterInspector 支持网格拖拽
- [ ] 实现通用组件头部渲染
- [ ] 添加资源字段验证

#### 4.4 ContentBrowser 增强 - 统一资源浏览
- [ ] 实现资源扫描和注册
- [ ] 添加拖拽源支持
- [ ] 实现缩略图生成
- [ ] 添加资源过滤和搜索

**实现策略：**
1. 从底层拖拽系统开始
2. 创建通用 UI 组件
3. 逐步集成到现有 Inspector
4. 最后完善 ContentBrowser

## 当前状态分析

基于现有代码分析：

- ✅ Asset 基类已存在并增强完成
- ✅ MeshAsset 已完整实现
- ✅ TextureAsset 已完整实现
- ✅ AssetReference 模板已完整实现
- ✅ AssetRegistry 已实现（在 AssetManager 中）
- ✅ 异步加载系统已实现
- ✅ 组件重构已完成
- ❌ Editor 拖拽系统尚未完成

## 第一阶段详细任务

### 1.1 增强 Asset 基类

- [ ] 添加 assetId 字段
- [ ] 添加序列化接口
- [ ] 添加缩略图支持
- [ ] 添加依赖关系管理

### 1.2 创建 AssetInfo 和 AssetRegistry

- [ ] 定义 AssetInfo 结构
- [ ] 实现 AssetRegistry 单例
- [ ] 添加文件扫描功能
- [ ] 添加元数据管理

### 1.3 实现 AssetReference 模板

- [ ] 设计模板接口
- [ ] 实现懒加载机制
- [ ] 添加默认资源支持
- [ ] 添加序列化支持

### 1.4 创建 TextureAsset 类

- [ ] 继承 Asset 基类
- [ ] 包装现有 Texture 类
- [ ] 实现加载/卸载
- [ ] 添加缩略图生成

## 实现优先级

**高优先级**：

1. Asset 基类增强
2. AssetReference 模板
3. TextureAsset 创建

**中优先级**：

1. AssetRegistry 实现
2. 异步加载系统
3. 组件重构

**低优先级**：

1. 缓存系统
2. Editor 集成
3. 性能优化

## 测试策略

每个阶段完成后进行：

1. **单元测试** - 验证核心功能
2. **集成测试** - 验证组件协作
3. **性能测试** - 验证加载性能
4. **Editor 测试** - 验证拖拽功能

## 风险评估

**技术风险**：

- 异步加载的线程安全
- 循环依赖的处理
- 内存管理的复杂性

**兼容性风险**：

- 现有代码的重构影响
- 序列化格式的向后兼容
- Editor 工具的兼容性

**性能风险**：

- 频繁的 Asset 查找
- 大量小文件的 IO
- 内存碎片化

## 下一步行动

让我们从第一阶段开始，首先增强 Asset 基类，然后依次实现每个组件。

准备好开始实现了吗？我们从哪个组件开始？
