#include <External/Catch2/catch_amalgamated.hpp>
#include "Utility/Math/Point3.hpp"

using namespace Simple;

TEST_CASE("Point3 constructor and accessors", "[Point3]")
{
	const Point3f p(1.0f, -2.0f, 3.5f);
	REQUIRE(p.x == 1.0f);
	REQUIRE(p.y == -2.0f);
	REQUIRE(p.z == 3.5f);
}

TEST_CASE("Point3 default constructor", "[Point3]")
{
	const Point3f defaultPoint;
	REQUIRE(defaultPoint.x == 0.0f);
	REQUIRE(defaultPoint.y == 0.0f);
	REQUIRE(defaultPoint.z == 0.0f);
}

TEST_CASE("Point3 static constant Zero", "[Point3]")
{
	constexpr Point3f zero = Point3f::Zero();
	REQUIRE(zero.x == 0.0f);
	REQUIRE(zero.y == 0.0f);
	REQUIRE(zero.z == 0.0f);
}

TEST_CASE("Point3 static constant One", "[Point3]")
{
	constexpr Point3f one = Point3f::One();
	REQUIRE(one.x == 1.0f);
	REQUIRE(one.y == 1.0f);
	REQUIRE(one.z == 1.0f);
}

TEST_CASE("Point3 template conversion", "[Point3]")
{
	const Point3i intPoint(4, 5, 6);
	const Point3f floatPoint = Point3f(intPoint);
	REQUIRE(floatPoint.x == 4.0f);
	REQUIRE(floatPoint.y == 5.0f);
	REQUIRE(floatPoint.z == 6.0f);
}

TEST_CASE("Point3 equality", "[Point3]")
{
	const Point3f a(1.0f, 2.0f, 3.0f);
	const Point3f b(1.0f, 2.0f, 3.0f);
	const Point3f c(2.0f, 3.0f, 4.0f);
	REQUIRE(a == b);
	REQUIRE(a != c);
}

TEST_CASE("Point3 addition with Vector3", "[Point3]")
{
	const Point3f point(1.0f, 2.0f, 3.0f);
	const Vector3f vector(4.0f, 5.0f, 6.0f);
	const Point3f result = point + vector;
	REQUIRE(result.x == 5.0f);
	REQUIRE(result.y == 7.0f);
	REQUIRE(result.z == 9.0f);
}

TEST_CASE("Point3 subtraction with Vector3", "[Point3]")
{
	const Point3f point(5.0f, 7.0f, 9.0f);
	const Vector3f vector(2.0f, 3.0f, 4.0f);
	const Point3f result = point - vector;
	REQUIRE(result.x == 3.0f);
	REQUIRE(result.y == 4.0f);
	REQUIRE(result.z == 5.0f);
}

TEST_CASE("Point3 addition with UnitVector3", "[Point3]")
{
	const Point3f point(1.0f, 2.0f, 3.0f);
	const UnitVector3f unitVector = UnitVector3f::Right();
	const Point3f result = point + unitVector;
	REQUIRE(result.x == 2.0f);
	REQUIRE(result.y == 2.0f);
	REQUIRE(result.z == 3.0f);
}

TEST_CASE("Point3 subtraction with UnitVector3", "[Point3]")
{
	const Point3f point(5.0f, 7.0f, 9.0f);
	const UnitVector3f unitVector = UnitVector3f::Up();
	const Point3f result = point - unitVector;
	REQUIRE(result.x == 5.0f);
	REQUIRE(result.y == 6.0f);
	REQUIRE(result.z == 9.0f);
}

TEST_CASE("Point3 negation", "[Point3]")
{
	const Point3f point(3.0f, -4.0f, 5.0f);
	const Point3f result = -point;
	REQUIRE(result.x == -3.0f);
	REQUIRE(result.y == 4.0f);
	REQUIRE(result.z == -5.0f);
}

TEST_CASE("Point3 subtraction resulting in Vector3", "[Point3]")
{
	const Point3f a(5.0f, 7.0f, 9.0f);
	const Point3f b(2.0f, 3.0f, 4.0f);
	const Vector3f result = a - b;
	REQUIRE(result.x == 3.0f);
	REQUIRE(result.y == 4.0f);
	REQUIRE(result.z == 5.0f);
}

TEST_CASE("Point3 addition assignment with Vector3", "[Point3]")
{
	Point3f point(1.0f, 2.0f, 3.0f);
	const Vector3f vector(4.0f, 5.0f, 6.0f);
	point += vector;
	REQUIRE(point.x == 5.0f);
	REQUIRE(point.y == 7.0f);
	REQUIRE(point.z == 9.0f);
}

TEST_CASE("Point3 addition assignment with UnitVector3", "[Point3]")
{
	Point3f point(1.0f, 2.0f, 3.0f);
	const UnitVector3f unitVector = UnitVector3f::Forward();
	point += unitVector;
	REQUIRE(point.x == 1.0f);
	REQUIRE(point.y == 2.0f);
	REQUIRE(point.z == 4.0f);
}

TEST_CASE("Point3 subtraction assignment with Vector3", "[Point3]")
{
	Point3f point(5.0f, 7.0f, 9.0f);
	const Vector3f vector(2.0f, 3.0f, 4.0f);
	point -= vector;
	REQUIRE(point.x == 3.0f);
	REQUIRE(point.y == 4.0f);
	REQUIRE(point.z == 5.0f);
}

TEST_CASE("Point3 subtraction assignment with UnitVector3", "[Point3]")
{
	Point3f point(5.0f, 7.0f, 9.0f);
	const UnitVector3f unitVector = UnitVector3f::Up();
	point -= unitVector;
	REQUIRE(point.x == 5.0f);
	REQUIRE(point.y == 6.0f);
	REQUIRE(point.z == 9.0f);
}