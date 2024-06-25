//
// Created by zhengshulin on 2024/6/25.
//

#ifndef OPENGLPROJ_CONTEXTMENU_H
#define OPENGLPROJ_CONTEXTMENU_H

#include <string>
#include <functional>
#include <map>
#include <vector>
using std::vector;
using std::map;
using std::function;
using std::string;
using ActionType=function<void(void*)>;

class ContextMenuItem{
public:
    string name;
    int order;
    ActionType action;
};

class ContextMenu {
    vector<ContextMenuItem> actions;
    bool shouldSort= false;
    string name;
public:
    explicit ContextMenu(const string &name);

public:
    void AddAction(string name,ActionType action,int order=0);
    void Draw();
};


#endif //OPENGLPROJ_CONTEXTMENU_H
