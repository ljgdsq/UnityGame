#pragma once
#include "Framework/Editor/AssetDragDropSystem.h"
#include "Framework/Asset/AssetManager.h"
#include "Framework/Log/Logger.h"
#include "imgui.h"
#include <memory>
#include <string>
#include <vector>
#include <cstring>

namespace editor
{

    // 资源选择器配置
    struct AssetFieldConfig
    {
        bool showPreview = true;             // 显示预览图
        bool allowNull = true;               // 允许空值
        bool showClearButton = true;         // 显示清除按钮
        bool showSelectButton = true;        // 显示选择按钮
        ImVec2 previewSize = ImVec2(64, 64); // 预览图尺寸
        ImVec2 buttonSize = ImVec2(120, 20); // 按钮尺寸
        std::string nullText = "None";       // 空值显示文本
        std::string selectText = "Select";   // 选择按钮文本
        std::string clearText = "Clear";     // 清除按钮文本
    };

    // 通用资源字段模板
    template <typename AssetType>
    class AssetField
    {
    public:
        // 渲染资源字段（返回true表示值有变化）
        static bool Render(const std::string &label,
                           std::shared_ptr<AssetType> &asset,
                           const AssetFieldConfig &config = AssetFieldConfig());

        // 渲染资源引用字段
        static bool Render(const std::string &label,
                           framework::AssetReference<AssetType> &assetRef,
                           const AssetFieldConfig &config = AssetFieldConfig());

        // 渲染只读资源字段
        static void RenderReadOnly(const std::string &label,
                                   std::shared_ptr<AssetType> asset,
                                   const AssetFieldConfig &config = AssetFieldConfig());

        // 渲染资源预览
        static bool RenderPreview(std::shared_ptr<AssetType> &asset,
                                  const ImVec2 &size = ImVec2(64, 64));

        // 渲染资源信息
        static void RenderAssetInfo(std::shared_ptr<AssetType> asset);

    private:
        // 内部渲染方法
        static bool RenderAssetSelector(const std::string &label,
                                        std::shared_ptr<AssetType> &asset,
                                        const AssetFieldConfig &config);

        static bool RenderAssetButton(const std::string &label,
                                      std::shared_ptr<AssetType> asset,
                                      const AssetFieldConfig &config);

        static bool RenderDropTarget(std::shared_ptr<AssetType> &asset,
                                     const ImVec2 &size);

        static bool ShowAssetSelectionDialog(std::shared_ptr<AssetType> &asset);

        static void RenderAssetThumbnail(std::shared_ptr<AssetType> asset,
                                         const ImVec2 &size);

        static std::string GetAssetTypeName();
        static framework::AssetType GetAssetType();
    };

    // 模板实现
    template <typename AssetType>
    bool AssetField<AssetType>::Render(const std::string &label,
                                       std::shared_ptr<AssetType> &asset,
                                       const AssetFieldConfig &config)
    {
        bool changed = false;

        ImGui::PushID(label.c_str());

        // 渲染标签
        ImGui::Text("%s", label.c_str());

        // 渲染预览图和按钮
        if (config.showPreview)
        {
            // 预览区域
            ImVec2 previewPos = ImGui::GetCursorPos();
            if (RenderPreview(asset, config.previewSize))
            {
                changed = true;
            }
        }

        // 按钮区域
        ImGui::SameLine();
        ImGui::BeginGroup();

        // 资源信息
        if (asset)
        {
            ImGui::Text("%s", asset->GetName().c_str());
            if (config.showClearButton)
            {
                if (ImGui::Button(config.clearText.c_str(), config.buttonSize))
                {
                    asset.reset();
                    changed = true;
                }
            }
        }
        else
        {
            ImGui::Text("%s", config.nullText.c_str());
        }

        // 选择按钮
        if (config.showSelectButton)
        {
            if (ImGui::Button(config.selectText.c_str(), config.buttonSize))
            {
                if (ShowAssetSelectionDialog(asset))
                {
                    changed = true;
                }
            }
        }

        ImGui::EndGroup();

        ImGui::PopID();
        return changed;
    }



