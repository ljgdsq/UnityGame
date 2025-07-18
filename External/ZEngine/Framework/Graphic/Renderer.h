#pragma once
#include "Framework/Common/Define.h"
#include "Framework/Window/IWindow.h"
enum class PolygonMode
{
    Fill, // FILL
    Line, // LINE
    Point // POINT
};
namespace framework
{
    class Renderer
    {
    public:
        // SINGLETON_CLASS(Renderer);
        using ReturnType = void (*)();
        using LoaderFunc = ReturnType (*)(const char *);

    public:
        void Initialize(IWindow *window, LoaderFunc renderAPILoaderProc = nullptr);
        void Clear();
        void SwapBuffers();
        void SetPolygonMode(PolygonMode mode);
        void EnableDepthTest(bool enable = true);
        void EnableBlending(bool enable = true);
        void SetViewport(int x, int y, int width, int height);

    protected:
        IWindow *m_window = nullptr;
    };
}
