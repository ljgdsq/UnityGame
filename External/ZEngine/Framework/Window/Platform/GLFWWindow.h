#pragma once
#include "Framework/Window/IWindow.h"
#include "GLFW/glfw3.h"

namespace framework
{
    class GLFWWindow : public IWindow
    {

    public:
        GLFWWindow() = default;
        virtual ~GLFWWindow() override;
        virtual void Initialize(const WindowConfig &config) override;
        virtual void SetTitle(const std::string &title) override;
        virtual void SetSize(int width, int height) override;
        virtual void SetPosition(int x, int y) override;
        virtual void SetVisible(bool visible) override;
        virtual void SetResizable(bool resizable) override;
        virtual void SetFullscreen(bool fullscreen) override;
        virtual void SetVSync(bool enabled) override;
        virtual void PollEvents() override;
        virtual void WaitForSleep(double seconds) override; // 等待指定时间，通常用于帧率控制
        virtual void SetShouldClose(bool shouldClose) override; // 设置窗口是否应该关闭
        virtual bool ShouldClose() const override;             // 检查窗口是否应该关闭
        virtual void *GetNativeWindowHandle() const override;       // 返回原生窗口
        virtual void *GetNativeDisplayHandle() const override;      // 返回原生显示
        virtual void SetIcon(const std::string &iconPath) override; // 设置窗口图标
        virtual void SetCursorPosition(int x, int y) override;
        virtual void SetCursorVisible(bool visible) override;                                     // 设置光标可见
        virtual void SetCursorGrabbed(bool grabbed) override;                                     // 设置光标是否被抓取
        virtual void SetCursorType(int cursorType) override;                                      // 设置光标类型
        virtual void SetMouseButtonCallback(MouseButtonCallback callback) override;               // 设置鼠标按钮回调
        virtual void SetKeyCallback(KeyCallback callback) override;                               // 设置键盘回调
        virtual void SetScrollCallback(ScrollCallback callback) override;                         // 设置滚轮回调
        virtual void SetFramebufferSizeCallback(FramebufferSizeCallback callback) override;       // 设置帧缓冲大小回调
        virtual void SetWindowCloseCallback(WindowCloseCallback callback) override;               // 设置窗口关闭回调
        virtual void SetWindowFocusCallback(WindowFocusCallback callback) override;               // 设置窗口焦点回调
        virtual void SetWindowIconifyCallback(WindowIconifyCallback callback) override;           // 设置窗口最小化回调
        virtual void SetWindowMaximizeCallback(WindowMaximizeCallback callback) override;         // 设置窗口最大化回调
        virtual void SetWindowContentScaleCallback(WindowContentScaleCallback callback) override; // 设置窗口内容缩放回调
        virtual void SetWindowRefreshCallback(WindowRefreshCallback callback) override;           // 设置窗口刷新回调
        virtual void SetWindowPosCallback(WindowPosCallback callback) override;                   // 设置窗口位置回调
        virtual void SetWindowSizeCallback(WindowSizeCallback callback) override;                 // 设置窗口大小回调
        virtual void Destroy() override;                                                          // 销毁窗口
        virtual void SwapBuffers() override;                                                      // 交换缓冲区

    private:
        // GLFW specific members
        GLFWwindow *m_window = nullptr;
    };
}