    template <typename AssetType>
    void AssetField<AssetType>::RenderReadOnly(const std::string &label,
                                               std::shared_ptr<AssetType> asset,
                                               const AssetFieldConfig &config)
    {
        ImGui::PushID(label.c_str());

        ImGui::Text("%s", label.c_str());

        if (config.showPreview)
        {
            // 对于只读模式，我们需要创建一个临时的可修改版本
            auto tempAsset = asset;
            RenderPreview(tempAsset, config.previewSize);
            ImGui::SameLine();
        }

        if (asset)
        {
            ImGui::Text("%s", asset->GetName().c_str());
        }
        else
        {
            ImGui::Text("%s", config.nullText.c_str());
        }

        ImGui::PopID();
    }

    template <typename AssetType>
    bool AssetField<AssetType>::RenderPreview(std::shared_ptr<AssetType> &asset,
                                              const ImVec2 &size)
    {
        bool changed = false;

        if (asset && asset->HasThumbnail())
        {
            void *thumbnailId = asset->GetThumbnailTextureId();
            ImGui::Image(thumbnailId, size);
        }
        else
        {
            // 渲染默认预览
            ImGui::Button("?", size);
        }

        // 在渲染的元素上立即检查拖拽目标
        // 这必须在 UI 元素渲染后立即调用
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("DND_ASSET"))
            {
                // 处理拖拽载荷
                editor::DragDropPayload dragPayload;
                memcpy(&dragPayload, payload->Data, sizeof(DragDropPayload));

                // 通过 AssetManager 获取资源
                auto &assetManager = framework::AssetManager::GetInstance();
                auto draggedAsset = assetManager.GetAsset(dragPayload.dataId);

                if (draggedAsset && draggedAsset->GetType() == GetAssetType())
                {
                    // 类型匹配，可以接受拖拽
                    asset = std::static_pointer_cast<AssetType>(draggedAsset);
                    changed = true;
                    Logger::Debug("Asset drag-drop successful: {} -> {}", dragPayload.dataId, asset->GetName());
                }
            }
            ImGui::EndDragDropTarget();
        }

        return changed;
    }

    template <typename AssetType>
    void AssetField<AssetType>::RenderAssetInfo(std::shared_ptr<AssetType> asset)
    {
        if (!asset)
            return;

        ImGui::Text("Name: %s", asset->GetName().c_str());
        ImGui::Text("Type: %s", GetAssetTypeName().c_str());
        ImGui::Text("Size: %ld bytes", asset->GetSize());
        ImGui::Text("Loaded: %s", asset->IsLoaded() ? "Yes" : "No");
    }

    template <typename AssetType>
    bool AssetField<AssetType>::RenderDropTarget(std::shared_ptr<AssetType> &asset,
                                                 const ImVec2 &size)
    {
        return AssetDragDropHelper::RenderAssetDropTarget(
            "Drag " + GetAssetTypeName() + " here", asset, size);
    }

    template <typename AssetType>
    bool AssetField<AssetType>::ShowAssetSelectionDialog(std::shared_ptr<AssetType> &asset)
    {
        // 简化实现：显示一个弹出窗口列出所有可用资源
        static bool showDialog = true;
        bool selected = false;

        if (showDialog)
        {
            ImGui::OpenPopup("Asset Selection");
            showDialog = false;
        }

        if (ImGui::BeginPopupModal("Asset Selection", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Select %s:", GetAssetTypeName().c_str());
            ImGui::Separator();

            // 获取所有同类型资源
            auto &assetManager = framework::AssetManager::GetInstance();
            auto assetNames = assetManager.GetAllAssetNames();

            for (const auto &name : assetNames)
            {
                auto testAsset = assetManager.GetAsset(name);
                if (testAsset && testAsset->GetType() == GetAssetType())
                {
                    if (ImGui::Selectable(name.c_str()))
                    {
                        asset = std::static_pointer_cast<AssetType>(testAsset);
                        selected = true;
                        ImGui::CloseCurrentPopup();
                    }
                }
            }

            ImGui::Separator();

            if (ImGui::Button("Cancel"))
            {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        return selected;
    }

    // // 特化方法需要在 cpp 文件中实现
    // template <typename AssetType>
    // std::string AssetField<AssetType>::GetAssetTypeName();

    // template <typename AssetType>
    // framework::AssetType AssetField<AssetType>::GetAssetType();

} // namespace editor
