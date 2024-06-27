//
// Created by zhengshulin on 2024/6/27.
//

#include "LogWindow.h"
#include "imgui.h"

LogWindow::LogWindow(const string &name, EditorScene *scene) : EditorWidget(name, scene) {}

void LogWindow::Draw() {
    ImGui::Begin(name.c_str());
    if (ImGui::Button("Clear Logs")) {
        if (logSink) {
            logSink->buffer.clear();
        }
    }
    if (logSink){
        for (const auto& log : logSink->buffer) {
            // 根据日志级别设置颜色
            if (log.find("[error]") != std::string::npos) {
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%s", log.c_str());
            } else if (log.find("[warn]") != std::string::npos) {
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", log.c_str());
            } else {
                ImGui::Text("%s", log.c_str());
            }
        }
    }


    ImGui::End();
}

ImGuiLogSink *LogWindow::getLogSink() const {
    return logSink;
}

void LogWindow::setLogSink(ImGuiLogSink *logSink) {
    this->logSink = logSink;
}

void LogWindow::clear() {
    this->logSink->clear();
}
