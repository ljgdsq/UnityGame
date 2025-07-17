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
        // SINGLETON_CLASS(Renderer);

    public:
        void Initialize(IWindow *window, void *renderAPILoaderProc = nullptr);
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
