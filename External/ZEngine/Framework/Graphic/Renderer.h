#pragma once
#include "Framework/Common/Define.h"
#include "Framework/Window/IWindow.h"
enum class PolygonMode
{
    Fill = 0x1B02,      // GL_FILL
    Line = 0x1B01,      // GL_LINE
    Point = 0x1B00      // GL_POINT
};
namespace framework {
    class Renderer
    {
        SINGLETON_CLASS(Renderer);
    public:

        void Initialize();
        void Clear();
        void SwapBuffers();
        void SetPolygonMode(PolygonMode mode);
        void EnableDepthTest(bool enable = true);
        void EnableBlending(bool enable = true);
        void SetViewport(int x, int y, int width, int height);

    protected:
        IWindow* window = nullptr;
    };
}
