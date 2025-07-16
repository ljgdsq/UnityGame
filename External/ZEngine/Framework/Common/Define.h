#pragma once


// 定义一个宏，用于禁止类的实例化和拷贝
// 适用于单例类或工具类，确保只能通过静态方法访问
#define STATIC_CLASS(name)                  \
private:                                     \
    name() = delete;                        \
    ~name() = delete;                       \
    name(const name &) = delete;            \
    name &operator=(const name &) = delete; \
    name(name &&) = delete;                 \
    name &operator=(name &&) = delete;\
