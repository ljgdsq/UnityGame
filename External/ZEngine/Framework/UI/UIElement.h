#pragma once
#include "Framework/Component/Component.h"
#include "glm/glm.hpp"
namespace framework
{
    class Renderer; // 前向声明渲染器类
    class UIElement : public ComponentBase<UIElement>
    {
    public:
        explicit UIElement(GameObject *gameObject) : ComponentBase<UIElement>(gameObject) {}

        virtual const char *GetName() const = 0;
        virtual rapidjson::Value Serialize() const = 0;
        virtual void Deserialize(const rapidjson::Value &jsonValue) = 0;

    protected:
        glm::vec2 sizeDelta = {100, 100}; // 尺寸
                                          // 渲染相关
        int sortingOrder = 0;             // 渲染顺序
        bool isVisible = true;            // 是否可见
        float alpha = 1.0f;               // 透明度

        // 交互相关
        bool raycastTarget = true; // 是否接收射线检测

    public:
        // 变换操作
        virtual glm::vec2 GetWorldPosition();
        virtual glm::vec2 GetSize();
        virtual void SetAnchoredPosition(glm::vec2 pos);
        virtual void SetSize(glm::vec2 size);

        // 渲染接口
        virtual void OnRender(Renderer *renderer) = 0;
        virtual bool IsVisible() const { return isVisible && alpha > 0; }
    };
}