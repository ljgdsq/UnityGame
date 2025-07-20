#pragma once
#include "Framework/Component/Component.h"
#include "Framework/Core/GameObject.h"
#include "Framework/Component/Transform.h"
namespace framework
{

    enum class LightType
    {
        Directional,
        Point,
        Spot
    };
    class Light;

    struct LightData
    {
        // 光源类型
        LightType type; // 光源类型 (方向光、点光源、聚光灯)

        // 位置信息
        glm::vec3 position;  // 点光源/聚光灯的世界位置
        glm::vec3 direction; // 方向光/聚光灯的方向向量

        // 光照属性
        glm::vec3 color; // 光源颜色 (RGB)
        float intensity; // 光强

        // 衰减参数
        float range;     // 影响范围 (点光源/聚光灯)
        float constant;  // 常数衰减
        float linear;    // 线性衰减
        float quadratic; // 二次衰减

        // 聚光灯特有参数
        float spotInnerAngle; // 内锥角 (cos值)
        float spotOuterAngle; // 外锥角 (cos值)

        // 阴影相关 (未来扩展)
        int castShadows;  // 是否投射阴影
        float shadowBias; // 阴影偏移

        // 构造函数
        // LightData();
        // static LightData FromLight(const Light* light);
    };

    class Light : public ComponentBase<Light>
    {
    private:
        LightType type;
        glm::vec3 color;
        float intensity;

    public:
        Light(GameObject *gameObject)
            : ComponentBase<Light>(gameObject), type(LightType::Directional), color(glm::vec3(1.0f)), intensity(1.0f) {}

        virtual ~Light() = default;
        // Returns the type of the light
        LightType GetLightType() const { return type; }
        // Sets the type of the light
        void SetLightType(LightType lightType) { type = lightType; }
        // Returns the color of the light
        const glm::vec3 &GetColor() const { return color; }
        // Sets the color of the light
        void SetColor(const glm::vec3 &lightColor)
        {
            color = lightColor;
        }
        // Returns the intensity of the light
        float GetIntensity() const { return intensity; }
        // Sets the intensity of the light
        void SetIntensity(float lightIntensity) { intensity = lightIntensity; }

        // Component lifecycle
        virtual void OnEnable() override;
        virtual void OnDisable() override;

        // serialization
        virtual rapidjson::Value Serialize(rapidjson::Document::AllocatorType &allocator) const override;
        virtual void Deserialize(const rapidjson::Value &jsonValue) override;
    };
}
