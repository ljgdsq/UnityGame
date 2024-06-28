//
// Created by zhengshulin on 2024/6/28.
//

#ifndef OPENGLPROJ_CUBEMESH_H
#define OPENGLPROJ_CUBEMESH_H


#include "core/Mesh.h"

class CubeMesh: public Mesh {
public:
    CubeMesh();

    void UpdateShader() override;
};


#endif //OPENGLPROJ_CUBEMESH_H
