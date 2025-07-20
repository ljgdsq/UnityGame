# ZEngine Asset System 重构设计方案

## 1. 问题分析

### 1.1 现有系统问题

通过代码分析，发现以下问题：

1. **Asset 继承体系不统一**：

   - `Texture`类没有继承`Asset`基类
   - `Mesh`类没有继承`Asset`基类
   - 只有`MeshAsset`继承了`Asset`，但没有对应的`TextureAsset`

2. **资源管理分离**：

   - `Texture`和`Mesh`是独立的渲染类，不在 Asset 管理系统中
   - `AssetManager`无法统一管理所有资源类型

3. **拖拽系统不完整**：

   - 只有 Texture 支持拖拽，Mesh 不支持
   - 拖拽目标硬编码，缺乏统一的拖拽框架

4. **组件接口不一致**：
   - `MeshRenderer`使用`Material`来管理`Texture`
   - `MeshFilter`直接使用`Mesh`对象
   - 没有统一的 Asset 引用方式

## 2. 重构目标

### 2.1 统一 Asset 体系

- 所有可拖拽的资源都应该继承自`Asset`基类
- 提供统一的 Asset 加载、卸载和管理接口
- 支持 Asset 的序列化和反序列化

### 2.2 完善拖拽系统

- 实现通用的拖拽框架
- 支持所有 Asset 类型的拖拽操作
- 提供类型安全的拖拽验证

### 2.3 统一组件接口

- 组件通过 Asset 引用而非直接对象引用
- 支持 Asset 的动态加载和卸载
- 提供 Asset 缺失时的默认行为

## 3. 整体架构设计

```
┌─────────────────────────────────────────────────────────────┐
│                    Asset System Architecture                 │
└─────────────────────────────────────────────────────────────┘

┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   Asset (基类)   │    │  AssetManager   │    │  AssetLoader    │
│                │    │   (资源管理器)   │    │   (加载器)     │
└─────────────────┘    └─────────────────┘    └─────────────────┘
         │                       │                       │
         ├─────────────────────────────────────────────────
         │                       │
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│  TextureAsset   │    │   MeshAsset     │    │  MaterialAsset  │
│  (纹理资源)     │    │   (网格资源)    │    │   (材质资源)    │
└─────────────────┘    └─────────────────┘    └─────────────────┘
         │                       │                       │
         │                       │                       │
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│    Texture      │    │      Mesh       │    │    Material     │
│   (渲染对象)    │    │   (渲染对象)    │    │   (渲染对象)    │
└─────────────────┘    └─────────────────┘    └─────────────────┘

┌─────────────────────────────────────────────────────────────┐
│                    Editor Integration                        │
└─────────────────────────────────────────────────────────────┘

┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│ AssetRegistry   │    │ DragDropSystem  │    │ AssetInspector  │
│  (资源注册)     │    │   (拖拽系统)    │    │  (资源检查器)   │
└─────────────────┘    └─────────────────┘    └─────────────────┘
         │                       │                       │
         └───────────────────────┼───────────────────────┘
                                 │
                    ┌─────────────────┐
                    │  AssetField     │
                    │  (资源字段UI)   │
                    └─────────────────┘
```

## 4. 核心类设计

### 4.1 Asset 基类增强

```cpp
namespace framework {
    enum class AssetType {
        Unknown = 0,
        Texture,
        Mesh,
        Material,
        Shader,
        Audio,
        Animation,
        Font,
        Count
    };

    class Asset {
    public:
        Asset(const std::string& name, AssetType type);
        virtual ~Asset() = default;

        // 基本信息
        const std::string& GetName() const { return m_name; }
        const std::string& GetPath() const { return m_path; }
        AssetType GetType() const { return m_type; }

        // 唯一标识符
        const std::string& GetAssetId() const { return m_assetId; }

        // 加载状态
        bool IsLoaded() const { return m_isLoaded; }
        void SetLoaded(bool loaded) { m_isLoaded = loaded; }

        // 引用计数
        void AddRef() { ++m_refCount; }
        void Release();
        int GetRefCount() const { return m_refCount; }

        // 资源操作
        virtual void Load() = 0;
        virtual void Unload() = 0;
        virtual long GetSize() const = 0;

        // 序列化支持
        virtual rapidjson::Value Serialize(rapidjson::Document::AllocatorType& allocator) const = 0;
        virtual void Deserialize(const rapidjson::Value& json) = 0;

        // 缩略图支持（用于Editor）
        virtual void* GetThumbnailTextureId() const { return nullptr; }
        virtual void GenerateThumbnail() {}

    protected:
        std::string m_name;
        std::string m_path;
        std::string m_assetId;
        AssetType m_type;
        bool m_isLoaded = false;
        int m_refCount = 0;
    };
}
```

### 4.2 TextureAsset 设计

