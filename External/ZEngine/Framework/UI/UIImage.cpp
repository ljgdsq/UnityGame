#include "Framework/UI/UIImage.h"

namespace framework {

}
rapidjson::Value framework::UIImage::Serialize() const
{
    return rapidjson::Value();
}

void framework::UIImage::Deserialize(const rapidjson::Value &jsonValue)
{
}

void framework::UIImage::OnRender(Renderer *renderer)
{
}
