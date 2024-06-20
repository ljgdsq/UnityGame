//
// Created by zhengshulin on 2024/6/20.
//

#ifndef OPENGLPROJ_UISPRITE_H
#define OPENGLPROJ_UISPRITE_H


#include "SceneNode.h"
#include "core/texture.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include <string>
class UISprite: public SceneNode{
public:
    static UISprite*Create(std::string path);
    bool Init(std::string path);
    void Position(int x,int y);
    void Scale(float x,float y);
    void Angle(float angle);
private:
    void InitWithTexture(std::string path);
    void UpdateShader();

};


#endif //OPENGLPROJ_UISPRITE_H
