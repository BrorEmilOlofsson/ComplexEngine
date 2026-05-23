#pragma once
#include "Engine/Reflection/JsonAny.hpp"

namespace CLX
{
    struct ComponentBufferData
    {
        bool anyItemActiveLastFrame = false;
        JsonAny storedComponent;
    };
}