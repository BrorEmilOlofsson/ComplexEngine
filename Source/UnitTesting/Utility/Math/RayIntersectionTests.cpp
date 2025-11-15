#include <External/Catch2/catch_amalgamated.hpp>

#include "Utility/Intersection.hpp"

using namespace Simple;

TEST_CASE("Ray Vs Sphere Intersection")
{
	{
		constexpr Ray3f ray(Point3f(0.f, 0, -10.f), UnitVector3f::Forward());
		constexpr Spheref sphere(5.f);

		auto result = IntersectionRaySphere(ray, sphere);

		REQUIRE(result.has_value());
		REQUIRE(result->hitPoint == Point3f(0, 0, -5.f));
	}

	{
		constexpr Ray3f ray(Point3f(-6.f, 0, -10.f), UnitVector3f::Forward());
		constexpr Spheref sphere(5.f);

		auto result = IntersectionRaySphere(ray, sphere);

		REQUIRE(!result.has_value());
	}
}


TEST_CASE("Ray Vs AABB Intersection")
{
	constexpr AABB3f aabb = AABB3f::FromMinAndMax(Point3f(0, 0, 0), Point3f(1, 1, 2));
	constexpr Ray3f ray(Point3f(0.3f, 0, -1.f), UnitVector3f::Forward());

	auto result = IntersectionRayAABB(ray, aabb);

	REQUIRE(result.has_value());
	REQUIRE(result->hitPoint == Point3f(0.3f, 0, 0));
}

TEST_CASE("Ray Vs Plane Intersection")
{
	{
		constexpr Ray3f ray(Point3f(1, 1, 1), UnitVector3f::Down());
		constexpr Planef plane;

		constexpr auto result = IntersectionRayPlane(ray, plane);
		REQUIRE(result.has_value());
		REQUIRE(result->hitPoint == Point3f(1, 0, 1));
	}
	{
		constexpr Ray3f ray(Point3f(1, 1, 1), UnitVector3f::Down());
		constexpr Planef plane(Point3f::Zero(), UnitVector3f::Down());

		constexpr auto result = IntersectionRayPlane(ray, plane);
		REQUIRE(result.has_value());
	}
}

TEST_CASE("Ray Vs Disk Intersection")
{
	constexpr Ray3f ray(Point3f(1, 1, -1), UnitVector3f::Forward());
	constexpr Diskf disk(Point3f::Zero(), UnitVector3f::Backward(), 5.f);

	const auto result = IntersectionRayDisk(ray, disk);
}

TEST_CASE("Ray Vs Triangle Intersection")
{
	constexpr Ray3f ray(Point3f(0.5f, 0.5f, -1.f), UnitVector3f::Forward());
	constexpr Triangle3f triangle(Point3f(0.f, 0.f, 0.f), Point3f(0.f, 1.f, 0.f), Point3f(1.f, 1.f, 0.f));
	const auto result = IntersectionRayTriangle(ray, triangle);
	REQUIRE(result.has_value());
	REQUIRE(result->hitPoint == Point3f(0.5f, 0.5f, 0.f));
}