
#include <iostream>

using std::string;
#include "spdlog/spdlog.h"

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

int main() {

    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::debug);

    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/engine_editor.log",false);
    file_sink->set_level(spdlog::level::warn);
    std::vector<spdlog::sink_ptr> sinks{console_sink, file_sink};
    auto logger = std::make_shared<spdlog::logger>("editor_logger", sinks.begin(), sinks.end() );

    spdlog::set_default_logger(logger);
    spdlog::info("some info log");
    spdlog::error("critical issue");

    return 0;
}

