#include <External/Catch2/catch_amalgamated.hpp>

#include "Utility/Intersection.hpp"

using namespace Simple;

TEST_CASE("Ray Vs Sphere Intersection")
{
	{
		constexpr Simple::Ray3f ray(Simple::Point3f(0.f, 0, -10.f), Simple::UnitVector3f::Forward());
		constexpr Simple::Spheref sphere(5.f);

		auto result = Simple::IntersectionRaySphere(ray, sphere);

		REQUIRE(result.has_value());
		REQUIRE(result->hitPoint == Simple::Point3f(0, 0, -5.f));
	}

	{
		constexpr Simple::Ray3f ray(Simple::Point3f(-6.f, 0, -10.f), Simple::UnitVector3f::Forward());
		constexpr Simple::Spheref sphere(5.f);

		auto result = Simple::IntersectionRaySphere(ray, sphere);

		REQUIRE(!result.has_value());
	}
}


TEST_CASE("Ray Vs AABB Intersection")
{
	constexpr Simple::AABB3f aabb = Simple::AABB3f::CreateFromMinAndMax(Simple::Point3f(0, 0, 0), Simple::Point3f(1, 1, 2));
	constexpr Simple::Ray3f ray(Simple::Point3f(0.3f, 0, -1.f), Simple::UnitVector3f::Forward());

	auto result = Simple::IntersectionRayAABB(ray, aabb);

	REQUIRE(result.has_value());
	REQUIRE(result->hitPoint == Simple::Point3f(0.3f, 0, 0));
}

TEST_CASE("Ray Vs Plane Intersection")
{
	{
		constexpr Simple::Ray3f ray(Simple::Point3f(1, 1, 1), Simple::UnitVector3f::Down());
		constexpr Simple::Planef plane;

		constexpr auto result = Simple::IntersectionRayPlane(ray, plane);
		REQUIRE(result.has_value());
		REQUIRE(result->hitPoint == Simple::Point3f(1, 0, 1));
	}
	{
		constexpr Simple::Ray3f ray(Simple::Point3f(1, 1, 1), Simple::UnitVector3f::Down());
		constexpr Simple::Planef plane(Simple::Point3f::Zero(), Simple::UnitVector3f::Down());

		constexpr auto result = Simple::IntersectionRayPlane(ray, plane);
		REQUIRE(result.has_value());
	}
}

TEST_CASE("Ray Vs Disk Intersection")
{
	constexpr Simple::Ray3f ray(Simple::Point3f(1, 1, -1), Simple::UnitVector3f::Forward());
	constexpr Simple::Diskf disk(Simple::Point3f::Zero(), Simple::UnitVector3f::Backward(), 5.f);

	const auto result = Simple::IntersectionRayDisk(ray, disk);
}

TEST_CASE("Ray Vs Triangle Intersection")
{
	constexpr Simple::Ray3f ray(Simple::Point3f(0.5f, 0.5f, -1.f), Simple::UnitVector3f::Forward());
	constexpr Simple::Triangle3f triangle(Simple::Point3f(0.f, 0.f, 0.f), Simple::Point3f(0.f, 1.f, 0.f), Simple::Point3f(1.f, 1.f, 0.f));
	const auto result = Simple::IntersectionRayTriangle(ray, triangle);
	REQUIRE(result.has_value());
	REQUIRE(result->hitPoint == Simple::Point3f(0.5f, 0.5f, 0.f));
}