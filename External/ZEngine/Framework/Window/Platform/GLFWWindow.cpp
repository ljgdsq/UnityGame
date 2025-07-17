#include "Framework/Window/Platform/GLFWWindow.h"
#include <stdexcept>
namespace framework
{

    GLFWWindow::~GLFWWindow()
    {
        if (m_window)
        {
            glfwDestroyWindow(m_window);
            m_window = nullptr;
        }
    }

    void GLFWWindow::Initialize(const WindowConfig &config)
    {
        // Initialize GLFW and create the window
        if (!glfwInit())
        {
            throw std::runtime_error("Failed to initialize GLFW");
        }

        glfwWindowHint(GLFW_RESIZABLE, config.resizable ? GLFW_TRUE : GLFW_FALSE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // For macOS compatibility
#endif

        m_window = glfwCreateWindow(config.width, config.height, config.title.c_str(), nullptr, nullptr);
        if (!m_window)
        {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }

        glfwMakeContextCurrent(m_window);
        glfwSetWindowUserPointer(m_window, this);
        glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow *window, int width, int height)
                                       {
            auto self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));
            if (self->m_framebufferSizeCallback){
                self->m_framebufferSizeCallback(width, height);
            } });

    }

    void GLFWWindow::SetTitle(const std::string &title)
    {
        glfwSetWindowTitle(m_window, title.c_str());
    }

    void GLFWWindow::SetSize(int width, int height)
    {
        glfwSetWindowSize(m_window, width, height);
    }

    void GLFWWindow::SetPosition(int x, int y)
    {
        glfwSetWindowPos(m_window, x, y);
    }

    void GLFWWindow::SetVisible(bool visible)
    {
        if (visible)
        {
            glfwShowWindow(m_window);
        }
        else
        {
            glfwHideWindow(m_window);
        }
    }

    void GLFWWindow::SetResizable(bool resizable)
    {
        glfwSetWindowAttrib(m_window, GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);
    }

    void GLFWWindow::SetFullscreen(bool fullscreen)
    {
        // todo
    }

    void GLFWWindow::SetVSync(bool enabled)
    {
        glfwSwapInterval(enabled ? 1 : 0);
    }

    void GLFWWindow::PollEvents()
    {
        glfwPollEvents();
    }

    void GLFWWindow::WaitForSleep(double seconds)
    {
        glfwWaitEventsTimeout(seconds);
    }

    void *GLFWWindow::GetNativeWindowHandle() const
    {
        return static_cast<void *>(m_window);
    }

    void *GLFWWindow::GetNativeDisplayHandle() const
    {
        // GLFW does not provide a direct way to get the native display handle
        return nullptr; // or return a specific display handle if needed
    }

    void GLFWWindow::SetIcon(const std::string &iconPath)
    {
        // todo
    }

    void GLFWWindow::SetCursorPosition(int x, int y)
    {
        glfwSetCursorPos(m_window, x, y);
    }

    void GLFWWindow::SetCursorVisible(bool visible)
    {
        if (visible)
        {
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else
        {
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        }
    }

    void GLFWWindow::SetCursorGrabbed(bool grabbed)
    {
        glfwSetInputMode(m_window, GLFW_CURSOR, grabbed ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    }

    void GLFWWindow::SetCursorType(int cursorType)
    {
        // todo
    }

    void GLFWWindow::SetMouseButtonCallback(MouseButtonCallback callback)
    {
        m_mouseButtonCallback = callback;
        glfwSetMouseButtonCallback(m_window, [](GLFWwindow *window, int button, int action, int mods)
                                   {
            auto self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));
            if (self->m_mouseButtonCallback)
            {
                self->m_mouseButtonCallback(button, action,mods);
            } });
    }

    void GLFWWindow::SetKeyCallback(KeyCallback callback)
    {
        m_keyCallback = callback;
        glfwSetKeyCallback(m_window, [](GLFWwindow *window, int key, int scancode, int action, int mods)
                           {
            auto self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));
            if (self->m_keyCallback)
            {
                self->m_keyCallback(key, action);
            } });
    }

    void GLFWWindow::SetScrollCallback(ScrollCallback callback)
    {
        m_scrollCallback = callback;
        glfwSetScrollCallback(m_window, [](GLFWwindow *window, double xoffset, double yoffset)
                              {
            auto self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));
            if (self->m_scrollCallback)
            {
                self->m_scrollCallback(xoffset, yoffset);
            } });
    }

    void GLFWWindow::SetFramebufferSizeCallback(FramebufferSizeCallback callback)
    {
        m_framebufferSizeCallback = callback;
        glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow *window, int width, int height)
                                       {
            auto self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));
            if (self->m_framebufferSizeCallback)
            {
                self->m_framebufferSizeCallback(width, height);
            } });
    }

    void GLFWWindow::SetShouldClose(bool shouldClose)
    {
        glfwSetWindowShouldClose(m_window, shouldClose);
    }

    void GLFWWindow::SetWindowCloseCallback(WindowCloseCallback callback)
    {
        m_windowCloseCallback = callback;
        glfwSetWindowCloseCallback(m_window, [](GLFWwindow *window)
                                   {
            auto self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));
            if (self->m_windowCloseCallback)
            {
                self->m_windowCloseCallback();
            } });
    }

    void GLFWWindow::SetWindowFocusCallback(WindowFocusCallback callback)
    {
        m_windowFocusCallback = callback;
        glfwSetWindowFocusCallback(m_window, [](GLFWwindow *window, int focused)
                                   {
            auto self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));
            if (self->m_windowFocusCallback)
            {
                self->m_windowFocusCallback(focused);
            } });
    }

    void GLFWWindow::SetWindowIconifyCallback(WindowIconifyCallback callback)
    {
        m_windowIconifyCallback = callback;
        glfwSetWindowIconifyCallback(m_window, [](GLFWwindow *window, int iconified)
                                     {
            auto self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));
            if (self->m_windowIconifyCallback)
            {
                self->m_windowIconifyCallback(iconified);
            } });
    }

    void GLFWWindow::SetWindowMaximizeCallback(WindowMaximizeCallback callback)
    {
        m_windowMaximizeCallback = callback;
        glfwSetWindowMaximizeCallback(m_window, [](GLFWwindow *window, int maximized)
                                      {
            auto self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));
            if (self->m_windowMaximizeCallback)
            {
                self->m_windowMaximizeCallback(maximized);
            } });
    }

    void GLFWWindow::SetWindowContentScaleCallback(WindowContentScaleCallback callback)
    {
        m_windowContentScaleCallback = callback;
        glfwSetWindowContentScaleCallback(m_window, [](GLFWwindow *window, float xscale, float yscale)
                                          {
            auto self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));
            if (self->m_windowContentScaleCallback)
            {
                self->m_windowContentScaleCallback(xscale, yscale);
            } });
    }

    void GLFWWindow::SetWindowRefreshCallback(WindowRefreshCallback callback)
    {
        m_windowRefreshCallback = callback;
        glfwSetWindowRefreshCallback(m_window, [](GLFWwindow *window)
                                     {
            auto self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));
            if (self->m_windowRefreshCallback)
            {
                self->m_windowRefreshCallback();
            } });
    }

    void GLFWWindow::SetWindowPosCallback(WindowPosCallback callback)
    {
        m_windowPosCallback = callback;
        glfwSetWindowPosCallback(m_window, [](GLFWwindow *window, int xpos, int ypos)
                                 {
            auto self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));
            if (self->m_windowPosCallback)
            {
                self->m_windowPosCallback(xpos, ypos);
            } });
    }

    void GLFWWindow::SetWindowSizeCallback(WindowSizeCallback callback)
    {
        m_windowSizeCallback = callback;
        glfwSetWindowSizeCallback(m_window, [](GLFWwindow *window, int width, int height)
                                  {
            auto self = static_cast<GLFWWindow*>(glfwGetWindowUserPointer(window));
            if (self->m_windowSizeCallback)
            {
                self->m_windowSizeCallback(width, height);
            } });
    }

    void GLFWWindow::SwapBuffers()
    {
        if (m_window)
        {
            glfwSwapBuffers(m_window);
        }
        else
        {
            throw std::runtime_error("GLFWWindow: No window to swap buffers.");
        }
    }

    void GLFWWindow::Destroy()
    {
        if (m_window)
        {
            glfwDestroyWindow(m_window);
            m_window = nullptr;
        }
        glfwTerminate();
    }

} // namespace framework
