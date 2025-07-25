#pragma once
#include "Framework/Editor/EditorWidget.h"
#include "Framework/Editor/Log/EditorLogSink.h"
#include <vector>
#include <string>
#include <memory>
namespace editor
{
    class LogView : public EditorWidget
    {
    public:
        LogView();
        ~LogView() override;

        void Initialize() override;
        void Update(float deltaTime) override;
        void Render() override;
        void Shutdown() override;
        void SetLogSink(std::shared_ptr<editor::EditorLogSink> logSink);
    private:
        void RenderLogEntries();
        void ClearLogEntries();
        std::shared_ptr<editor::EditorLogSink> m_logSink; // 日志接收器

    };
} // namespace editor
