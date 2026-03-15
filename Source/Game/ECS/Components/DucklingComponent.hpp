#pragma once
#include "Engine/ECS/EntityID.hpp"

struct DucklingComponent final
{
    float speed = 1.f;
};


void RegisterDucklingComponent();