#include <External/Catch2/catch_amalgamated.hpp>

#include "Engine/Math/DynamicPoint.hpp"
#include "Engine/Math/DynamicVector.hpp"
#include "Engine/Math/DynamicUnitVector.hpp"

using namespace CLX;

TEST_CASE("DynamicPoint::Default Constructor", "[DynamicPoint]")
{
	const DynamicPointf point;
	REQUIRE(point.GetDimensionCount() == 0);
}

TEST_CASE("DynamicPoint::CreateFromDimensionCount", "[DynamicPoint]")
{
	constexpr std::size_t dimensionCount = 3;
	const DynamicPointf point = DynamicPointf::CreateFromDimensionCount(dimensionCount);
	REQUIRE(point.GetDimensionCount() == dimensionCount);
	REQUIRE(point[0] == 0.0f);
	REQUIRE(point[1] == 0.0f);
	REQUIRE(point[2] == 0.0f);
}

TEST_CASE("DynamicPoint::CreateFromInitializerList", "[DynamicPoint]")
{
	const DynamicPointf point = DynamicPointf::CreateFromValues({ 1.0f, 2.0f, 3.0f });
	REQUIRE(point.GetDimensionCount() == 3);
	REQUIRE(point[0] == 1.0f);
	REQUIRE(point[1] == 2.0f);
	REQUIRE(point[2] == 3.0f);
}

TEST_CASE("DynamicPoint::CreateFromVariadic", "[DynamicPoint]")
{
	const DynamicPointf point = DynamicPointf::CreateFromValues(1.0f, 2.0f, 3.0f);
	REQUIRE(point.GetDimensionCount() == 3);
	REQUIRE(point[0] == 1.0f);
	REQUIRE(point[1] == 2.0f);
	REQUIRE(point[2] == 3.0f);
}

TEST_CASE("DynamicPoint::SetDimensionCount", "[DynamicPoint]")
{
	DynamicPointf point = DynamicPointf::CreateFromValues(1.0f, 2.0f);
	REQUIRE(point.GetDimensionCount() == 2);
	point.SetDimensionCount(4);
	REQUIRE(point.GetDimensionCount() == 4);
	REQUIRE(point[0] == 1.0f);
	REQUIRE(point[1] == 2.0f);
	REQUIRE(point[2] == 0.0f);
	REQUIRE(point[3] == 0.0f);
}

TEST_CASE("DynamicPoint::AddDimension", "[DynamicPoint]")
{
	DynamicPointf point = DynamicPointf::CreateFromValues({ 1.0f, 2.0f });
	REQUIRE(point.GetDimensionCount() == 2);
	point.AddDimension(3.0f);
	REQUIRE(point.GetDimensionCount() == 3);
	REQUIRE(point[0] == 1.0f);
	REQUIRE(point[1] == 2.0f);
	REQUIRE(point[2] == 3.0f);
}

TEST_CASE("DynamicPoint::Equality Operator", "[DynamicPoint]")
{
	const DynamicPointf pointA = DynamicPointf::CreateFromValues(1.0f, 2.0f, 3.0f);
	const DynamicPointf pointB = DynamicPointf::CreateFromValues(1.0f, 2.0f, 3.0f);
	const DynamicPointf pointC = DynamicPointf::CreateFromValues(4.0f, 5.0f, 6.0f);
	REQUIRE(pointA == pointB);
	REQUIRE(!(pointA == pointC));
}

TEST_CASE("DynamicPoint::Inequality Operator", "[DynamicPoint]")
{
	const DynamicPointf pointA = DynamicPointf::CreateFromValues(1.0f, 2.0f, 3.0f);
	const DynamicPointf pointB = DynamicPointf::CreateFromValues(1.0f, 2.0f, 3.0f);
	const DynamicPointf pointC = DynamicPointf::CreateFromValues(4.0f, 5.0f, 6.0f);
	REQUIRE(!(pointA != pointB));
	REQUIRE(pointA != pointC);
}

TEST_CASE("DynamicPoint::Addition with DynamicVector", "[DynamicPoint]")
{
	const DynamicPointf point = DynamicPointf::CreateFromValues(1.0f, 2.0f, 3.0f);
	const DynamicVectorf vector = DynamicVectorf::CreateFromValues(4.0f, 5.0f, 6.0f);
	const DynamicPointf result = point + vector;
	REQUIRE(result.GetDimensionCount() == 3);
	REQUIRE(result[0] == 5.0f);
	REQUIRE(result[1] == 7.0f);
	REQUIRE(result[2] == 9.0f);
}

