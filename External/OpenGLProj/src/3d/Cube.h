//
// Created by zhengshulin on 2024/6/20.
//

#ifndef OPENGLPROJ_CUBE_H
#define OPENGLPROJ_CUBE_H


#include "ui/SceneNode.h"

class Cube : public SceneNode{
public:
    static Cube*Create(std::string path="");
    bool Init(std::string path);
    void SetTexture(std::string path);
private:
    void UpdateShader(const Context&ctx);
    std::string path;
    bool hasTexture= false;
};


#endif //OPENGLPROJ_CUBE_H