```cpp
namespace framework {
    class TextureAsset : public Asset {
    public:
        TextureAsset(const std::string& name);
        virtual ~TextureAsset() override;

        // Asset接口实现
        void Load() override;
        void Unload() override;
        long GetSize() const override;

        // 序列化
        rapidjson::Value Serialize(rapidjson::Document::AllocatorType& allocator) const override;
        void Deserialize(const rapidjson::Value& json) override;

        // 缩略图
        void* GetThumbnailTextureId() const override;
        void GenerateThumbnail() override;

        // TextureAsset特有方法
        std::shared_ptr<Texture> GetTexture() const { return m_texture; }
        TextureType GetTextureType() const { return m_textureType; }
        void SetTextureType(TextureType type) { m_textureType = type; }

        // 纹理信息
        int GetWidth() const;
        int GetHeight() const;
        int GetChannels() const;

    private:
        std::shared_ptr<Texture> m_texture;
        TextureType m_textureType = TextureType::DIFFUSE;
        void* m_thumbnailTextureId = nullptr;
    };
}
```

### 4.3 MeshAsset 增强

```cpp
namespace framework {
    class MeshAsset : public Asset {
    public:
        MeshAsset(const std::string& name);
        virtual ~MeshAsset() override;

        // Asset接口实现
        void Load() override;
        void Unload() override;
        long GetSize() const override;

        // 序列化
        rapidjson::Value Serialize(rapidjson::Document::AllocatorType& allocator) const override;
        void Deserialize(const rapidjson::Value& json) override;

        // 缩略图
        void* GetThumbnailTextureId() const override;
        void GenerateThumbnail() override;

        // MeshAsset特有方法
        std::shared_ptr<Mesh> GetMesh() const { return m_mesh; }
        void SetMesh(const std::shared_ptr<Mesh>& mesh);

        // 网格信息
        long GetVertexCount() const;
        long GetTriangleCount() const;
        glm::vec3 GetBounds() const;

    private:
        std::shared_ptr<Mesh> m_mesh;
        long m_vertexCount = 0;
        long m_triangleCount = 0;
        glm::vec3 m_bounds = glm::vec3(0);
        void* m_thumbnailTextureId = nullptr;
    };
}
```

### 4.4 AssetReference 模板

```cpp
namespace framework {
    template<typename AssetType>
    class AssetReference {
    public:
        AssetReference() = default;
        AssetReference(const std::string& assetId);
        AssetReference(std::shared_ptr<AssetType> asset);
        ~AssetReference();

        // 赋值操作
        AssetReference& operator=(const std::string& assetId);
        AssetReference& operator=(std::shared_ptr<AssetType> asset);

        // 访问操作
        std::shared_ptr<AssetType> Get() const;
        std::shared_ptr<AssetType> operator->() const;
        AssetType& operator*() const;

        // 状态查询
        bool IsValid() const;
        bool IsLoaded() const;
        const std::string& GetAssetId() const { return m_assetId; }

        // 序列化
        rapidjson::Value Serialize(rapidjson::Document::AllocatorType& allocator) const;
        void Deserialize(const rapidjson::Value& json);

    private:
        std::string m_assetId;
        mutable std::weak_ptr<AssetType> m_cachedAsset;

        std::shared_ptr<AssetType> LoadAsset() const;
    };
}
```

## 5. 组件重构

### 5.1 MeshFilter 重构

```cpp
namespace framework {
    class MeshFilter : public ComponentBase<MeshFilter> {
    public:
        MeshFilter(GameObject* owner);
        ~MeshFilter() = default;

        // 使用AssetReference
        void SetMesh(const std::string& meshAssetId);
        void SetMesh(std::shared_ptr<MeshAsset> meshAsset);

        std::shared_ptr<MeshAsset> GetMeshAsset() const;
        std::shared_ptr<Mesh> GetMesh() const; // 便利方法

        bool HasMesh() const;

        // 序列化
        void Deserialize(const rapidjson::Value& json) override;
        rapidjson::Value Serialize(rapidjson::Document::AllocatorType& allocator) const override;

    private:
        AssetReference<MeshAsset> m_meshAsset;
    };
}
```

### 5.2 Material 重构

```cpp
namespace framework {
    class Material {
    public:
        // 纹理管理改为使用AssetReference
        void SetTexture(const std::string& name, const std::string& textureAssetId);
        void SetTexture(const std::string& name, std::shared_ptr<TextureAsset> textureAsset);

        std::shared_ptr<TextureAsset> GetTextureAsset(const std::string& name) const;
        std::shared_ptr<Texture> GetTexture(const std::string& name) const; // 便利方法

        // 获取所有纹理引用
        std::vector<TextureBinding> GetAllTextureBindings() const;

        // 序列化支持
        rapidjson::Value Serialize(rapidjson::Document::AllocatorType& allocator) const;
        void Deserialize(const rapidjson::Value& json);

    private:
        struct TextureBinding {
            std::string name;
            AssetReference<TextureAsset> textureAsset;
            int slot;
            TextureType type;
        };

        std::vector<TextureBinding> m_textureBindings;
    };
}
```

