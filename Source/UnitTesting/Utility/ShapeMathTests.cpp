#include <External/Catch2/catch_amalgamated.hpp>

#include "Utility/ShapeMath.hpp"

using namespace Simple;

TEST_CASE("Get Triangle Edge Normals")
{
	constexpr Triangle3f triangle(Point3f(0.0f, 0.0f, 0.0f), Point3f(0.0f, 1.0f, 0.0f), Point3f(1.0f, 1.0f, 0.0f));

	const auto a = GetEdgeNormals(triangle);

	REQUIRE(a[0] == UnitVector3f(-1.0f, 0.0f, 0.0f));
	REQUIRE(a[1] == UnitVector3f(0.0f, 1.0f, 0.0f));
	REQUIRE(NearlyEqual(a[2], UnitVector3f(1.f, -1.f, 0.f)));
}

TEST_CASE("Get Intersected Line")
{
	constexpr Planef plane1(Point3f(0.0f, 0.0f, 0.0f), UnitVector3f(0.0f, 1.0f, 0.0f));
	constexpr Planef plane2(Point3f(0.0f, 1.0f, 0.0f), UnitVector3f(1.0, 0.0f, 0.0f));

	constexpr std::optional<Line3f> line = GetIntersectionLine(plane1, plane2);

	REQUIRE(line.has_value());
	REQUIRE((line.value().GetDirection() == UnitVector3f::Forward() || line.value().GetDirection() == UnitVector3f::Backward()));
}

TEST_CASE("Plane GetProjectedPoint")
{
	constexpr Planef plane;

	constexpr Point3f p = GetProjectedPoint(plane, Point3f(10, 10, 7.4f));
	REQUIRE(p == Point3f(10.f, 0, 7.4f));
}

TEST_CASE("Line3 GetProjectedPoint")
{
	constexpr Line3f line(Point3f(0, 2, 2.5f), UnitVector3f::Right());

	constexpr Point3f p = GetProjectedPoint(line, Point3f(10, 10, 7.4f));
	REQUIRE(p == Point3f(10.f, 2.f, 2.5f));
}

TEST_CASE("Line2 GetProjectedPoint")
{
	constexpr Line2f line(Point2f(0, 2), UnitVector2f::Right());

	constexpr Point2f p = GetProjectedPoint(line, Point2f(10, 10));
	REQUIRE(p == Point2f(10.f, 2.f));
}

TEST_CASE("Get Overlap AABB2")
{
	constexpr AABB2f a = AABB2f::CreateFromMinAndMax(Point2f(0, 0), Point2f(5, 5));
	constexpr AABB2f b = AABB2f::CreateFromMinAndMax(Point2f(3, 3), Point2f(7, 7));
	constexpr std::optional<AABB2f> overlap = GetOverlap(a, b);
	REQUIRE(overlap.has_value());
	REQUIRE(overlap->GetMin() == Point2f(3.f, 3.f));
	REQUIRE(overlap->GetMax() == Point2f(5.f, 5.f));
}

TEST_CASE("Get Overlap AABB3")
{
	constexpr AABB3f a = AABB3f::CreateFromMinAndMax(Point3f(0, 0, 0), Point3f(5, 5, 5));
	constexpr AABB3f b = AABB3f::CreateFromMinAndMax(Point3f(3, 3, 3), Point3f(7, 7, 7));
	constexpr std::optional<AABB3f> overlap = GetOverlap(a, b);
	REQUIRE(overlap.has_value());
	REQUIRE(overlap->GetMin() == Point3f(3.f, 3.f, 3.f));
	REQUIRE(overlap->GetMax() == Point3f(5.f, 5.f, 5.f));
}

TEST_CASE("Get Area AABB2")
{
	constexpr AABB2f aabb = AABB2f::CreateFromMinAndMax(Point2f(0, 0), Point2f(5, 5));
	constexpr float area = GetArea(aabb);
	REQUIRE(area == 25.0f);
}

