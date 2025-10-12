#pragma once
#include "Utility/Shapes/Sphere.hpp"
#include "Utility/Shapes/AABB3.hpp"
#include "Utility/Shapes/Ray3.hpp"
#include <variant>

namespace Simple
{
	using CollisionShape = std::variant<Spheref, AABB3f, Ray3f>;

}