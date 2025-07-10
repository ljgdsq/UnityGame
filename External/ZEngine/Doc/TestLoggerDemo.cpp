#include "Framework/Log/Logger.h"

void TestLoggerFunctionality() {
    // 测试新的带文件名和行号的日志宏
    LOG_INFO("测试信息日志，这行应该显示文件名和行号");
    LOG_DEBUG("这是一个调试消息");
    LOG_WARN("这是一个警告消息");
    LOG_ERROR("这是一个错误消息");
    
    // 测试传统的日志方法（向后兼容）
    LoggerCompat::Log("这是传统的日志方法，不显示文件名行号");
    
    // 测试格式化字符串
    int value = 42;
    std::string text = "Hello";
    LOG_INFO("格式化测试: value = {}, text = '{}'", value, text);
}
