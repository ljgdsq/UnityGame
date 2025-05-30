//
// Created by zhengshulin on 2024/6/21.
//

#ifndef OPENGLPROJ_INSPECTOR_H
#define OPENGLPROJ_INSPECTOR_H

#include "EditorWidget.h"
class SceneNode;
class GameObject;
class Inspector: public EditorWidget{
private:
    std::string inspectName;
    GameObject*node= nullptr;
     std::vector<class ComponentInspector*> componentInspectors;
public:
    void Draw() override;

    Inspector(const std::string &name, EditorScene *scene);

//    void InspectNode( SceneNode*node);
    void InspectNode( GameObject*node);
};


#endif //OPENGLPROJ_INSPECTOR_H
