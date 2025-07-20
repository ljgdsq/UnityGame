#pragma once
#include "Framework/UI/UICamera.h"
#include "Framework/Component/Component.h"
#include <vector>
namespace framework
{
    enum class CanvasRenderMode
    {
        ScreenSpaceOverlay, // 屏幕空间覆盖
        ScreenSpaceCamera,  // 屏幕空间相机
        WorldSpace          // 世界空间
    };

    class UIElement; // 前向声明UI元素类

    class UICanvas : public ComponentBase<UICanvas>
    {
    public:
        void AddUIElement(UIElement *element)
        {
            uiElements.push_back(element);
        }
        void RemoveUIElement(UIElement *element)
        {
            uiElements.erase(std::remove(uiElements.begin(), uiElements.end(), element), uiElements.end());
        }

        virtual const char *GetName() const override
        {
            return "UICanvas";
        }

        virtual rapidjson::Value Serialize(rapidjson::Document::AllocatorType &allocator) const override
        {

            // 实现序列化逻辑
            rapidjson::Value jsonValue(rapidjson::kObjectType);
            jsonValue.AddMember("renderMode", static_cast<int>(renderMode), allocator);
            jsonValue.AddMember("sortingOrder", sortingOrder, allocator);
            // 序列化UI元素等
            return jsonValue;
        }

        virtual void Deserialize(const rapidjson::Value &jsonValue) override
        {
            // 实现反序列化逻辑
            if (jsonValue.HasMember("renderMode"))
            {
                renderMode = static_cast<CanvasRenderMode>(jsonValue["renderMode"].GetInt());
            }
            if (jsonValue.HasMember("sortingOrder"))
            {
                sortingOrder = jsonValue["sortingOrder"].GetInt();
            }
            // 反序列化UI元素等
        }

    private:
        CanvasRenderMode renderMode = CanvasRenderMode::ScreenSpaceOverlay; // 默认渲染模式
        UICamera *uiCamera = nullptr;                                       // 关联的UI摄像机
        int sortingOrder = 0;                                               // 渲染顺序
        std::vector<UIElement *> uiElements;                                // UI元素列表
    };
}
