#include "Engine/Precompiled/EnginePch.hpp"
#include "TriggerComponent.hpp"
#include "Engine/Reflection/Reflection.hpp"

namespace CLX
{

    REGISTER_COMPONENT(TriggerComponent);
    REGISTER_MEMBER(TriggerComponent::shape);
    REGISTER_MEMBER(TriggerComponent::debugColor);


    void RegisterTriggerComponent() {}
}