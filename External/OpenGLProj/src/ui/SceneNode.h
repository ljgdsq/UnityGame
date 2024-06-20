//
// Created by zhengshulin on 2024/6/20.
//

#ifndef OPENGLPROJ_SCENENODE_H
#define OPENGLPROJ_SCENENODE_H


#include "core/Drawable.h"
#include "core/texture.h"
#include "core/Context.h"

class SceneNode {
protected:
    Drawable* drawable= nullptr;
    glm::vec2 position;
    glm::vec2 size;
    glm::vec2 scale;
    float rotate;
    glm::vec3 color;
    Texture2D texture2D;

public:
    virtual void Update(){}
    virtual void Draw(const Context&ctx){

        if (drawable){
            drawable->ApplyStateChange(ctx);
            drawable->Draw();
        }
    }
};


#endif //OPENGLPROJ_SCENENODE_H
