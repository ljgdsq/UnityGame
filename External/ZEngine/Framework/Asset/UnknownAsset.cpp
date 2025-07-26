
#include "rapidjson/document.h"
#include "Framework/Asset/UnknownAsset.h"

namespace framework
{
    UnknownAsset::UnknownAsset(const std::string &name): Asset(name,AssetType::Unknown){}

    long UnknownAsset::GetSize() const
    {
        return m_fileSize;
    }

    rapidjson::Value UnknownAsset::Serialize(rapidjson::MemoryPoolAllocator<> &allocator) const
    {
        return Asset::Serialize(allocator);
    }

    void UnknownAsset::Deserialize(const rapidjson::Value &json)
    {
        Asset::Deserialize(json);
    }

    void UnknownAsset::GenerateThumbnail()
    {
    }
    void UnknownAsset::SetFileSize(unsigned int size)
    {
        m_fileSize = size;
    }
}
