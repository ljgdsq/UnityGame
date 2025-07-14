Editor 拖拽资源系统设计方案
1. 问题分析
当前系统存在以下问题：
我来分析当前的资源系统并设计一个支持拖拽操作的资源管理方案。

## 当前问题分析

让我先检查现有的 Asset 系统实现： 现在检查 Texture 和 Mesh 的具体实现： # Editor 拖拽资源系统设计方案

## 1. 问题分析

当前系统存在以下问题：
1. **Asset 系统与 Editor 分离**：Asset 管理和 Editor 操作没有良好集成
2. **缺乏拖拽接口**：没有统一的拖拽操作接口
3. **组件赋值复杂**：需要手动代码方式设置 Texture 和 Mesh
4. **缺乏可视化预览**：无法在 Inspector 中预览资源

## 2. 整体架构设计

```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   ContentBrowser│────│  AssetRegistry  │────│ DragDropSystem  │
│   (资源浏览器)   │    │   (资源注册)    │    │   (拖拽系统)    │
└─────────────────┘    └─────────────────┘    └─────────────────┘
         │                       │                       │
         └───────────────────────┼───────────────────────┘
                                 │
                    ┌─────────────────┐
                    │   Inspector     │
                    │  (检查器面板)    │
                    └─────────────────┘
```

## 3. 核心组件设计

### 3.1 AssetRegistry (资源注册中心)

```cpp
namespace editor {
    struct AssetInfo {
        std::string assetId;           // 唯一标识
        std::string displayName;       // 显示名称
        std::string filePath;         // 文件路径
        framework::AssetType type;    // 资源类型
        void* thumbnailTextureId;     // 缩略图纹理ID
        std::shared_ptr<framework::Asset> asset; // 资源实例
    };

    class AssetRegistry {
    public:
        static AssetRegistry& GetInstance();

        // 注册资源
        void RegisterAsset(const AssetInfo& info);
        void UnregisterAsset(const std::string& assetId);

        // 查询资源
        const AssetInfo* GetAssetInfo(const std::string& assetId) const;
        std::vector<AssetInfo> GetAssetsByType(framework::AssetType type) const;
        std::vector<AssetInfo> GetAllAssets() const;

        // 扫描文件夹并注册资源
        void ScanAssetsInDirectory(const std::string& directory);

    private:
        std::unordered_map<std::string, AssetInfo> m_assets;
    };
}
```

### 3.2 DragDropSystem (拖拽系统)

```cpp
namespace editor {
    enum class DragDropType {
        Asset,
        GameObject,
        Component
    };

    struct DragDropPayload {
        DragDropType type;
        std::string dataId;
        void* userData;
    };

    class DragDropSystem {
    public:
        // 开始拖拽
        static void BeginDragDrop(DragDropType type, const std::string& dataId, void* userData = nullptr);

        // 接受拖拽
        static bool AcceptDragDrop(DragDropType expectedType, DragDropPayload& payload);

        // 渲染拖拽源
        static void RenderDragSource(const std::string& label, DragDropType type, const std::string& dataId);

        // 渲染拖拽目标
        static bool RenderDropTarget(const std::string& label, DragDropType expectedType, DragDropPayload& payload);
    };
}
```

### 3.3 AssetField (资源字段组件)

```cpp
namespace editor {
    template<typename AssetType>
    class AssetField {
    public:
        // 渲染资源选择字段
        static bool Render(const std::string& label, std::shared_ptr<AssetType>& asset, framework::AssetType assetType);

        // 渲染资源预览
        static void RenderPreview(std::shared_ptr<AssetType> asset, const ImVec2& size = ImVec2(64, 64));

    private:
        static void RenderAssetSelector(std::shared_ptr<AssetType>& asset, framework::AssetType assetType);
        static void HandleDragDrop(std::shared_ptr<AssetType>& asset, framework::AssetType assetType);
    };
}
```

## 4. Inspector 集成

### 4.1 MeshRendererInspector 改进

