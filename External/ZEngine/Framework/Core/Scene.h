#pragma once

#include <string>
#include <memory>

namespace framework {

class Scene {
public:
    Scene() = default;
    virtual ~Scene() = default;

    virtual void Initialize() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Render() = 0;
    virtual void Shutdown() = 0;

    virtual const char* GetName() const = 0;
};

using ScenePtr = std::shared_ptr<Scene>;

} // namespace framework
