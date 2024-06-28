//
// Created by zhengshulin on 2024/6/28.
//

#include "CubeMesh.h"
#include "core/ResourceManager.h"

vector<Vertex> cubeVertices = {
        // positions          // texture Coords
        {-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,},
        {0.5f, -0.5f, -0.5f, 1.0f, 0.0f,},
        {0.5f, 0.5f, -0.5f, 1.0f, 1.0f,},
        {0.5f, 0.5f, -0.5f, 1.0f, 1.0f,},
        {-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,},
        {-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,},

        {-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,},
        {0.5f, -0.5f, 0.5f, 1.0f, 0.0f,},
        {0.5f, 0.5f, 0.5f, 1.0f, 1.0f,},
        {0.5f, 0.5f, 0.5f, 1.0f, 1.0f,},
        {-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,},
        {-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,},

        {-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,},
        {-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,},
        {-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,},
        {-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,},
        {-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,},
        {-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,},

        {0.5f, 0.5f, 0.5f, 1.0f, 0.0f,},
        {0.5f, 0.5f, -0.5f, 1.0f, 1.0f,},
        {0.5f, -0.5f, -0.5f, 0.0f, 1.0f,},
        {0.5f, -0.5f, -0.5f, 0.0f, 1.0f,},
        {0.5f, -0.5f, 0.5f, 0.0f, 0.0f,},
        {0.5f, 0.5f, 0.5f, 1.0f, 0.0f,},
        {-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,},
        {0.5f, -0.5f, -0.5f, 1.0f, 1.0f,},
        {0.5f, -0.5f, 0.5f, 1.0f, 0.0f,},
        {0.5f, -0.5f, 0.5f, 1.0f, 0.0f,},
        {-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,},
        {-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,},
        {-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,},
        {0.5f, 0.5f, -0.5f, 1.0f, 1.0f,},
        {0.5f, 0.5f, 0.5f, 1.0f, 0.0f,},
        {0.5f, 0.5f, 0.5f, 1.0f, 0.0f,},
        {-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,},
        {-0.5f, 0.5f, -0.5f, 0.0f, 1.0f}
};

CubeMesh::CubeMesh() : Mesh(cubeVertices, {}, {}) {
    textures.push_back(ResourceManager::GetTexture("container"));
    shader=ResourceManager::GetShader("cube2");
}

void CubeMesh::UpdateShader() {
    shader->SetInteger("hasTex",1);
    textures[0]->Bind();
}
