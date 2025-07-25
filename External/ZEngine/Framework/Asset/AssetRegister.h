#pragma once
#include "Framework/Asset/AssetManager.h"
// 在类定义中使用的宏
#define REGISTER_LOADER(LoaderClass)                                                  \
    namespace                                                                         \
    {                                                                                 \
        class LoaderClass##AutoRegister                                               \
        {                                                                             \
        public:                                                                       \
            LoaderClass##AutoRegister()                                               \
            {                                                                         \
                framework::AssetManager::GetInstance().RegisterLoader<LoaderClass>(); \
            }                                                                         \
        };                                                                            \
        static LoaderClass##AutoRegister g_##LoaderClass##Instance;                   \
    }

// #define REGISTER_LOADER(LoaderClass)
