//
// Created by zhengshulin on 2024/6/21.
//

#include "PlaneQuadDrawable.h"


void PlaneQuadDrawable::Draw() {

    VAO->bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    VAO->unbind();
}

PlaneQuadDrawable::PlaneQuadDrawable(Shader *shader) : Drawable(shader) {
    float vertices[] = {
            // Pos      // Tex
            0.0f, 1.0f,0, 1.0f, 0.0f,
            1.0f, 0.0f,0, 0.0f, 1.0f,
            0.0f, 0.0f,0, 1.0f, 1.0f,

            0.0f, 1.0f,0, 1.0f, 0.0f,
            1.0f, 1.0f,0, 0.0f, 0.0f,
            1.0f, 0.0f,0, 0.0f, 1.0f
    };

    VAO=new VertexArray();
    VBO=new Buffer(BufferType::VBO);

    VBO->uploadData(sizeof(vertices),vertices,DataUsage::STATIC_DRAW);

    VAO->addBufferLayout({0,3,GL_FLOAT, false,5* sizeof(float ),0});
    VAO->enableAttribute(0);

    VAO->addBufferLayout({1,2,GL_FLOAT, false,5* sizeof(float ),3* sizeof(float )});
    VAO->enableAttribute(1);

    VBO->unbind();
    VAO->unbind();

}

PlaneQuadDrawable::~PlaneQuadDrawable() {
    VBO->destroy();
    delete VBO;
    VAO->destroy();
    delete VAO;
}
