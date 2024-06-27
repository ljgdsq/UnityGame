//
// Created by zhengshulin on 2024/6/27.
//

#ifndef OPENGLPROJ_LOGGER_H
#define OPENGLPROJ_LOGGER_H

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

class Logger {
    static bool init;
public:
    static void Init(const std::vector<spdlog::sink_ptr>& custom_sinks = {});
public:
    template<typename... Args>
    static void log(const char* fmt, const Args&... args)
    {
        spdlog::info(fmt, args...);
    }

    template<typename... Args>
    static void warn(const char* fmt, const Args&... args)
    {
        spdlog::warn(fmt, args...);
    }

    template<typename... Args>
    static void error(const char* fmt, const Args&... args)
    {
        spdlog::error(fmt, args...);
    }
};



#endif //OPENGLPROJ_LOGGER_H
