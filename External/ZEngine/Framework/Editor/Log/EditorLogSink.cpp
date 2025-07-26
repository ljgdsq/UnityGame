#include "Framework/Editor/Log/EditorLogSink.h"

namespace editor
{
    EditorLogSink::EditorLogSink()
    {
        // 构造函数
    }

    EditorLogSink::~EditorLogSink()
    {
        // 析构函数
    }

    void EditorLogSink::Clear()
    {
        m_logEntries.clear();
    }

    void EditorLogSink::sink_it_(const spdlog::details::log_msg &msg)
    {

        spdlog::memory_buf_t formatted;
        formatter_->format(msg, formatted);
        m_logEntries.push_back(fmt::to_string(formatted));
    }

    void EditorLogSink::flush_()
    {

    }
}
