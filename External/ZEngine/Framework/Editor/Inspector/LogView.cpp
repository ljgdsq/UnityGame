#include "Framework/Editor/LogView.h"

namespace editor
{
    LogView::LogView() : EditorWidget("Log View")
    {
    }

    LogView::~LogView()
    {
        Shutdown();
    }

    void LogView::Initialize()
    {
    }

    void LogView::SetLogSink(std::shared_ptr<editor::EditorLogSink> logSink)
    {
        m_logSink = logSink;
        if (m_logSink)
        {
            m_logSink->Clear(); // 清空日志条目
        }
    }

    void LogView::Update(float deltaTime)
    {
    }

    void LogView::Render()
    {
        ImGui::Begin(name.c_str());

        // 获取窗口尺寸
        ImVec2 windowSize = ImGui::GetWindowSize();
        float buttonHeight = ImGui::GetFrameHeight() + 20; // 按钮区域高度（包含边距）

        // 日志内容区域（可滚动，占据大部分空间）
        ImGui::BeginChild("LogScrollArea", ImVec2(0, -buttonHeight), true, ImGuiWindowFlags_HorizontalScrollbar);
        RenderLogEntries();
        ImGui::EndChild();

        // 分隔线
        ImGui::Separator();

        // 按钮区域（固定在底部，不滚动）
        float buttonWidth = 100;
        ImGui::SetCursorPosX(windowSize.x - buttonWidth - 15); // 右对齐

        if (ImGui::Button("Clear Logs", ImVec2(buttonWidth, 0)))
        {
            ClearLogEntries();
        }

        ImGui::End();
    }

    void LogView::Shutdown()
    {
        ClearLogEntries();
    }

    void LogView::RenderLogEntries()
    {
        if (m_logSink)
        {
            const auto &logEntries = m_logSink->GetLogEntries();
            for (const auto &log : logEntries)
            {

                if (log.find("[error]") != std::string::npos)
                {
                    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%s", log.c_str());
                }
                else if (log.find("[warn]") != std::string::npos)
                {
                    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", log.c_str());
                }
                else if (log.find("[info]") != std::string::npos)
                {
                    ImGui::TextColored(ImVec4(0.0f, 0.9f, 0.0f, 1.0f), "%s", log.c_str());
                }
                else if (log.find("[debug]") != std::string::npos)
                {
                    ImGui::TextColored(ImVec4(0.9f, 0.9f, 0.9f, 1.0f), "%s", log.c_str());
                }
                else
                {
                    ImGui::Text("%s", log.c_str());
                }
            }
        }
        else
        {
            ImGui::Text("No log sink set.");
        }
    }

    void LogView::ClearLogEntries()
    {
        if (m_logSink)
        {
            m_logSink->Clear();
        }
    }
}
