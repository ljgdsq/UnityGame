#pragma once
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include <string>
#include <filesystem>

class Logger
{
    static bool initialized;

public:
    static void Init(const std::vector<spdlog::sink_ptr> &custom_sinks = {});

private:
    // 提取文件名（去掉路径）
    static std::string ExtractFileName(const char *filePath)
    {
        std::filesystem::path path(filePath);
        return path.filename().string();
    }

public:
    template <typename... Args>
    static void Debug(const char *file, int line, const char *fmt, const Args &...args)
    {
        std::string location = "[" + ExtractFileName(file) + ":" + std::to_string(line) + "] ";
        spdlog::debug(location + fmt, args...);
    }

    template <typename... Args>
    static void Log(const char *file, int line, const char *fmt, const Args &...args)
    {
        std::string location = "[" + ExtractFileName(file) + ":" + std::to_string(line) + "] ";
        spdlog::info(location + fmt, args...);
    }

    template <typename... Args>
    static void Warn(const char *file, int line, const char *fmt, const Args &...args)
    {
        std::string location = "[" + ExtractFileName(file) + ":" + std::to_string(line) + "] ";
        spdlog::warn(location + fmt, args...);
    }

    template <typename... Args>
    static void Error(const char *file, int line, const char *fmt, const Args &...args)
    {
        std::string location = "[" + ExtractFileName(file) + ":" + std::to_string(line) + "] ";
        spdlog::error(location + fmt, args...);
    }

    // 兼容旧的日志方法，不显示文件名和行号

    template <typename... Args>
    static void Debug(const char *fmt, const Args &...args)
    {
        spdlog::debug(fmt, args...);
    }

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

#if _DEBUG

// 便利宏，自动传递文件名和行号
#define LOG_DEBUG(fmt, ...) Logger::Debug(__FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) Logger::Log(__FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) Logger::Warn(__FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) Logger::Error(__FILE__, __LINE__, fmt, ##__VA_ARGS__)

#else

#define LOG_DEBUG(fmt, ...) Logger::Debug(fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) Logger::Log(fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) Logger::Warn(fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) Logger::Error(fmt, ##__VA_ARGS__)

#endif