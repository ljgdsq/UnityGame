#pragma once
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

class Logger
{
    static bool initialized;
public:
    const std::vector<spdlog::sink_ptr>& custom_sinks = {}
public:
    template <typename... Args>
    static void Log(const char *fmt, const Args &...args)
    {
        spdlog::info(fmt, args...);
    }
    template <typename... Args>
    static void warn(const char *fmt, const Args &...args)
    {
        spdlog::warn(fmt, args...);
    }

    template <typename... Args>
    static void error(const char *fmt, const Args &...args)
    {
        spdlog::error(fmt, args...);
    }
};