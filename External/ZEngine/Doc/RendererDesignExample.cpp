#include "RendererDesignExample.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace framework {

// Pimpl实现类
class Renderer::RendererImpl {
public:
    GLFWwindow* glfwWindow = nullptr;
    IWindow* abstractWindow = nullptr;
    float clearColor[4] = {0.2f, 0.3f, 0.3f, 1.0f};
    bool useAbstractWindow = false;

    void SwapBuffersImpl() {
        if (useAbstractWindow && abstractWindow) {
            abstractWindow->SwapBuffers();
            abstractWindow->PollEvents();
        } else if (glfwWindow) {
            glfwSwapBuffers(glfwWindow);
            glfwPollEvents();
        }
    }
};

// 使用原始窗口句柄构造
Renderer::Renderer(void* windowHandle) : m_impl(new RendererImpl()) {
    m_impl->glfwWindow = static_cast<GLFWwindow*>(windowHandle);
    m_impl->useAbstractWindow = false;
    
    // 初始化OpenGL设置
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

// 使用抽象窗口接口构造
Renderer::Renderer(IWindow* window) : m_impl(new RendererImpl()) {
    m_impl->abstractWindow = window;
    m_impl->useAbstractWindow = true;
    if (window) {
        m_impl->glfwWindow = static_cast<GLFWwindow*>(window->GetNativeWindow());
    }
    
    // 初始化OpenGL设置
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Renderer::~Renderer() {
    delete m_impl;
}

void Renderer::Clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(
        m_impl->clearColor[0],
        m_impl->clearColor[1],
        m_impl->clearColor[2],
        m_impl->clearColor[3]
    );
}

void Renderer::SetClearColor(float r, float g, float b, float a) {
    m_impl->clearColor[0] = r;
    m_impl->clearColor[1] = g;
    m_impl->clearColor[2] = b;
    m_impl->clearColor[3] = a;
}

void Renderer::SwapBuffers() {
    m_impl->SwapBuffersImpl();
}

void Renderer::EnableDepthTest(bool enable) {
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

} // namespace framework
