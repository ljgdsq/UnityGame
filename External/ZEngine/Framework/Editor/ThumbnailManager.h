#pragma once
#include "Framework/Common/Define.h"
#include "Framework/Asset/Icons.h"
#include "Framework/Asset/TextureAsset.h"

namespace editor
{
    using namespace framework;
    class ThumbnailManager
    {
        SINGLETON_CLASS(ThumbnailManager);
        public:
        std::shared_ptr<TextureAsset> GetThumbnail(AssetType type);
    };
}
