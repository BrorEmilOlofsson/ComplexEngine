#include "Engine/Precompiled/EnginePch.hpp"
#include "CylinderComponent.hpp"
#include "Engine/Reflection/Reflection.hpp"

namespace Simple
{

    void RegisterCylinderComponent()
    {
    }

    REGISTER_COMPONENT(CylinderComponent);
    REGISTER_MEMBER(CylinderComponent::cylinder);
    REGISTER_MEMBER(CylinderComponent::color);

}