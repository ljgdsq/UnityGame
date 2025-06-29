#pragma once

struct GLFWwindow; // Forward declaration to avoid including GLFW header

enum class PolygonMode
{
    Fill = 0x1B02,      // GL_FILL
    Line = 0x1B01,      // GL_LINE
    Point = 0x1B00      // GL_POINT
};

class Renderer
{
public:
    Renderer(GLFWwindow *window);
    virtual ~Renderer() = default;

    void Clear();
    void SwapBuffers();

    void SetPolygonMode(PolygonMode mode);
    void EnableDepthTest(bool enable = true);
    void EnableBlending(bool enable = true);
    void SetViewport(int x, int y, int width, int height);

protected:
    GLFWwindow *window = nullptr;
};