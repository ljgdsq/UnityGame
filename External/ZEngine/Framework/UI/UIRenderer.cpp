#include "Framework/UI/UIRenderer.h"

namespace framework
{
    UIRenderer *UIRenderer::instance = nullptr;

    UIRenderer *UIRenderer::GetInstance()
    {
        if (!instance)
        {
            instance = new UIRenderer();
        }
        return instance;
    }

    void UIRenderer::Render(UIElement *element)
    {
    
    }
} // namespace framework