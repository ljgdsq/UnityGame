#pragma once
#include "spdlog/sinks/base_sink.h"
#include "spdlog/spdlog.h"
#include <vector>
#include <string>

namespace editor
{
    class EditorLogSink : public spdlog::sinks::base_sink<std::mutex>
    {
    public:
        EditorLogSink();
        ~EditorLogSink() override;
        void Clear();
        std::vector<std::string> GetLogEntries() const { return m_logEntries; }
    protected:
        void sink_it_(const spdlog::details::log_msg &msg) override;
        void flush_() override;

    private:
        std::vector<std::string> m_logEntries; // 存储日志条目
    };
} // namespace editor
