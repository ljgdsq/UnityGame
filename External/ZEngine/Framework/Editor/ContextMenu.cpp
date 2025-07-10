#include "imgui.h"
#include "Framework/Editor/ContextMenu.h"
#include <algorithm>
using namespace std;
namespace editor
{

    void ContextMenu::AddAction(string name, ActionType action, int order)
    {
        actions.push_back({name, order, action});
        shouldSort = true;
    }

    void ContextMenu::Render()
    {
        if (shouldSort)
        {
            shouldSort = false;
            std::sort(actions.begin(), actions.end(), [](const ContextMenuItem &a, const ContextMenuItem &b)
                      { return a.order < b.order; });
        }

        if (ImGui::BeginPopupContextWindow(name.c_str()))
        {
            for (const auto &item : actions)
            {
                if (ImGui::MenuItem(item.name.c_str()))
                {
                    item.action(nullptr);
                }
            }
            ImGui::EndPopup();
        }
    }

    ContextMenu::ContextMenu(const string &name) : name(name) {}

}