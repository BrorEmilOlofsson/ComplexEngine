#include <External/Catch2/catch_amalgamated.hpp>

#include "Physics/PhysicsCollision.hpp"


TEST_CASE("Physics::Sphere Vs Cylinder")
{


	constexpr Simple::Spheref sphere = Simple::Spheref::FromCenterAndRadius(Simple::Point3f(0.f, 0.f, 0.f), 1.f);
	constexpr Simple::Cylinderf cylinder(Simple::Point3f(0.f, 2.0000f, 0.f), 1.f, Simple::UnitVector3f::Right(), 2.f);
	constexpr bool result = Simple::DetectCollision(sphere, cylinder);
	REQUIRE(result == true);
}