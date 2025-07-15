#pragma once
#include "imgui.h"
#include "Framework/Asset/AssetManager.h"
#include <string>
#include <memory>
#include <functional>

namespace framework
{
    class Asset;
    class AssetManager;
    template <typename T>
    class AssetReference;
}

namespace editor
{

    // 拖拽数据类型
    enum class DragDropType
    {
        Asset,
        GameObject,
        Component,
        File
    };

    // 拖拽载荷数据结构
    struct DragDropPayload
    {
        DragDropType type;
        std::string dataId;       // 资源ID或其他标识符
        std::string displayName;  // 显示名称
        void *userData = nullptr; // 用户自定义数据

        // 便利方法
        bool IsAssetType() const { return type == DragDropType::Asset; }
        bool IsValid() const { return !dataId.empty(); }
    };

    // 拖拽系统管理器
    class AssetDragDropSystem
    {
    public:
        // 开始拖拽操作
        static void BeginDragDrop(DragDropType type, const std::string &dataId,
                                  const std::string &displayName = "",
                                  void *userData = nullptr);

        // 接受拖拽操作
        static bool AcceptDragDrop(DragDropType expectedType, DragDropPayload &payload);

        // 检查拖拽是否可接受
        static bool IsAcceptableDragDrop(DragDropType expectedType);

        // 渲染拖拽源（带图标和文本）
        static void RenderDragSource(const std::string &label, DragDropType type,
                                     const std::string &dataId,
                                     const std::string &displayName = "",
                                     void *thumbnailTextureId = nullptr,
                                     const ImVec2 &size = ImVec2(64, 64));

        // 渲染拖拽目标区域
        static bool RenderDropTarget(const std::string &hint, DragDropType expectedType,
                                     DragDropPayload &payload,
                                     const ImVec2 &size = ImVec2(0, 0));

        // 渲染拖拽预览
        static void RenderDragPreview(const std::string &displayName,
                                      void *thumbnailTextureId = nullptr);

        // 设置拖拽载荷类型标识符
        static const char *GetPayloadTypeString(DragDropType type);

        // 验证拖拽类型兼容性
        static bool IsCompatibleType(DragDropType dragType, DragDropType dropType);

    private:
        // 内部工具函数
        static void SetupDragDropPayload(const DragDropPayload &payload);
        static bool ReadDragDropPayload(const char *payloadType, DragDropPayload &payload);

        // 视觉反馈
        static void RenderDropTargetHighlight(const ImVec2 &size);
        static void RenderDragSourceHighlight();
    };

    // 专用于资源拖拽的辅助类
    class AssetDragDropHelper
    {
    public:
        // 开始资源拖拽
        template <typename AssetType>
        static void BeginAssetDrag(std::shared_ptr<AssetType> asset);

        template <typename AssetType>
        static void BeginAssetDrag(const framework::AssetReference<AssetType> &assetRef);

        // 接受资源拖拽
        template <typename AssetType>
        static bool AcceptAssetDrop(std::shared_ptr<AssetType> &asset);

        template <typename AssetType>
        static bool AcceptAssetDrop(framework::AssetReference<AssetType> &assetRef);

        // 渲染资源拖拽源
        template <typename AssetType>
        static void RenderAssetDragSource(const std::string &label,
                                          std::shared_ptr<AssetType> asset,
                                          const ImVec2 &size = ImVec2(64, 64))
        {
            if (!asset)
            {
                ImGui::Button(("No " + label).c_str(), size);
                return;
            }

            void *thumbnailId = asset->GetThumbnailTextureId();
            AssetDragDropSystem::RenderDragSource(label, DragDropType::Asset,
                                                  asset->GetAssetId(), asset->GetName(),
                                                  thumbnailId, size);
        }

        // 渲染资源拖拽目标
        template <typename AssetType>
        static bool RenderAssetDropTarget(const std::string &hint,
                                          std::shared_ptr<AssetType> &asset,
                                          const ImVec2 &size = ImVec2(0, 0))
        {
            DragDropPayload payload;
            return AssetDragDropSystem::RenderDropTarget(hint, DragDropType::Asset, payload, size) &&
                   AcceptAssetDrop(asset);
        }
    };

} // namespace editor

// 模板实现的显式实例化声明
namespace editor
{
    template <typename AssetType>
    void AssetDragDropHelper::BeginAssetDrag(std::shared_ptr<AssetType> asset)
    {
        if (!asset)
            return;

        AssetDragDropSystem::BeginDragDrop(DragDropType::Asset,
                                           asset->GetAssetId(),
                                           asset->GetName());
    }

    template <typename AssetType>
    void AssetDragDropHelper::BeginAssetDrag(const framework::AssetReference<AssetType> &assetRef)
    {
        if (auto asset = assetRef.Get())
        {
            BeginAssetDrag(asset);
        }
    }

    template <typename AssetType>
    bool AssetDragDropHelper::AcceptAssetDrop(std::shared_ptr<AssetType> &asset)
    {
        DragDropPayload payload;
        if (AssetDragDropSystem::AcceptDragDrop(DragDropType::Asset, payload))
        {
            if (payload.IsAssetType())
            {
                // 直接使用 AssetManager 的 GetInstance 方法
                auto &assetManager = framework::AssetManager::GetInstance();
                asset = assetManager.LoadAsset<AssetType>(payload.dataId);
                return asset != nullptr;
            }
        }
        return false;
    }

    template <typename AssetType>
    bool AssetDragDropHelper::AcceptAssetDrop(framework::AssetReference<AssetType> &assetRef)
    {
        std::shared_ptr<AssetType> asset;
        if (AcceptAssetDrop(asset))
        {
            assetRef.Set(asset);
            return true;
        }
        return false;
    }
}
