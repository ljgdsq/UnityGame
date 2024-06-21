//
// Created by zhengshulin on 2024/6/21.
//

#ifndef OPENGLPROJ_HIERARCHY_H
#define OPENGLPROJ_HIERARCHY_H


#include "EditorWidget.h"
#include <vector>
#include <string>
using std::vector;
using std::string;
class Hierarchy: public EditorWidget{
    vector<string> nodeNames;
    int selectIndex=-1;
public:
    Hierarchy(const std::string &name, EditorScene *scene);

    void Draw() override;
};


#endif //OPENGLPROJ_HIERARCHY_H
