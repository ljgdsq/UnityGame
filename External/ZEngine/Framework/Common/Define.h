#pragma once







#define SINGLETON_CLASS(className) \
    private: \
        className() = default; \
        ~className() = default; \
    public: \
        static className& GetInstance() { \
            static className instance; \
            return instance; \
        } \
    private: \
        className(const className&) = delete; \
        className& operator=(const className&) = delete; \
        className(className&&) = delete; \
        className& operator=(className&&) = delete; \
