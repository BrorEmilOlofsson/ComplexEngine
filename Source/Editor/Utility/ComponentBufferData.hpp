#pragma once
#include <any>

namespace CLX
{
    struct ComponentBufferData
    {
        bool anyItemActiveLastFrame = false;
        std::any storedComponent;
    };
}