## 6. Editor 拖拽系统

### 6.1 AssetDragDrop 系统

```cpp
namespace editor {
    enum class DragDropType {
        Asset,
        TextureAsset,
        MeshAsset,
        MaterialAsset
    };

    struct AssetDragDropPayload {
        DragDropType type;
        std::string assetId;
        framework::AssetType assetType;
        void* thumbnailTextureId;
    };

    class AssetDragDropSystem {
    public:
        // 开始拖拽Asset
        static void BeginAssetDrag(const std::string& assetId, framework::AssetType assetType);

        // 接受Asset拖拽
        static bool AcceptAssetDrop(framework::AssetType expectedType, std::string& outAssetId);

        // 检查是否可以接受拖拽
        static bool CanAcceptAssetType(framework::AssetType dragType, framework::AssetType targetType);

        // 渲染拖拽预览
        static void RenderDragPreview(const AssetDragDropPayload& payload);
    };
}
```

### 6.2 AssetField 通用组件

```cpp
namespace editor {
    template<typename AssetType>
    class AssetField {
    public:
        static bool Render(const std::string& label,
                          framework::AssetReference<AssetType>& assetRef,
                          framework::AssetType assetType,
                          const ImVec2& previewSize = ImVec2(64, 64));

    private:
        static void RenderAssetPreview(std::shared_ptr<AssetType> asset, const ImVec2& size);
        static void RenderAssetSelector(framework::AssetReference<AssetType>& assetRef,
                                       framework::AssetType assetType);
        static bool HandleAssetDragDrop(framework::AssetReference<AssetType>& assetRef,
                                       framework::AssetType assetType);
    };
}
```

### 6.3 Inspector 重构

```cpp
namespace editor {
    class MeshFilterInspector : public TypedComponentInspector<framework::MeshFilter> {
    public:
        void Inspect(framework::GameObject* node) override {
            MeshFilter* meshFilter = GetTypedComponent(node);
            if (!meshFilter) return;

            ImGui::Text("Mesh Filter");

            // 使用AssetField渲染Mesh选择
            auto meshAssetRef = meshFilter->GetMeshAssetReference();
            if (AssetField<framework::MeshAsset>::Render("Mesh", meshAssetRef, framework::AssetType::Mesh)) {
                // Asset已通过AssetField更新
            }
        }
    };

    class MeshRendererInspector : public TypedComponentInspector<framework::MeshRenderer> {
    public:
        void Inspect(framework::GameObject* node) override {
            MeshRenderer* meshRenderer = GetTypedComponent(node);
            if (!meshRenderer) return;

            ImGui::Text("Mesh Renderer");

            Material* material = meshRenderer->GetMaterial();
            if (material) {
                RenderMaterialEditor(material);
            }
        }

    private:
        void RenderMaterialEditor(framework::Material* material) {
            auto textureBindings = material->GetAllTextureBindings();

            for (auto& binding : textureBindings) {
                std::string label = binding.name + " (" + GetTextureTypeName(binding.type) + ")";

                if (AssetField<framework::TextureAsset>::Render(label, binding.textureAsset,
                                                               framework::AssetType::Texture)) {
                    // 纹理已通过AssetField更新
                }
            }
        }
    };
}
```

## 7. 实现计划

### 7.1 第一阶段：基础重构

1. **创建 TextureAsset 类**
2. **增强 MeshAsset 类**
3. **实现 AssetReference 模板**
4. **重构 AssetManager 以支持新的 Asset 类型**

### 7.2 第二阶段：组件重构

1. **重构 MeshFilter 使用 AssetReference**
2. **重构 Material 使用 TextureAsset**
3. **更新序列化系统**
4. **测试资源加载和引用**

### 7.3 第三阶段：Editor 集成

1. **实现 AssetDragDropSystem**
2. **创建 AssetField 通用组件**
3. **重构 Inspector 使用新的拖拽系统**
4. **更新 ContentBrowser 支持所有 Asset 类型**

### 7.4 第四阶段：优化和完善

1. **实现缩略图生成**
2. **添加 Asset 热重载支持**
3. **性能优化和内存管理**
4. **添加 Asset 验证和错误处理**

## 8. 优势分析

### 8.1 统一性

- 所有可拖拽资源都继承自 Asset
- 统一的序列化和加载机制
- 一致的拖拽体验

### 8.2 类型安全

- AssetReference 提供类型安全的资源引用
- 拖拽系统验证 Asset 类型匹配
- 编译时类型检查

### 8.3 可扩展性

- 新的 Asset 类型只需继承 Asset 基类
- AssetField 模板自动支持新 Asset 类型
- 插件式的 AssetLoader 系统

### 8.4 Editor 友好

- 统一的拖拽框架
- 自动的缩略图生成
- 直观的资源管理界面

这个重构方案将为你的引擎提供一个现代化、统一的 Asset 系统，支持完整的拖拽操作，并为未来的扩展打下良好基础。
