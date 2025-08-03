#include "Framework/Editor/Hierarchy.h"
#include "Framework/Core/SceneManager.h"
#include "Framework/Core/GameObject.h"
#include "Framework/Editor/ContextMenu.h"
#include "Framework/Editor/EditorContext.h"
#include "Framework/Editor/ObjectCreator.h"
namespace fk = framework;

namespace editor
{
    static bool showpop = false;
    static bool showEmptyAreaPop = false; // 新增：空白区域右键菜单标志
    static bool showRenamePopup = false;  // 新增：重命名弹窗标志
    static int index = -1;
    static int selectIndex = -1;
    static char renameBuffer[256] = ""; // 重命名输入缓冲区
    HierarchyWidget::HierarchyWidget()
        : EditorWidget("Hierarchy")
    {
    }

    HierarchyWidget::~HierarchyWidget()
    {
    }

    void HierarchyWidget::Initialize()
    {
        contextMenu = new ContextMenu("HierarchyContextMenu");

        // 选中物体的右键菜单
        contextMenu->AddAction("Delete", [=](void *)
                               {
            // TODO: 删除选中的GameObject
            auto activeScene = fk::SceneManager::GetInstance().GetActiveScene();
            if (activeScene && index >= 0) {
                auto childs = activeScene->GetAllGameObjects();
                if (index < childs.size()) {
                    activeScene->RemoveGameObjectFromEditor(childs[index]);
                }
            }
            showpop = false; }, 0);

        contextMenu->AddAction("Rename", [=](void *)
                               {
            // 重命名选中的GameObject
            auto activeScene = fk::SceneManager::GetInstance().GetActiveScene();
            if (activeScene && index >= 0) {
                auto childs = activeScene->GetAllGameObjects();
                if (index < childs.size()) {
                    // 将当前名称复制到输入缓冲区
                    std::string currentName = childs[index]->GetName();
                    strncpy(renameBuffer, currentName.c_str(), sizeof(renameBuffer) - 1);
                    renameBuffer[sizeof(renameBuffer) - 1] = '\0';

                    showRenamePopup = true;
                }
            }
            showpop = false; }, 0);

        // 空白区域的右键菜单 - 创建新的上下文菜单
        emptyAreaContextMenu = new ContextMenu("EmptyAreaContextMenu");
        emptyAreaContextMenu->AddAction("Create Cube", [=](void *)
                                        {
            ObjectCreator::CreateCube("New Cube");
        }, 0);

        emptyAreaContextMenu->AddAction("Create GameObject", [=](void *)
                                        {
            // 创建空的GameObject
            auto activeScene = fk::SceneManager::GetInstance().GetActiveScene();
            if (activeScene) {
                auto newObj = new fk::GameObject("New GameObject");
                activeScene->AddGameObjectFromEditor(newObj);
            }
            showEmptyAreaPop = false; }, 0);
    }

    void HierarchyWidget::Update(float deltaTime)
    {
    }

    void HierarchyWidget::Render()
    {
        auto activeScene = fk::SceneManager::GetInstance().GetActiveScene();
        if (activeScene == nullptr)
            return;
        auto childs = activeScene->GetAllGameObjects();

        static ImGuiIO &io = ImGui::GetIO();
        ImGui::Begin(name.c_str());

        // 检测空白区域右键点击
        bool isListBoxHovered = false;

        if (ImGui::BeginListBox("##listbox 2", ImVec2(-FLT_MIN, 20 * ImGui::GetTextLineHeightWithSpacing())))
        {
            bool clickedOnItem = false;

            for (int n = 0; n < childs.size(); n++)
            {
                const bool is_selected = (selectIndex == n);
                if (ImGui::Selectable((childs[n]->GetName() + "##" + std::to_string(n)).c_str(), is_selected, ImGuiButtonFlags_MouseButtonRight | ImGuiButtonFlags_MouseButtonLeft))
                {
                    selectIndex = n;
                    EditorContext::GetInstance().SetSelectedGameObject(childs[n]);
                }

                if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
                {
                    Logger::Debug("HierarchyWidget::Render: Right click on item {}", n);
                    showpop = true;
                    showEmptyAreaPop = false; // 确保只显示一个菜单
                    index = n;
                    clickedOnItem = true;
                }

                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            {
                // 这里判断是否有任何的item被选中
                if (selectIndex >= 0 && selectIndex < childs.size() && clickedOnItem)
                {
                    showpop = true;
                }
                else
                {
                    showpop = false; // 如果没有选中任何项，则隐藏菜单
                }
            }

            ImGui::EndListBox();

            // 检测空白区域右键点击 - 移到 EndListBox 之后
            if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right) && !clickedOnItem)
            {
                showEmptyAreaPop = true;
                showpop = false; // 确保只显示一个菜单
            }
        }

        // 显示选中物体的右键菜单
        if (showpop)
            ImGui::OpenPopup("HierarchyContextMenu");
        if (contextMenu)
        {
            contextMenu->Render();
        }

        // 显示空白区域的右键菜单
        if (showEmptyAreaPop)
            ImGui::OpenPopup("EmptyAreaContextMenu");
        if (emptyAreaContextMenu)
        {
            emptyAreaContextMenu->Render();
        }

        // 点击其他地方关闭菜单
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGui::IsAnyItemHovered() && !ImGui::IsPopupOpen("", ImGuiPopupFlags_AnyPopupId))
        {
            showpop = false;
            showEmptyAreaPop = false;
        }

        // 检查菜单是否被关闭
        if (!ImGui::IsPopupOpen("HierarchyContextMenu"))
        {
            showpop = false;
        }
        if (!ImGui::IsPopupOpen("EmptyAreaContextMenu"))
        {
            showEmptyAreaPop = false;
        }

        // 重命名弹窗
        if (showRenamePopup)
        {
            ImGui::OpenPopup("Rename GameObject");
            showRenamePopup = false; // 只在第一帧打开，避免重复调用
        }

        // 重命名模态对话框
        if (ImGui::BeginPopupModal("Rename GameObject", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Enter new name:");
            ImGui::Separator();

            // 输入框，只在第一次显示时自动聚焦
            static bool firstFrame = true;
            if (firstFrame)
            {
                ImGui::SetKeyboardFocusHere();
                firstFrame = false;
            }

            bool enterPressed = ImGui::InputText("##rename", renameBuffer, sizeof(renameBuffer), ImGuiInputTextFlags_EnterReturnsTrue);

            ImGui::Separator();

            // 按钮
            if (ImGui::Button("OK", ImVec2(120, 0)) || enterPressed)
            {
                // 应用重命名
                auto activeScene = fk::SceneManager::GetInstance().GetActiveScene();
                if (activeScene && index >= 0)
                {
                    auto childs = activeScene->GetAllGameObjects();
                    if (index < childs.size() && strlen(renameBuffer) > 0)
                    {
                        childs[index]->SetName(std::string(renameBuffer));
                        Logger::Log("GameObject renamed to: {}", renameBuffer);
                    }
                }
                firstFrame = true; // 重置标志为下次使用
                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine();

            if (ImGui::Button("Cancel", ImVec2(120, 0)))
            {
                firstFrame = true; // 重置标志为下次使用
                ImGui::CloseCurrentPopup();
            }

            // ESC 键关闭
            if (ImGui::IsKeyPressed(ImGuiKey_Escape))
            {
                firstFrame = true; // 重置标志为下次使用
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        ImGui::End();
    }

    void HierarchyWidget::Shutdown()
    {
    }
} // namespace editor