```cpp
void MeshRendererInspector::RenderMaterial(framework::GameObject *node) {
    MeshRenderer* meshRenderer = GetTypedComponent(node);
    if (!meshRenderer) return;

    auto material = meshRenderer->GetMaterial();
    if (!material) return;

    ImGui::Text("Material Properties:");

    // 主纹理字段
    auto mainTexture = material->GetMainTexture();
    if (AssetField<framework::Texture>::Render("Main Texture", mainTexture, framework::AssetType::Texture)) {
        material->SetMainTexture(mainTexture);
    }

    // 法线贴图字段
    auto normalMap = material->GetNormalMap();
    if (AssetField<framework::Texture>::Render("Normal Map", normalMap, framework::AssetType::Texture)) {
        material->SetNormalMap(normalMap);
    }

    // 其他材质属性...
}
```

### 4.2 MeshFilter Inspector

```cpp
class MeshFilterInspector : public TypedComponentInspector<framework::MeshFilter> {
public:
    void Inspect(framework::GameObject *node) override {
        MeshFilter* meshFilter = GetTypedComponent(node);
        if (!meshFilter) return;

        ImGui::Text("Mesh Filter");

        // Mesh 资源字段
        auto mesh = meshFilter->GetMesh();
        if (AssetField<framework::Mesh>::Render("Mesh", mesh, framework::AssetType::Mesh)) {
            meshFilter->SetMesh(mesh);
        }
    }

    std::string GetComponentDisplayName() const override {
        return "Mesh Filter";
    }
};
```

## 5. ContentBrowser 改进

```cpp
class ContentBrowser : public EditorWidget {
private:
    void RenderAssetItem(const AssetInfo& assetInfo) {
        ImVec2 buttonSize(64, 64);

        // 渲染缩略图按钮
        if (ImGui::ImageButton(assetInfo.thumbnailTextureId, buttonSize)) {
            // 点击选择资源
            SetSelectedAsset(assetInfo.assetId);
        }

        // 拖拽源
        if (ImGui::BeginDragDropSource()) {
            ImGui::SetDragDropPayload("ASSET", &assetInfo.assetId, sizeof(std::string));
            ImGui::Text("Dragging %s", assetInfo.displayName.c_str());
            ImGui::EndDragDropSource();
        }

        // 显示资源名称
        ImGui::TextWrapped("%s", assetInfo.displayName.c_str());
    }
};
```

## 6. 实现优先级

### 第一阶段 - 基础框架
1. **实现 AssetRegistry**：资源注册和查询系统
2. **实现 DragDropSystem**：基础拖拽功能
3. **改进 AssetManager**：与 Editor 系统集成

### 第二阶段 - UI 组件
1. **实现 AssetField 模板**：通用资源选择组件
2. **改进 MeshRendererInspector**：支持纹理拖拽
3. **实现 MeshFilterInspector**：支持网格拖拽

### 第三阶段 - 高级功能
1. **缩略图生成**：自动生成资源预览
2. **资源热重载**：文件变化自动更新
3. **批量操作**：多选和批量赋值

## 7. 关键技术点

### 7.1 ImGui 拖拽实现
```cpp
// 拖拽源
if (ImGui::BeginDragDropSource()) {
    ImGui::SetDragDropPayload("ASSET_TEXTURE", &assetId, sizeof(std::string));
    ImGui::EndDragDropSource();
}

// 拖拽目标
if (ImGui::BeginDragDropTarget()) {
    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_TEXTURE")) {
        std::string assetId = *(const std::string*)payload->Data;
        // 处理拖拽数据
    }
    ImGui::EndDragDropTarget();
}
```

### 7.2 资源预览
```cpp
void RenderTexturePreview(std::shared_ptr<framework::Texture> texture, ImVec2 size) {
    if (texture && texture->IsLoaded()) {
        ImGui::Image((void*)(intptr_t)texture->GetTextureID(), size);
    } else {
        // 渲染默认占位符
        ImGui::Dummy(size);
        ImGui::SameLine(0, 0);
        ImGui::Text("No Texture");
    }
}
```

这个设计方案将大大改善资源管理的用户体验，使得在 Editor 中操作资源变得直观和高效。你觉得这个方案如何？有什么需要调整或补充的地方吗？
