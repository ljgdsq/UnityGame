#pragma once
#include "Framework/Editor/AssetInspectorRegistry.h"

#define REGISTER_ASSET_INSPECTOR(assetType, inspectorClass) \
    class inspectorClass##AutoRegister \
    { \
    public: \
        inspectorClass##AutoRegister() \
        { \
            AssetInspectorRegistry::GetInstance().RegisterInspector(assetType, new inspectorClass()); \
        } \
    }; \
    static inspectorClass##AutoRegister g_##inspectorClass##AutoRegister;
