#pragma once
#include <string>

namespace framework
{

    struct WindowConfig
    {
        std::string title;       // 窗口标题
        int width = 800;         // 窗口宽度
        int height = 600;        // 窗口高度
        bool resizable = true;   // 是否可调整大小
        bool fullscreen = false; // 是否全屏
        bool vsync = true;       // 是否启用垂直同步
        std::string iconPath;    // 窗口图标路径
    };

    /**
     * @brief 窗口接口，定义了窗口的基本操作和属性
     */
    class IWindow
    {
    public:

        using MouseButtonCallback = void (*)(int button, int action, int mods);
        using KeyCallback = void (*)(int key, int action);
        using ScrollCallback = void (*)(double xoffset, double yoffset);
        using FramebufferSizeCallback = void (*)(int width, int height);
        using WindowCloseCallback = void (*)();
        using WindowFocusCallback = void (*)(int focused);
        using WindowIconifyCallback = void (*)(int iconified);
        using WindowMaximizeCallback = void (*)(int maximized);
        using WindowContentScaleCallback = void (*)(float xscale, float yscale);
        using WindowRefreshCallback = void (*)();
        using WindowPosCallback = void (*)(int xpos, int ypos);
        using WindowSizeCallback = void (*)(int width, int height);
    public:
        IWindow() = default;
        virtual ~IWindow() = default;
        virtual void Initialize(const WindowConfig &config) = 0;
        virtual void SetTitle(const std::string &title) = 0;
        virtual void SetSize(int width, int height) = 0;
        virtual void SetPosition(int x, int y) = 0;
        virtual void SetVisible(bool visible) = 0;
        virtual void SetResizable(bool resizable) = 0;
        virtual void SetFullscreen(bool fullscreen) = 0;
        virtual void SetVSync(bool enabled) = 0;
        virtual void PollEvents() = 0;
        virtual void *GetNativeWindowHandle() const = 0;       // 返回原生窗口
        virtual void *GetNativeDisplayHandle() const = 0;      // 返回原生显示
        virtual void SetIcon(const std::string &iconPath) = 0; // 设置窗口图标
        virtual void SetCursorPosition(int x, int y) = 0;
        virtual void SetCursorVisible(bool visible) = 0;                                              // 设置光标可见
        virtual void SetCursorGrabbed(bool grabbed) = 0;                                              // 设置光标是否被抓取
        virtual void SetCursorType(int cursorType) = 0;                                               // 设置光标类型
        virtual void SetMouseButtonCallback(MouseButtonCallback callback) = 0;            // 设置鼠标按钮回调
        virtual void SetKeyCallback(KeyCallback callback) = 0;                       // 设置键盘回调
        virtual void SetScrollCallback(ScrollCallback callback) = 0;         // 设置滚轮回调
        virtual void SetFramebufferSizeCallback(FramebufferSizeCallback callback) = 0;         // 设置帧缓冲大小回调
        virtual void SetWindowCloseCallback(WindowCloseCallback callback) = 0;                                  // 设置窗口关闭回调
        virtual void SetWindowFocusCallback(WindowFocusCallback callback) = 0;                       // 设置窗口焦点回调
        virtual void SetWindowIconifyCallback(WindowIconifyCallback callback) = 0;                   // 设置窗口最小化回调
        virtual void SetWindowMaximizeCallback(WindowMaximizeCallback callback) = 0;                  // 设置窗口最大化回调
        virtual void SetWindowContentScaleCallback(WindowContentScaleCallback callback) = 0; // 设置窗口内容缩放回调
        virtual void SetWindowRefreshCallback(WindowRefreshCallback callback) = 0;                                // 设置窗口刷新回调
        virtual void SetWindowPosCallback(WindowPosCallback callback) = 0;                  // 设置窗口位置回调
        virtual void SetWindowSizeCallback(WindowSizeCallback callback) = 0;              // 设置窗口大小回调

        protected:
            MouseButtonCallback m_mouseButtonCallback;
            KeyCallback m_keyCallback;
            ScrollCallback m_scrollCallback;
            FramebufferSizeCallback m_framebufferSizeCallback;
            WindowCloseCallback m_windowCloseCallback;
            WindowFocusCallback m_windowFocusCallback;
            WindowIconifyCallback m_windowIconifyCallback;
            WindowMaximizeCallback m_windowMaximizeCallback;
            WindowContentScaleCallback m_windowContentScaleCallback;
            WindowRefreshCallback m_windowRefreshCallback;
            WindowPosCallback m_windowPosCallback;
            WindowSizeCallback m_windowSizeCallback;
            WindowConfig m_config; // 窗口配置
    };

} // namespace framework
