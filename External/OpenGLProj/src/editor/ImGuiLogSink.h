//
// Created by zhengshulin on 2024/6/27.
//

#ifndef OPENGLPROJ_IMGUILOGSINK_H
#define OPENGLPROJ_IMGUILOGSINK_H

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/base_sink.h"
class ImGuiLogSink : public spdlog::sinks::base_sink<std::mutex>
{
public:
    ImGuiLogSink() : buffer() {}

    std::vector<std::string> buffer;
    void clear(){
        buffer.clear();
    }
protected:
    void sink_it_(const spdlog::details::log_msg &msg) override
    {
        spdlog::memory_buf_t formatted;
        formatter_->format(msg, formatted);
        buffer.push_back(fmt::to_string(formatted));
    }

    void flush_() override
    {
        // No flushing needed for this sink
    }
};


#endif //OPENGLPROJ_IMGUILOGSINK_H
