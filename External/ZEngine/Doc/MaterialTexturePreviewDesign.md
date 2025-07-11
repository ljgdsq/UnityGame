# MeshRendererInspector 中的材质纹理预览 - 设计文档

## 概述

本文档概述了在 MeshRendererInspector 面板中实现纹理预览功能的设计。目标是通过允许用户直接在检查器中可视化预览与材质相关的纹理，来增强材质编辑体验。

## 当前状态分析

### 当前实现
- **MeshRendererInspector**: 目前只显示材质名称文本
- **Material 类**: 具有基本的纹理设置功能通过 `SetTexture(name, slot)` 但没有纹理存储/检索
- **Texture 类**: 提供纹理加载、绑定和通过 `GetTextureID()` 访问 OpenGL ID
- **RenderComponent**: 通过 `GetMaterial()` 和 `SetMaterial()` 提供材质管理

### 当前限制
1. 无法从材质中检索纹理进行预览
2. Material 类中没有纹理存储/管理
3. 检查器中没有关于材质纹理的视觉反馈
4. 没有简单的方法浏览和分配纹理到材质

## 设计目标

1. **非侵入性**: 对现有架构的最小更改
2. **性能**: 高效的纹理预览而不影响渲染性能
3. **用户体验**: 直观的纹理预览和分配界面
4. **可扩展性**: 支持多种纹理类型（漫反射、法线、镜面等）
5. **可维护性**: 清晰的关注点分离

## 建议架构

### 1. Material 类扩展

#### 新数据结构
```cpp
// 新的纹理绑定结构
struct TextureBinding {
    std::string name;           // 统一名称（例如 "texture_diffuse1"）
    Texture* texture;           // 纹理对象指针
    int slot;                   // 绑定的纹理槽
    TextureType type;           // 类型分类（漫反射、法线等）
};

// 纹理类型枚举
enum class TextureType {
    DIFFUSE,                    // 漫反射
    NORMAL,                     // 法线
    SPECULAR,                   // 镜面反射
    METALLIC,                   // 金属度
    ROUGHNESS,                  // 粗糙度
    AMBIENT_OCCLUSION,          // 环境遮挡
    EMISSION,                   // 发射
    HEIGHT,                     // 高度
    CUSTOM                      // 自定义
};
```

#### 增强的 Material 接口
```cpp
class Material {
private:
    std::vector<TextureBinding> m_textureBindings;
    std::unordered_map<std::string, size_t> m_textureNameToIndex;

public:
    // 增强的纹理管理
    void SetTexture(const std::string& name, Texture* texture, int slot = 0, TextureType type = TextureType::DIFFUSE);
    Texture* GetTexture(const std::string& name) const;
    const std::vector<TextureBinding>& GetAllTextures() const;
    
    // 纹理类型查询
    std::vector<TextureBinding> GetTexturesByType(TextureType type) const;
    bool HasTexture(const std::string& name) const;
    void RemoveTexture(const std::string& name);
    
    // 检查器特定的辅助方法
    size_t GetTextureCount() const;
    TextureBinding* GetTextureAtIndex(size_t index);
};
```

### 2. MeshRendererInspector 扩展

#### 新的检查器组件
```cpp
class MeshRendererInspector {
private:
    // 预览配置
    static constexpr float PREVIEW_SIZE = 64.0f;
    static constexpr float LARGE_PREVIEW_SIZE = 256.0f;
    static constexpr int MAX_PREVIEW_TEXTURES = 8;
    
    // 状态管理
    bool m_showLargePreview = false;
    int m_selectedTextureIndex = -1;
    std::string m_textureFilter = "";

public:
    // 增强的检查方法
    void RenderMaterialTexturePreview(Material* material);
    void RenderTextureGrid(const std::vector<TextureBinding>& textures);
    void RenderTextureDetails(const TextureBinding& binding);
    void RenderTextureAssignmentUI(Material* material);
    
private:
    // 辅助方法
    void RenderTexturePreviewTile(const TextureBinding& binding, int index);
    void RenderLargeTexturePreview(const TextureBinding& binding);
    void HandleTextureDropArea(Material* material, const std::string& uniformName);
    void ShowTextureContextMenu(Material* material, const TextureBinding& binding);
};
```

### 3. 纹理 ID 管理策略

#### 方案 A: Material 管理的纹理 ID（推荐）
在 Material 类中直接存储纹理 ID 用于 ImGui 访问：

```cpp
class Material {
private:
    struct TexturePreviewData {
        unsigned int textureID;     // 用于 ImGui 的 OpenGL 纹理 ID
        ImVec2 size;               // 原始纹理尺寸
        std::string filename;       // 显示名称
        bool isValid;              // 有效性标志
    };
    
    std::unordered_map<std::string, TexturePreviewData> m_texturePreviewCache;

public:
    // ImGui 特定的纹理访问
    unsigned int GetTextureIDForPreview(const std::string& name) const;
    ImVec2 GetTextureSize(const std::string& name) const;
    void InvalidateTexturePreview(const std::string& name);
    void RefreshTexturePreviewCache();
};
```

#### 方案 B: 检查器管理的缓存
检查器维护自己的纹理 ID 缓存的替代方案：

```cpp
class MeshRendererInspector {
private:
    struct TexturePreviewCache {
        std::unordered_map<std::string, unsigned int> textureIDCache;
        std::unordered_map<std::string, ImVec2> textureSizeCache;
        Material* cachedMaterial = nullptr;
        
        void UpdateCache(Material* material);
        void ClearCache();
        unsigned int GetTextureID(const std::string& name);
    };
    
    static TexturePreviewCache s_previewCache;
};
```

