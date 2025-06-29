#include "Framework/Render/Renderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
Renderer::Renderer(GLFWwindow *window)
{
    this->window = window;
    // // 初始化OpenGL相关设置
    // glEnable(GL_DEPTH_TEST); // 启用深度测试
    // glDepthFunc(GL_LESS); // 设置深度测试函数
    // glEnable(GL_BLEND); // 启用混合
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // 设置混合函数
}
void Renderer::Clear()
{
    // 清除颜色和深度缓冲
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

void Renderer::SwapBuffers()
{
     glfwSwapBuffers(window);
     glfwPollEvents();
}

void Renderer::SetPolygonMode(PolygonMode mode)
{
    switch (mode) {
        case PolygonMode::Fill:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        case PolygonMode::Line:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
        case PolygonMode::Point:
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            break;
        default:
            // 默认使用填充模式
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
    }
}

void Renderer::EnableDepthTest(bool enable)
{
    if (enable) {
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }
}

void Renderer::EnableBlending(bool enable) {
    if (enable) {
        glEnable(GL_BLEND);
    } else {
        glDisable(GL_BLEND);
    }
}

void Renderer::SetViewport(int x, int y, int width, int height) {
    glViewport(x, y, width, height);
}