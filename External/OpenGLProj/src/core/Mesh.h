//
// Created by zhengshulin on 2024/6/28.
//

#ifndef OPENGLPROJ_MESH_H
#define OPENGLPROJ_MESH_H
#include "Context.h"

#include "common.h"
#include "texture.h"
#include "shader.h"
#include "buffer.h"
#include <vector>

using std::vector;



class Mesh {
public:
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture2D*> textures;

    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture2D*> textures);
    class Shader*shader;
    virtual void Draw(const Context&ctx);
    virtual void UpdateShader();
private:
    Buffer *VBO;
    Buffer *EBO;
    VertexArray *VAO;
    void GenBuffer();
};


#endif //OPENGLPROJ_MESH_H
