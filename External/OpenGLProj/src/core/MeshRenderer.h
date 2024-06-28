//
// Created by zhengshulin on 2024/6/28.
//

#ifndef OPENGLPROJ_MESHRENDERER_H
#define OPENGLPROJ_MESHRENDERER_H


#include "Context.h"
#include <vector>
using std::vector;
class MeshRenderer {
public:
    void RenderAll(const Context&context);
    static void SubmitMesh(class Mesh*mesh);
    static void RemoveMesh(class Mesh*mesh);
private:
   static vector<class Mesh*> meshes;
};


#endif //OPENGLPROJ_MESHRENDERER_H
