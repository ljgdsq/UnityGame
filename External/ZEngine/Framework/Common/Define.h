#pragma once

#define SINGLETON_CLASS(className)                    \
private:                                              \
    className() = default;                            \
    ~className() = default;                           \
                                                      \
public:                                               \
    static className &GetInstance()                   \
    {                                                 \
        static className instance;                    \
        return instance;                              \
    }                                                 \
                                                      \
private:                                              \
    className(const className &) = delete;            \
    className &operator=(const className &) = delete; \
    className(className &&) = delete;                 \
    className &operator=(className &&) = delete;

// 定义一个宏，用于禁止类的实例化和拷贝
// 适用于单例类或工具类，确保只能通过静态方法访问
#define STATIC_CLASS(name)                  \
private:                                    \
    name() = delete;                        \
    ~name() = delete;                       \
    name(const name &) = delete;            \
    name &operator=(const name &) = delete; \
    name(name &&) = delete;                 \
    name &operator=(name &&) = delete;

#ifdef _DEBUG

#define Engine_Assert(condition, message)            \
    if (!(condition))                                \
    {                                                \
        Logger::Error("Assertion failed: " message); \
        std::terminate();                            \
    }

#else

#define Engine_Assert(condition, message) \
    () // 在非调试模式下，断言不会执行任何操作

#endif // _DEBUG
