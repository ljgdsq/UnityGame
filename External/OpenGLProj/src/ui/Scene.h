//
// Created by zhengshulin on 2024/6/20.
//

#ifndef OPENGLPROJ_SCENE_H
#define OPENGLPROJ_SCENE_H
#include <vector>
#include "ui/SceneNode.h"
#include "core/Context.h"

class Scene {
    std::vector<SceneNode*> nodes;
    friend class Application;
    void Update();
    void Render(const Context&ctx);
    void Create();
private:

public:
    virtual void OnCreate(){};
    virtual void OnUpdate(){

    };
    void AddChild(SceneNode*child);
    virtual void Destroy(){};
};


#endif //OPENGLPROJ_SCENE_H