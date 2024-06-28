//
// Created by zhengshulin on 2024/6/28.
//

#include "MeshRenderer.h"
#include "Mesh.h"
vector<class Mesh*> MeshRenderer::meshes;
void MeshRenderer::RenderAll(const Context &context) {
    for (const auto&mesh:meshes){
        mesh->Draw(context);
    }
}

void MeshRenderer::SubmitMesh(Mesh *mesh) {
    meshes.push_back(mesh);
}

void MeshRenderer::RemoveMesh( Mesh *mesh) {
    auto it=std::find(meshes.begin(),meshes.end(),mesh);
    if (it!=meshes.end()){
        meshes.erase(it);
    }
}
