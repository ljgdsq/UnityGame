#include "Framework/Asset/Icons.h"
namespace framework
{
    const char *Icons::GetIconPath(IconType type)
    {
        switch (type)
        {
        case IconType::Folder:
            return "Res/Internal/Icons/Folder.png";
        case IconType::File:
            return "Res/Internal/Icons/File.png";
        case IconType::Shader:
            return "Res/Internal/Icons/shader.png";
        case IconType::Texture:
            return "Res/Internal/Icons/texture.png";
        case IconType::Material:
            return "Res/Internal/Icons/material.png";
        case IconType::Scene:
            return "Res/Internal/Icons/scene.png";
        case IconType::Script:
            return "Res/Internal/Icons/script.png";
        case IconType::Audio:
            return "Res/Internal/Icons/audio.png";
        case IconType::Video:
            return "Res/Internal/Icons/video.png";
        case IconType::Model:
            return "Res/Internal/Icons/model.png";
        case IconType::Light:
            return "Res/Internal/Icons/light.png";
        case IconType::Camera:
            return "Res/Internal/Icons/camera.png";
        case IconType::Animation:
            return "Res/Internal/Icons/animation.png";
        default:
            return "Res/Internal/Icons/unknown.png";
        }
    }

    const char *Icons::GetIconPath(AssetType type){
        switch (type)
        {
        case AssetType::Unknown:
            return GetIconPath(IconType::None);
        case AssetType::Shader:
            return GetIconPath(IconType::Shader);
        case AssetType::Texture:
            return GetIconPath(IconType::Texture);
        case AssetType::Material:
            return GetIconPath(IconType::Material);
        case AssetType::Scene:
            return GetIconPath(IconType::Scene);
        case AssetType::Script:
            return GetIconPath(IconType::Script);
        case AssetType::Audio:
            return GetIconPath(IconType::Audio);
        case AssetType::Video:
            return GetIconPath(IconType::Video);
        default:
            return GetIconPath(IconType::None);
        }
    }
}
