#pragma once
#include <string>
#include "Framework/Common/Define.h"
namespace framework
{
    class GameObject;
} // namespace framework
namespace editor
{
    class ObjectCreator
    {
        STATIC_CLASS(ObjectCreator);

    public:
        static framework::GameObject *CreateCube(const std::string &name);
    };

} // namespace editor
