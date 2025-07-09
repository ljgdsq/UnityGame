#pragma once
#include "Framework/UI/UICanvas.h"

namespace framework
{
    class UIRenderer
    {
        private:
        static UIRenderer *instance; // 单例实例
        static UIRenderer *GetInstance();
    public:
        void Render(UIElement *element);
    };
} // namespace framework
