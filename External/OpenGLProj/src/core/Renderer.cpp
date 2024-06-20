//
// Created by zhengshulin on 2024/6/17.
//

#include "Renderer.h"
#include "glad/gl.h"
#include "GLFW/glfw3.h"

static GLFWwindow* window;

void Renderer::ClearColor(float r, float g, float b, float a) {
    this->r=r;
    this->g=g;
    this->b=b;
    this->a=a;
}

void Renderer::SetClearMode(int mode) {
    if (mode & COLOR_BIT) {
        clearBit |= GL_COLOR_BUFFER_BIT;
    }
    if (mode & DEPTH_BIT) {
        clearBit |= GL_DEPTH_BUFFER_BIT;
    }
    if (mode & STENCIL_BIT) {
        clearBit |= GL_STENCIL_BUFFER_BIT;
    }
}


void Renderer::Present() {
    glfwSwapBuffers(window);
}

void Renderer::Clear() {
    glClear(clearBit);
    glClearColor(r,g,b,a);
}

Renderer::Renderer(Context*context) {
    this->context=context;
    window= static_cast<GLFWwindow *>(this->context->window);
    clearBit=0;
    SetClearMode(ClearMode::COLOR_BIT);
}

void Renderer::Enable(FuncType type) {
    if (type==FuncType::Depth_Test){
        glEnable(GL_DEPTH_TEST);
    }
}

void Renderer::Disable(FuncType type) {
    if (type==FuncType::Depth_Test){
        glDisable(GL_DEPTH_TEST);
    }
}

