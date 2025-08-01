#include "Framework/Editor/GameView.h"
#include "ImGuizmo.h"
namespace editor
{
    GameView::GameView() : EditorWidget("Game View"),
                           m_gameFrameBuffer(nullptr),
                           m_isFocused(false)
    {
    }
    GameView::~GameView()
    {
    }
    void GameView::Initialize()
    {
    }
    void GameView::Render()
    {
        ImGui::Begin(name.c_str());
        bool isFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);
        // Application::setIgnoreInput(!isFocused);
        ImVec2 windowSize = ImGui::GetContentRegionAvail();
        if (m_gameFrameBuffer)
        {
            int textureWidth = m_gameFrameBuffer->GetWidth();
            int textureHeight = m_gameFrameBuffer->GetHeight();
            // 计算保持宽高比缩放后的纹理大小
            float aspectRatio = static_cast<float>(textureWidth) / textureHeight;
            float displayWidth, displayHeight;
            if (windowSize.x / windowSize.y > aspectRatio)
            {
                // 窗口宽高比大于纹理宽高比，根据高度调整宽度
                displayHeight = windowSize.y;
                displayWidth = displayHeight * aspectRatio;
            }
            else
            {
                // 窗口宽高比小于或等于纹理宽高比，根据宽度调整高度
                displayWidth = windowSize.x;
                displayHeight = displayWidth / aspectRatio;
            }

            // 计算纹理显示位置使其居中
            ImVec2 centerOffset;
            centerOffset.x = (windowSize.x - displayWidth) * 0.5f;
            centerOffset.y = (windowSize.y - displayHeight) * 0.5f;

            // 保存当前光标位置，然后设置到居中位置
            ImVec2 contentRegionMin = ImGui::GetWindowContentRegionMin();
            ImVec2 cursorPos;
            cursorPos.x = contentRegionMin.x + centerOffset.x;
            cursorPos.y = contentRegionMin.y + centerOffset.y;
            ImGui::SetCursorPos(cursorPos);

            ImGui::Image(ImTextureID(static_cast<size_t>(m_gameFrameBuffer->GetColorBuffer())), ImVec2(displayWidth, displayHeight),
                         ImVec2(0, 1), ImVec2(1, 0)); // OpenGL纹理坐标系是左下角为原点

            // 设置ImGuizmo的渲染区域为GameView窗口
            if (true)
            {
                ImGuizmo::SetDrawlist();
                // 获取当前窗口在屏幕中的位置和内容区域偏移
                ImVec2 windowPos = ImGui::GetWindowPos();
                ImVec2 contentRegionMinAbs = ImVec2(windowPos.x + contentRegionMin.x, windowPos.y + contentRegionMin.y);
                // 设置 ImGuizmo 的渲染区域，使用绝对屏幕坐标
                ImGuizmo::SetRect(contentRegionMinAbs.x + centerOffset.x, contentRegionMinAbs.y + centerOffset.y, displayWidth, displayHeight);
            }
        }

        ImGui::End();
    }
    void GameView::Update(float deltaTime)
    {
    }
    void GameView::Shutdown()
    {
    }
    void GameView::SetGameFrameBuffer(FrameBuffer *frameBuffer)
    {
        m_gameFrameBuffer = frameBuffer;
    }
}
