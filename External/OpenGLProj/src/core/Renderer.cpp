//
// Created by zhengshulin on 2024/6/17.
//

#include "Renderer.h"
#include "Application.h"


void Renderer::ClearColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
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

static GLFWwindow*window= nullptr;

void Renderer::Present() {
    glfwSwapBuffers(window);
}

void Renderer::Clear() {
    glClear(clearBit);
}

Renderer::Renderer() {
    window=Application::GetInstance()->GetWindow();
    clearBit=0;
    SetClearMode(ClearMode::COLOR_BIT);
}

void Renderer::Enable(FuncType type) {
    if (type==FuncType::Depth_Test){
        glEnable(GL_DEPTH_TEST);
    }
}
