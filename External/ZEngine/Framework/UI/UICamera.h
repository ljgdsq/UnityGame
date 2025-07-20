#pragma once
#include "Framework/Component/Camera.h"
namespace framework
{

    class UICamera : public Camera
    {
    public:
        explicit UICamera(GameObject *gameObject) : Camera(gameObject) {}

        // 返回组件名称
        const char *GetName() const override { return "UICamera"; }

        // 序列化为JSON值
        rapidjson::Value Serialize(rapidjson::Document::AllocatorType &allocator) const override;

        // 从JSON值反序列化
        void Deserialize(const rapidjson::Value &jsonValue) override;

        // // 屏幕坐标转换
        // glm::vec2 ScreenToUIPoint(glm::vec2 screenPos);
        // glm::vec2 UIPointToScreen(glm::vec2 uiPos);

    public:
        const char *GetTypeName() const override { return "UICamera"; }
        const std::type_info &GetTypeInfo() const override { return typeid(UICamera); }
    };
}
