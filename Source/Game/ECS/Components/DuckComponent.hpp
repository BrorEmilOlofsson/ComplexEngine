#pragma once
#include <vector>
#include "Engine/ECS/EntityID.hpp"

struct DuckComponent final
{
    float speed = 1.f;
    std::vector<CLX::EntityID> ducklingIDs;
};

void RegisterDuckComponent();