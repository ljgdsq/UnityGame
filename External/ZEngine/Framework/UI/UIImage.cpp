#include "Framework/UI/UIImage.h"

namespace framework {

}
rapidjson::Value framework::UIImage::Serialize(rapidjson::Document::AllocatorType &allocator) const
{
    // rapidjson::Value jsonValue(rapidjson::kObjectType);
    // jsonValue.AddMember("imagePath", rapidjson::Value(texture->, allocator), allocator);
    // return jsonValue;

    return rapidjson::Value();
}

void framework::UIImage::Deserialize(const rapidjson::Value &jsonValue)
{
}

void framework::UIImage::OnRender(Renderer *renderer)
{
}
