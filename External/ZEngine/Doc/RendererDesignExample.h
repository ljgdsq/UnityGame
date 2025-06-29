#pragma once

// 使用前向声明，避免包含GLFW头文件
struct GLFWwindow;

namespace framework {

// 使用抽象窗口接口
class IWindow {
public:
    virtual ~IWindow() = default;
    virtual void* GetNativeWindow() const = 0;
    virtual void SwapBuffers() = 0;
    virtual void PollEvents() = 0;
};

// 渲染器类
class Renderer {
public:
    // 构造函数可以接受两种类型的窗口：原始指针或抽象接口
    explicit Renderer(void* windowHandle);
    explicit Renderer(IWindow* window);
    ~Renderer();

    // 禁用拷贝
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    // 渲染操作
    void Clear();
    void SetClearColor(float r, float g, float b, float a);
    void SwapBuffers();

    // 渲染器配置
    void EnableDepthTest(bool enable = true);
    void EnableBlending(bool enable = true);
    void SetViewport(int x, int y, int width, int height);

private:
    // 使用Pimpl模式隐藏实现细节
    class RendererImpl;
    RendererImpl* m_impl;

    // 或者使用不透明指针保存窗口（如果不用Pimpl）
    // void* m_windowHandle;
    // IWindow* m_window;
};

} // namespace framework
