#include "Framework/Editor/GameView.h"
#include "GameView.h"

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
            ImVec2 texturePosition = ImGui::GetCursorScreenPos();
            texturePosition.x += (windowSize.x - displayWidth) * 0.5f;
            texturePosition.y += (windowSize.y - displayHeight) * 0.5f;
            ImGui::SetCursorScreenPos(texturePosition);
            ImGui::Image(ImTextureID(m_gameFrameBuffer->GetColorBuffer()), ImVec2(displayWidth, displayHeight));
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
