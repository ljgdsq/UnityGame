//
// Created by zhengshulin on 2024/6/21.
//

#ifndef OPENGLPROJ_PLANE_H
#define OPENGLPROJ_PLANE_H


#include "ui/SceneNode.h"

class Plane: public SceneNode {
public:
    static Plane*Create(std::string path="");
    bool Init(std::string path);
    void SetTexture(std::string path);
protected:
    std::string path;
    bool hasTexture= false;
    void UpdateShader(const Context &ctx) override;

    std::string GetTypeName() override;
};


#endif //OPENGLPROJ_PLANE_H
