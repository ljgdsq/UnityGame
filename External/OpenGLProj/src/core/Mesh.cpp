//
// Created by zhengshulin on 2024/6/28.
//

#include "Mesh.h"
#include "shader.h"

void Mesh::GenBuffer() {
    VAO->bind();

    VBO->bind();
    VBO->uploadData(vertices.size()* sizeof(Vertex),&vertices[0],DataUsage::STATIC_DRAW);

    if (indices.size()>0){
        EBO->bind();
        EBO->uploadData(indices.size()* sizeof(unsigned int),&indices[0],DataUsage::STATIC_DRAW);
    }

    VAO->addBufferLayout({0,3,GL_FLOAT, false, sizeof(Vertex),0});
    VAO->enableAttribute(0);

    VAO->addBufferLayout({1,2,GL_FLOAT, false, sizeof(Vertex), (int)offsetof(Vertex,texCoords)});
    VAO->enableAttribute(1);

//    VAO->addBufferLayout({2,2,GL_FLOAT, false, sizeof(Vertex), (int)offsetof(Vertex,TexCoords)});
//    VAO->enableAttribute(2);

    VAO->unbind();
}

void Mesh::Draw(const Context&ctx) {
    shader->Use();
    UpdateShader();
    VAO->bind();
    if (indices.size()>0){
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    }else{
        glDrawArrays(GL_TRIANGLES,0,vertices.size());
    }
    VAO->unbind();
}

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture2D*> textures) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    VAO=new VertexArray();
    VBO=new Buffer(BufferType::VBO);
    EBO=new Buffer(BufferType::EBO);
    GenBuffer();
}

void Mesh::UpdateShader() {

}