### 4. 用户界面设计

#### 纹理预览布局
```
┌─────────────────────────────────────────────────┐
│ 材质: StandardMaterial                          │
├─────────────────────────────────────────────────┤
│ 纹理 (3/8)                      [过滤器: __]   │
├─────────────────────────────────────────────────┤
│ ┌─────────┐ ┌─────────┐ ┌─────────┐ ┌─────────┐ │
│ │ [漫反射] │ │ [法线]  │ │ [镜面]  │ │ [+添加] │ │
│ │ 512x512 │ │ 1024x1  │ │ 256x256 │ │  新纹理  │ │
│ │face.png │ │norm.png │ │spec.png │ │        │ │
│ └─────────┘ └─────────┘ └─────────┘ └─────────┘ │
├─────────────────────────────────────────────────┤
│ 已选择: texture_diffuse1                        │
│ 尺寸: 512x512 | 格式: RGB | 路径: face.png     │
│ [更换] [移除] [查看大图]                         │
└─────────────────────────────────────────────────┘
```

#### 纹理分配用户界面
```
┌─────────────────────────────────────────────────┐
│ 分配纹理                                        │
├─────────────────────────────────────────────────┤
│ 统一名称: [texture_diffuse1        ] [v]        │
│ 纹理类型: [漫反射                   ] [v]        │
│ 纹理槽位: [0                       ]            │
├─────────────────────────────────────────────────┤
│ ┌─────────────────────────────────────────────┐ │
│ │ 拖拽纹理到此处                              │ │
│ │ 或者                                        │ │
│ │ [浏览文件...]                               │ │
│ └─────────────────────────────────────────────┘ │
└─────────────────────────────────────────────────┘
```

### 5. 实现策略

#### 阶段 1: 核心基础设施
1. 扩展 Material 类的纹理存储和检索
2. 实现 TextureBinding 结构和管理
3. 添加纹理 ID 缓存机制
4. 创建基本纹理预览功能

#### 阶段 2: 检查器增强
1. 实现纹理网格显示
2. 添加纹理详情面板
3. 创建纹理分配界面
4. 实现拖拽功能

#### 阶段 3: 高级功能
1. 添加纹理过滤和搜索
2. 实现大图预览模态框
3. 添加纹理右键菜单操作
4. 实现纹理类型分类

#### 阶段 4: 集成和润色
1. 与 ContentBrowser 连接进行纹理浏览
2. 添加纹理验证和错误处理
3. 实现性能优化
4. 添加预览设置用户偏好

### 6. 性能考虑

#### 纹理加载优化
- **懒加载**: 只有在检查器打开时才加载纹理预览
- **缩略图生成**: 为网格显示创建更小的预览版本
- **内存管理**: 实现有大小限制的纹理缓存
- **异步加载**: 异步加载纹理以防止 UI 阻塞

#### 渲染性能
- **纹理绑定优化**: 缓存纹理绑定以避免冗余的 OpenGL 调用
- **预览尺寸限制**: 将预览尺寸限制在合理范围内
- **选择性更新**: 只有在材质更改时才更新纹理预览

### 7. 错误处理和边界情况

#### 纹理加载失败
- 为失败的加载显示占位符纹理
- 显示带有失败原因的错误提示
- 为临时失败提供重试机制

#### 内存管理
- 实现纹理引用计数
- 优雅地处理纹理删除
- 防止预览缓存中的内存泄漏

#### UI 响应性
- 限制同时进行的纹理预览数量
- 为大纹理列表实现虚拟化
- 为慢操作提供进度指示器

### 8. 测试策略

#### 单元测试
- Material 纹理存储和检索
- TextureBinding 操作
- 预览缓存管理
- 错误处理场景

#### 集成测试
- 检查器-材质集成
- 纹理分配工作流
- 负载下的性能
- 内存使用模式

#### 用户体验测试
- 纹理预览准确性
- 大纹理的 UI 响应性
- 拖拽功能
- 右键菜单操作

### 9. 未来增强功能

#### 高级功能
- **纹理图集**: 支持纹理图集预览
- **动画预览**: 预览动画纹理
- **实时编辑**: 实时纹理参数调整
- **批量操作**: 多纹理分配和修改

#### 集成机会
- **材质编辑器**: 专用材质编辑窗口
- **纹理检查器**: 详细纹理属性编辑器
- **资产管道**: 与纹理导入设置集成
- **着色器集成**: 从着色器自动检测纹理槽

### 10. 迁移路径

#### 向后兼容性
- 现有材质无需修改即可继续工作
- 新纹理存储是可选的且向后兼容
- 现有材质逐步迁移到新系统

#### 废弃策略
- 将旧纹理方法标记为废弃
- 提供迁移工具
- 在过渡期间支持新旧系统

## 结论

这个设计为 MeshRendererInspector 中的材质纹理预览提供了一个全面的解决方案，同时保持与现有架构的兼容性。模块化方法允许增量实现和未来增强，同时解决了纹理管理和可视化方面的当前限制。

推荐的方法优先考虑：
1. **Material 管理的纹理存储**以获得清晰的所有权
2. **增量实现**以降低风险
3. **性能优化**以获得流畅的用户体验
4. **可扩展性**以便未来增强

该设计确保纹理预览功能与现有的 ZEngine 架构无缝集成，同时为高级材质编辑功能提供坚实的基础。