TEST_CASE("DynamicPoint::Addition with DynamicUnitVector", "[DynamicPoint]")
{
	const DynamicPointf point = DynamicPointf::CreateFromValues(1.0f, 2.0f, 3.0f);
	const DynamicUnitVectorf vector = DynamicUnitVectorf::CreateFromValues(1.0f, 0.f, 0.f);

	const DynamicPointf result = point + vector;
	REQUIRE(result.GetDimensionCount() == 3);
	REQUIRE(result[0] == 2.f);
	REQUIRE(result[1] == 2.f);
	REQUIRE(result[2] == 3.f);
}

TEST_CASE("DynamicPoint::Subtraction with DynamicVector", "[DynamicPoint]")
{
	const DynamicPointf point = DynamicPointf::CreateFromValues(1.0f, 2.0f, 3.0f);
	const DynamicVectorf vector = DynamicVectorf::CreateFromValues(4.0f, 5.0f, 6.0f);
	const DynamicPointf result = point - vector;
	REQUIRE(result.GetDimensionCount() == 3);
	REQUIRE(result[0] == -3.0f);
	REQUIRE(result[1] == -3.0f);
	REQUIRE(result[2] == -3.0f);
}

TEST_CASE("DynamicPoint::Subtraction with DynamicUnitVector", "[DynamicPoint]")
{
	const DynamicPointf point = DynamicPointf::CreateFromValues(1.0f, 2.0f, 3.0f);
	const DynamicUnitVectorf vector = DynamicUnitVectorf::CreateFromValues(1.0f, 0.f, 0.f);

	const DynamicPointf result = point - vector;
	REQUIRE(result.GetDimensionCount() == 3);
	REQUIRE(result[0] == 0.f);
	REQUIRE(result[1] == 2.f);
	REQUIRE(result[2] == 3.f);
}

TEST_CASE("DynamicPoint::Subtraction", "[DynamicPoint]")
{
	const DynamicPointf point1 = DynamicPointf::CreateFromValues(-6.0f, 2.0f, 3.0f);
	const DynamicPointf point2 = DynamicPointf::CreateFromValues(9.0f, -3.0f, 1.0f);
	const DynamicVectorf result = point1 - point2;
	REQUIRE(result.GetDimensionCount() == 3);
	REQUIRE(result[0] == -15.f);
	REQUIRE(result[1] == 5.f);
	REQUIRE(result[2] == 2.f);
}

TEST_CASE("DynamicPoint::Addition Assignment with DynamicVector", "[DynamicPoint]")
{
	DynamicPointf p = DynamicPointf::CreateFromValues(-6.0f, 2.0f, 3.0f);
	const DynamicVectorf v = DynamicVectorf::CreateFromValues(2.f, 1.f, -5.f);
	p += v;
	REQUIRE(p[0] == -4.f);
	REQUIRE(p[1] == 3.f);
	REQUIRE(p[2] == -2.f);
}

TEST_CASE("DynamicPoint::Addition Assignment with DynamicUnitVector", "[DynamicPoint]")
{
	DynamicPointf p = DynamicPointf::CreateFromValues(-6.0f, 2.0f, 3.0f);
	const DynamicUnitVectorf v = DynamicUnitVectorf::CreateFromValues(1.f, 0.f, 0.f);
	p += v;
	REQUIRE(p[0] == -5.f);
	REQUIRE(p[1] == 2.f);
	REQUIRE(p[2] == 3.f);
}

TEST_CASE("DynamicPoint::Subtraction Assignment with DynamicVector", "[DynamicPoint]")
{
	DynamicPointf p = DynamicPointf::CreateFromValues(-6.0f, 2.0f, 3.0f);
	const DynamicVectorf v = DynamicVectorf::CreateFromValues(2.f, 1.f, -5.f);
	p -= v;
	REQUIRE(p[0] == -8.f);
	REQUIRE(p[1] == 1.f);
	REQUIRE(p[2] == 8.f);
}

TEST_CASE("DynamicPoint::Subtraction Assignment with DynamicUnitVector", "[DynamicPoint]")
{
	DynamicPointf p = DynamicPointf::CreateFromValues(-6.0f, 2.0f, 3.0f);
	const DynamicUnitVectorf v = DynamicUnitVectorf::CreateFromValues(1.f, 0.f, 0.f);
	p -= v;
	REQUIRE(p[0] == -7.f);
	REQUIRE(p[1] == 2.f);
	REQUIRE(p[2] == 3.f);
}