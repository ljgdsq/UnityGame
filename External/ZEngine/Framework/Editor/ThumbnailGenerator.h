#pragma once
#include <memory>
#include "Framework/Common/Define.h"
#include "Framework/Editor/ThumbnailConfig.h"
#include <unordered_map>
#include <string>
#include "Framework/Graphic/Buffer.h"
namespace framework
{
    class MeshAsset;
    class ShaderAsset;
    class TextureAsset;
    class Asset;
    class Shader;
}

namespace editor
{
    using namespace framework;
    class ThumbnailGenerator
    {

        SINGLETON_CLASS(ThumbnailGenerator);

    public:
       std::shared_ptr<framework::TextureAsset> GenerateThumbnail(std::shared_ptr<framework::Asset> asset, const ThumbnailConfig &config);

    private:
        std::shared_ptr<framework::TextureAsset> GenerateMeshThumbnail(std::shared_ptr<framework::MeshAsset> mesh, const ThumbnailConfig &config);
        void SetupMatrices(const ThumbnailConfig &config);
        void SetupFrameBuffer(const ThumbnailConfig &config, const std::string &name);

        std::shared_ptr<framework::Shader> m_thumbnailShader;

        std::unordered_map<std::string, std::unique_ptr<FrameBuffer>> m_frameBuffers;
    };
} // namespace editor
