//
// Created by zhengshulin on 2024/6/20.
//

#ifndef OPENGLPROJ_CUBE_H
#define OPENGLPROJ_CUBE_H


#include "ui/SceneNode.h"

class Cube : public SceneNode{
public:
    static std::string type;
    static Cube*Create(std::string path="");
    bool Init(std::string path);
    void SetTexture(std::string path);
private:
    std::string path;
    bool hasTexture= false;

protected:
    void UpdateShader(const Context &ctx) override;

    std::string GetTypeName() override;
};


#endif //OPENGLPROJ_CUBE_H
