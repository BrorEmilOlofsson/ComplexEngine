#pragma once
#include <vector>
#include <string>
#include <variant>

#include "Engine/ECS/EntityID.hpp"

namespace CLX
{

    using PropertyElement = std::variant<std::string, size_t>;

    struct PropertyPath final
    {
        EntityID entityID;

        std::vector<PropertyElement> elements;
    };
}