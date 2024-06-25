//
// Created by zhengshulin on 2024/6/20.
//

#include "QuadDrawable.h"

void QuadDrawable::Draw() {

    VAO->bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    VAO->unbind();
}

QuadDrawable::QuadDrawable(Shader *shader) : Drawable(shader) {
    float vertices[] = {
            // Pos               // Tex
            -0.5f,  0.5f, 0.0f, 1.0f,  // 左上角
            0.5f, -0.5f, 1.0f, 0.0f,  // 右下角
            -0.5f, -0.5f, 0.0f, 0.0f,  // 左下角

            -0.5f,  0.5f, 0.0f, 1.0f,  // 左上角
            0.5f,  0.5f, 1.0f, 1.0f,  // 右上角
            0.5f, -0.5f, 1.0f, 0.0f   // 右下角
    };


    VAO=new VertexArray();
    VBO=new Buffer(BufferType::VBO);

    VBO->uploadData(sizeof(vertices),vertices,DataUsage::STATIC_DRAW);

    VAO->addBufferLayout({0,4,GL_FLOAT, false,4* sizeof(float ),0});
    VAO->enableAttribute(0);

    VBO->unbind();
    VAO->unbind();

}

QuadDrawable::~QuadDrawable() {
    VBO->destroy();
    delete VBO;
    VAO->destroy();
    delete VAO;
}
