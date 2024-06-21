//
// Created by zhengshulin on 2024/6/20.
//

#ifndef OPENGLPROJ_SCENENODE_H
#define OPENGLPROJ_SCENENODE_H


#include "core/Drawable.h"
#include "core/texture.h"
#include "core/Context.h"
#include "core/resource_manager.h"

class SceneNode {
public:
    glm::vec3 position;
    glm::vec3 size;
    glm::vec3 scale;
    glm::vec3 rotate;
    glm::vec3 color;

    std::function<void(const Context&ctx)> BeforeDraw;
    std::function<void(const Context&ctx)> AfterDraw;
    Texture2D texture2D;

protected:
    Drawable* drawable= nullptr;
protected:
    virtual void UpdateShader(const Context&ctx){};
public:
    void SetShader(std::string name){
        if (drawable){
            drawable->shader= ResourceManager::GetShader(name);
        }
    }
    virtual void Update(){}
    virtual void Draw(const Context&ctx){
        UpdateShader(ctx);
        if (BeforeDraw)
            BeforeDraw(ctx);
        if (drawable){
            drawable->Draw();
        }
        if (AfterDraw)
            AfterDraw(ctx);
    }
};


#endif //OPENGLPROJ_SCENENODE_H
