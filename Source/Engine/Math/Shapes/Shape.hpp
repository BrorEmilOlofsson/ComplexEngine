#pragma once
#include <variant>
#include "Engine/Math/Shapes/Sphere.hpp"
#include "Engine/Math/Shapes/AABB3.hpp"

namespace CLX
{

    using Shape = std::variant<Spheref, AABB3f>;
}