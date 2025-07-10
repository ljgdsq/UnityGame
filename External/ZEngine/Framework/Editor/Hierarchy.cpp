#include "Framework/Editor/Hierarchy.h"
#include "Framework/Core/SceneManager.h"
#include "Framework/Core/GameObject.h"
#include "Framework/Editor/ContextMenu.h"
namespace fk = framework;
static bool showpop= false;
static int index=-1;
static    int selectIndex=-1;
namespace editor
{
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
        //     contextMenu->AddAction("Delete", [=](void *)
        //                            {
        //     this->scene->getRunningScene()->DeleteChild(index);
        //     showpop = false;
        // }, 0);

        contextMenu->AddAction("Add", [=](void *)
                               {
        if (ImGui::MenuItem("zz")){

        }
        if (ImGui::MenuItem("cc")){

        } }, 0);
    }

    void HierarchyWidget::Update(float deltaTime)
    {
    }

    void HierarchyWidget::Render()
    {
        auto activeScene = fk::SceneManager::GetInstance().GetActiveScene();
        if (activeScene == nullptr)return;
        auto childs = activeScene->GetAllGameObjects();

        static ImGuiIO &io = ImGui::GetIO();
        ImGui::Begin(name.c_str());
        if (ImGui::BeginListBox("##listbox 2", ImVec2(-FLT_MIN, 20 * ImGui::GetTextLineHeightWithSpacing())))
        {
            for (int n = 0; n < childs.size(); n++)
            {
                const bool is_selected = (selectIndex == n);
                if (ImGui::Selectable((childs[n]->GetName() + "##" + std::to_string(n)).c_str(), is_selected, ImGuiButtonFlags_MouseButtonRight | ImGuiButtonFlags_MouseButtonLeft))
                {
                    selectIndex = n;
                    // scene->InspectNode(childs[n]);
                }

                if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
                {
                    showpop = true;
                    index = n;
                }

                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndListBox();
        }

        if (showpop)
            ImGui::OpenPopup("HierarchyContextMenu");
        if (contextMenu)
        {
            contextMenu->Render();
        }
        if ((ImGui::IsMouseClicked(ImGuiMouseButton_Left) || ImGui::IsMouseClicked(ImGuiMouseButton_Right)) && !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
        {
            showpop = false;
        }
        ImGui::End();
    }

    void HierarchyWidget::Shutdown()
    {
    }
} // namespace editor
