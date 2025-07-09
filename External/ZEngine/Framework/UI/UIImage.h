#pragma once
#include "Framework/UI/UIElement.h"
#include "Framework/Renderer/Texture.h"
namespace framework
{
    class UIImage : public UIElement
    {
    public:
        explicit UIImage(GameObject *gameObject) : UIElement(gameObject) {}

        const char *GetName() const override { return "UIImage"; }

        rapidjson::Value Serialize() const override;

        void Deserialize(const rapidjson::Value &jsonValue) override;

        void OnRender(Renderer *renderer) override;

    private:
        Texture* texture; // 纹理资源
        glm::vec2 size = {100, 100};       // 图片尺寸
    };
} // namespace framework