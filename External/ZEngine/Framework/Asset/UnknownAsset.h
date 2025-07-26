#pragma once
#include "Framework/Asset/Asset.h"
namespace  framework
{
    class UnknownAsset : public Asset
    {
    public:
        UnknownAsset(const std::string &name);
        ~UnknownAsset() override = default;

        long GetSize() const override;

        rapidjson::Value Serialize(rapidjson::MemoryPoolAllocator<> &allocator) const override;

        void Deserialize(const rapidjson::Value &json) override;

        void GenerateThumbnail() override;

        void SetFileSize(unsigned int size);
        private:
        unsigned int m_fileSize = 0; // 文件大小，单位为字节
    };

} // namespace  framework


