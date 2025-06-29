#include "Logger.h"
bool Logger::initialized = false;
void Logger::Init(const std::vector<spdlog::sink_ptr> &custom_sinks)
{
    if (initialized)
        return;
    initialized = true;

    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::debug);

    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/engine_editor.log", false);
    file_sink->set_level(spdlog::level::info);

    std::vector<spdlog::sink_ptr> sinks{console_sink, file_sink};
    sinks.insert(sinks.end(), custom_sinks.begin(), custom_sinks.end());

    auto logger = std::make_shared<spdlog::logger>("", sinks.begin(), sinks.end());

    spdlog::set_default_logger(logger);
    spdlog::set_level(spdlog::level::debug);
}