TEST_CASE("Get Volume AABB3")
{
	constexpr AABB3f aabb = AABB3f::CreateFromMinAndMax(Point3f(0, 0, 0), Point3f(5, 5, 5));
	constexpr float volume = GetVolume(aabb);
	REQUIRE(volume == 125.0f);
}

TEST_CASE("Get Overlap Percentage AABB2")
{
	constexpr AABB2f a = AABB2f::CreateFromMinAndMax(Point2f(0, 0), Point2f(5, 5));
	constexpr AABB2f b = AABB2f::CreateFromMinAndMax(Point2f(2.5f, 2.5f), Point2f(7, 7));
	constexpr Percentf overlapPercentage = GetOverlapPercentage(a, b);
	REQUIRE(overlapPercentage == Percentf(0.25f));
}

TEST_CASE("Get Overlap Percentage AABB3")
{
	constexpr AABB3f a = AABB3f::CreateFromMinAndMax(Point3f(0, 0, 0), Point3f(5, 5, 5));
	constexpr AABB3f b = AABB3f::CreateFromMinAndMax(Point3f(2.5f, 2.5f, 2.5f), Point3f(7, 7, 7));
	constexpr Percentf overlapPercentage = GetOverlapPercentage(a, b);
	REQUIRE(overlapPercentage == Percentf(0.125f));
}

TEST_CASE("Get Overlap Percentage AABB2 No Overlap")
{
	constexpr AABB2f a = AABB2f::CreateFromMinAndMax(Point2f(0, 0), Point2f(5, 5));
	constexpr AABB2f b = AABB2f::CreateFromMinAndMax(Point2f(6, 6), Point2f(10, 10));
	constexpr Percentf overlapPercentage = GetOverlapPercentage(a, b);

	REQUIRE(overlapPercentage == Percentf(0.0f));
}

TEST_CASE("Get Overlap Percentage AABB3 No Overlap")
{
	constexpr AABB3f a = AABB3f::CreateFromMinAndMax(Point3f(0, 0, 0), Point3f(5, 5, 5));
	constexpr AABB3f b = AABB3f::CreateFromMinAndMax(Point3f(6, 6, 6), Point3f(10, 10, 10));
	constexpr Percentf overlapPercentage = GetOverlapPercentage(a, b);
	REQUIRE(overlapPercentage == Percentf(0.0f));
}

TEST_CASE("Get Overlap Percentage AABB2 Full Overlap")
{
	constexpr AABB2f a = AABB2f::CreateFromMinAndMax(Point2f(0, 0), Point2f(5, 5));
	constexpr AABB2f b = AABB2f::CreateFromMinAndMax(Point2f(1, 1), Point2f(4, 4));
	constexpr Percentf overlapPercentage = GetOverlapPercentage(a, b);
	REQUIRE(overlapPercentage == Percentf(0.36f));
}

TEST_CASE("Get Overlap Percentage AABB3 Full Overlap")
{
	constexpr AABB3f a = AABB3f::CreateFromMinAndMax(Point3f(0, 0, 0), Point3f(5, 5, 5));
	constexpr AABB3f b = AABB3f::CreateFromMinAndMax(Point3f(1, 1, 1), Point3f(4, 4, 4));
	constexpr Percentf overlapPercentage = GetOverlapPercentage(a, b);
	REQUIRE(overlapPercentage == Percentf(0.216f));
}

TEST_CASE("Get Area Triangle2")
{
	{
		constexpr Triangle2f triangle(Point2f(0.0f, 0.0f), Point2f(0.0f, 1.0f), Point2f(1.0f, 1.0f));
		constexpr float area = GetArea(triangle);
		REQUIRE(area == 0.5f);
	}

	{
		constexpr Triangle2f triangle(Point2f(0.0f, 0.0f), Point2f(-1.0f, 1.0f), Point2f(1.0f, 1.0f));
		constexpr float area = GetArea(triangle);
		REQUIRE(area == 1.f);
	}

	{
		constexpr AABB3i aabb = AABB3i::CreateFromMinAndMax(Point3i(0, 0, 0), Point3i(5, 5, 5));

		const auto corners = GetCorners<int, std::vector<Point3i>>(aabb);
	}
}