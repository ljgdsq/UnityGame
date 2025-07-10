#pragma once
#include <string>
#include <functional>
#include <map>
#include <vector>


using ActionType = std::function<void(void *)>;
namespace editor
{

    class ContextMenuItem
    {
    public:
        std::string name;
        int order;
        ActionType action;
    };

    class ContextMenu
    {
         std::vector<ContextMenuItem> actions;
        bool shouldSort = false;
         std::string name;

    public:
        explicit ContextMenu(const std::string &name);

    public:
        void AddAction( std::string name, ActionType action, int order = 0);
        void Render();
    };
}