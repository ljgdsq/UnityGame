#pragma once
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include <string>

class Logger
{
    static bool initialized;

public:
    static void Init(const std::vector<spdlog::sink_ptr> &custom_sinks = {});

public:
    template <typename... Args>
    static void Log(const char *fmt, const Args &...args)
    {
        spdlog::info(fmt, args...);
    }
    template <typename... Args>
    static void Warn(const char *fmt, const Args &...args)
    {
        spdlog::warn(fmt, args...);
    }

    template <typename... Args>
    static void Error(const char *fmt, const Args &...args)
    {
        spdlog::error(fmt, args...);
    }
};