#include "Framework/Graphic/Renderer.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Framework/Log/Logger.h"
namespace framework
{

    static void framebuffer_size_callback(int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    void Renderer::Initialize(IWindow *window, void *renderAPILoaderProc)
    {
        m_window = window;
        // glad: load all OpenGL function pointers
        if (!gladLoadGLLoader((GLADloadproc)renderAPILoaderProc))
        {
            m_window->Destroy();
            throw std::runtime_error("Failed to initialize GLAD");
        }
        glViewport(0, 0, m_window->GetWidth(), m_window->GetHeight());

        glEnable(GL_DEPTH_TEST); // 启用深度测试
        glEnable(GL_BLEND);      // 启用混合

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // 设置混合函数
        m_window->SetFramebufferSizeCallback(framebuffer_size_callback);
    }

    void Renderer::Clear()
    {
        // 清除颜色和深度缓冲
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    }

    void Renderer::SwapBuffers()
    {
        if (!m_window)
        {
            LOG_ERROR("Renderer: No window set for swapping buffers.");
            return;
        }
        m_window->SwapBuffers();
    }

    void Renderer::SetPolygonMode(PolygonMode mode)
    {
        switch (mode)
        {
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
        if (enable)
        {
            glEnable(GL_DEPTH_TEST);
        }
        else
        {
            glDisable(GL_DEPTH_TEST);
        }
    }

    void Renderer::EnableBlending(bool enable)
    {
        if (enable)
        {
            glEnable(GL_BLEND);
        }
        else
        {
            glDisable(GL_BLEND);
        }
    }

    void Renderer::SetViewport(int x, int y, int width, int height)
    {
        glViewport(x, y, width, height);
    }

}
