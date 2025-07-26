#include "Framework/Editor/AssetDragDropSystem.h"
#include "Framework/Asset/Asset.h"
#include "Framework/Asset/AssetManager.h"
#include "Framework/Log/Logger.h"
#include <cstring>
#include "Framework/Editor/EditorContext.h"

namespace editor
{

    // 拖拽载荷类型字符串
    const char *AssetDragDropSystem::GetPayloadTypeString(DragDropType type)
    {
        switch (type)
        {
        case DragDropType::Asset:
            return "DND_ASSET";
        case DragDropType::Texture:
            return "DND_TEXTURE";
        case DragDropType::GameObject:
            return "DND_GAMEOBJECT";
        case DragDropType::Component:
            return "DND_COMPONENT";
        case DragDropType::File:
            return "DND_FILE";
        default:
            return "DND_UNKNOWN";
        }
    }

    void AssetDragDropSystem::BeginDragDrop(DragDropType type, const std::string &dataId,
                                            const std::string &displayName, void *userData)
    {
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
        {
            DragDropPayload payload;
            payload.SetPayLoad(type, dataId, displayName, userData);
            SetupDragDropPayload(payload);
            RenderDragPreview(payload.displayName, nullptr);

            ImGui::EndDragDropSource();
        }
    }

    bool AssetDragDropSystem::AcceptDragDrop(DragDropType expectedType, DragDropPayload &payload)
    {
        if (ImGui::BeginDragDropTarget())
        {
            const char *payloadType = GetPayloadTypeString(expectedType);
            if (const ImGuiPayload *imguiPayload = ImGui::AcceptDragDropPayload(payloadType))
            {
                bool success = ReadDragDropPayload(payloadType, payload);
                ImGui::EndDragDropTarget();
                return success;
            }
            ImGui::EndDragDropTarget();
        }
        return false;
    }

    bool AssetDragDropSystem::IsAcceptableDragDrop(DragDropType expectedType)
    {
        const char *payloadType = GetPayloadTypeString(expectedType);
        return ImGui::BeginDragDropTarget() &&
               ImGui::GetDragDropPayload() &&
               ImGui::GetDragDropPayload()->IsDataType(payloadType);
    }

    bool AssetDragDropSystem::RenderDragSource(const std::string &label, DragDropType type,
                                               const std::string &dataId,
                                               const std::string &displayName,
                                               void *thumbnailTextureId,
                                               const ImVec2 &size)
    {
        // 渲染缩略图按钮
        bool clicked = false;
        if (thumbnailTextureId)
        {
            clicked = ImGui::ImageButton(thumbnailTextureId, size);
        }
        else
        {
            clicked = ImGui::Button(label.c_str(), size);
        }

        // 设置拖拽源
        if (type != DragDropType::Unknown && ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
        {
            clicked = false;
            DragDropPayload payload;
            payload.SetPayLoad(type, dataId, displayName, thumbnailTextureId);

            SetupDragDropPayload(payload);
            RenderDragPreview(payload.displayName, thumbnailTextureId);
            EditorContext::GetInstance().isDrag= true;
            ImGui::EndDragDropSource();
        }

        // 工具提示
        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip("%s", displayName.empty() ? label.c_str() : displayName.c_str());
        }

        return clicked;
    }

    bool AssetDragDropSystem::RenderDropTarget(const std::string &hint, DragDropType expectedType,
                                               DragDropPayload &payload, const ImVec2 &size)
    {
        // 渲染拖拽目标区域
        ImVec2 actualSize = size;
        if (actualSize.x <= 0)
            actualSize.x = ImGui::GetContentRegionAvail().x;
        if (actualSize.y <= 0)
            actualSize.y = 32.0f;

        ImVec2 cursorPos = ImGui::GetCursorPos();
        ImGui::Dummy(actualSize);

        // 检查是否有拖拽操作
        bool accepted = false;
        if (ImGui::BeginDragDropTarget())
        {
            RenderDropTargetHighlight(actualSize);

            const char *payloadType = GetPayloadTypeString(expectedType);
            if (const ImGuiPayload *imguiPayload = ImGui::AcceptDragDropPayload(payloadType))
            {
                accepted = ReadDragDropPayload(payloadType, payload);
            }

            ImGui::EndDragDropTarget();
        }

        // 渲染提示文本
        ImGui::SetCursorPos(cursorPos);
        ImVec2 textSize = ImGui::CalcTextSize(hint.c_str());
        ImVec2 textPos = ImVec2(
            cursorPos.x + (actualSize.x - textSize.x) * 0.5f,
            cursorPos.y + (actualSize.y - textSize.y) * 0.5f);
        ImGui::SetCursorPos(textPos);
        ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "%s", hint.c_str());

        return accepted;
    }

    void AssetDragDropSystem::RenderDragPreview(const std::string &displayName,
                                                void *thumbnailTextureId)
    {
        if (thumbnailTextureId)
        {
            ImGui::Image(thumbnailTextureId, ImVec2(32, 32));
            ImGui::SameLine();
        }
        ImGui::Text("%s", displayName.c_str());
    }

    bool AssetDragDropSystem::IsCompatibleType(DragDropType dragType, DragDropType dropType)
    {
        return dragType == dropType;
    }

    void AssetDragDropSystem::SetupDragDropPayload(const DragDropPayload &payload)
    {
        const char *payloadType = GetPayloadTypeString(payload.type);
        ImGui::SetDragDropPayload(payloadType, &payload, sizeof(DragDropPayload));
    }

    bool AssetDragDropSystem::ReadDragDropPayload(const char *payloadType, DragDropPayload &payload)
    {
        const ImGuiPayload *imguiPayload = ImGui::GetDragDropPayload();
        if (imguiPayload && imguiPayload->IsDataType(payloadType))
        {
            memcpy(&payload, imguiPayload->Data, sizeof(DragDropPayload));
            return payload.IsValid();
        }
        return false;
    }

    void AssetDragDropSystem::RenderDropTargetHighlight(const ImVec2 &size)
    {
        ImDrawList *drawList = ImGui::GetWindowDrawList();
        ImVec2 p_min = ImGui::GetItemRectMin();
        ImVec2 p_max = ImGui::GetItemRectMax();

        // 绘制高亮边框
        drawList->AddRect(p_min, p_max, IM_COL32(100, 150, 255, 255), 2.0f, 0, 2.0f);

        // 绘制半透明背景
        drawList->AddRectFilled(p_min, p_max, IM_COL32(100, 150, 255, 50), 2.0f);
    }

    void AssetDragDropSystem::RenderDragSourceHighlight()
    {
        ImDrawList *drawList = ImGui::GetWindowDrawList();
        ImVec2 p_min = ImGui::GetItemRectMin();
        ImVec2 p_max = ImGui::GetItemRectMax();

        // 绘制拖拽源高亮
        drawList->AddRect(p_min, p_max, IM_COL32(255, 255, 100, 255), 2.0f, 0, 1.0f);
    }

    // 辅助函数，获取 AssetManager 实例
    framework::AssetManager &GetAssetManager()
    {
        return framework::AssetManager::GetInstance();
    }

} // namespace editor
