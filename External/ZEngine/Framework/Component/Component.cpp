#include "Framework/Component/Component.h"
namespace framework
{
    void Component::SetEnabled(bool enabled)
    {
        if (isActive == enabled)
            return; // 如果状态没有变化，直接返回
        if (state == State::Destroyed)
        {
            Logger::Error("Cannot change enabled state of a destroyed component.");
            return;
        }

        isActive = enabled;
        if ((state == State::Created || state == State::Disabled) && enabled)
        {
            OnEnable();
        }
        else if ((state == State::Enabled || state == State::Updating || state == State::Started) && !enabled)
        {
            OnDisable();
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
        isActive = false;     // 禁用组件
        gameObject = nullptr; // Clear the reference to the GameObject
    }


    void Component::OnCreate()
    {
        Engine_Assert(state == State::None, "Component must be in None state to be created.");
        state = State::Created;
    }

    void Component::OnEnable()
    {
        Engine_Assert(state == State::Started || state == State::Disabled, "Component must be in Created or Disabled state to be enabled.");
        state = State::Enabled;
    }

    void Component::OnStart()
    {
        Engine_Assert(state == State::Created, "Component must be in Created state to start.");
        state = State::Started;
    }

    void Component::OnUpdate(float deltaTime)
    {
        Engine_Assert(state == State::Enabled, "Component must be in Enabled state to update.");
    }

    void Component::OnDisable()
    {
        Engine_Assert(state == State::Started || state == State::Enabled || state == State::Updating, "Component must be in Started or Enabled or Updating state to be disabled.");
        state = State::Disabled;
    }

    void Component::OnDestroy()
    {
        Engine_Assert(state != State::Destroyed, "Component is already destroyed.");
        state = State::Destroyed;
    }

} // namespace framework
