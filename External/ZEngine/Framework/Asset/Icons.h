#pragma once
#include "Framework/Common/Define.h"
#include "Framework/Asset/Asset.h"
namespace framework
{
    // 图标资源的枚举类型
    enum class IconType
    {
        None = 0,
        Folder,
        File,
        Shader,
        Texture,
        Material,
        Scene,
        Script,
        Audio,
        Video,
        Model,
        Light,
        Camera,
        Animation
    };

    // 图标资源类
    class Icons
    {
        STATIC_CLASS(Icons);
    public:
        static const char* GetIconPath(IconType type);
        static const char* GetIconPath(AssetType type);
    };
} // namespace framework
