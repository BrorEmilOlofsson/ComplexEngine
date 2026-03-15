#pragma once

struct DuckComponent final
{
    float speed = 1.f;
    std::vector<CLX::EntityID> ducklingIDs;
};

void RegisterDuckComponent();