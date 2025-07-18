#include "Framework/Component/Component.h"
namespace framework
{
    void Component::SetEnabled(bool enabled)
    {
        if (isEnabled == enabled)
            return; // 如果状态没有变化，直接返回
        if (state == State::Destroyed)
        {
            Logger::Error("Cannot change enabled state of a destroyed component.");
            return;
        }

        isEnabled = enabled;
        if ((state == State::Created || state == State::Disabled) && enabled)
        {
            OnEnable();
            state = State::Enabled;
        }
        else if ((state == State::Enabled || state == State::Updating || state == State::Started) && !enabled)
        {
            OnDisable();
            state = State::Disabled;
        }

    }

    void Component::Destroy()
    {
        if (state == State::Destroyed)
        {
            Logger::Error("Component is already destroyed.");
            return;
        }

        OnDestroy();
        state = State::Destroyed;
        isEnabled = false; // 禁用组件
        gameObject = nullptr; // Clear the reference to the GameObject
    }

} // namespace framework
