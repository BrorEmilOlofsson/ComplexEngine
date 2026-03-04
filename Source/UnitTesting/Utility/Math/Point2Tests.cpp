#include <External/Catch2/catch_amalgamated.hpp>

#include "Engine/Math/Point2.hpp"

using namespace Simple;

TEST_CASE("Point2 constructor and accessors", "[Point2]")
{
	const Point2f p(3.5f, -2.0f);
	REQUIRE(p.x == 3.5f);
	REQUIRE(p.y == -2.0f);
}

TEST_CASE("Point2 static constants", "[Point2]")
{
	constexpr Point2f zero = Point2f::Zero();
	constexpr Point2f one = Point2f::One();
	REQUIRE(zero.x == 0.0f);
	REQUIRE(zero.y == 0.0f);
	REQUIRE(one.x == 1.0f);
	REQUIRE(one.y == 1.0f);
}

TEST_CASE("Point2 template conversion", "[Point2]")
{
	const Point2i intPoint(4, 5);
	const Point2f floatPoint = Point2f(intPoint);
	REQUIRE(floatPoint.x == 4.0f);
	REQUIRE(floatPoint.y == 5.0f);
}

TEST_CASE("Point2 default constructor", "[Point2]")
{
	const Point2f defaultPoint;
	REQUIRE(defaultPoint.x == 0.0f);
	REQUIRE(defaultPoint.y == 0.0f);
}

TEST_CASE("Point2 subtraction", "[Point2]")
{
	const Point2f a(5.0f, 7.0f);
	const Point2f b(2.0f, 3.0f);
	const Vector2f result = a - b;
	REQUIRE(result.x == 3.0f);
	REQUIRE(result.y == 4.0f);
}

TEST_CASE("Point2 equality", "[Point2]")
{
	const Point2f a(1.0f, 2.0f);
	const Point2f b(1.0f, 2.0f);
	const Point2f c(2.0f, 3.0f);
	REQUIRE(a == b);
	REQUIRE(a != c);
}

TEST_CASE("Point2 addition with Vector2", "[Point2]")
{
	const Point2f point(1.0f, 2.0f);
	const Vector2f vector(3.0f, 4.0f);
	const Point2f result = point + vector;
	REQUIRE(result.x == 4.0f);
	REQUIRE(result.y == 6.0f);
}

TEST_CASE("Point2 addition with UnitVector2", "[Point2]")
{
	const Point2f point(1.0f, 2.0f);
	const UnitVector2f unitVector = UnitVector2f::Right();
	const Point2f result = point + unitVector;
	REQUIRE(result.x == 2.0f);
	REQUIRE(result.y == 2.0f);
}

TEST_CASE("Point2 subtraction with Vector2", "[Point2]")
{
	const Point2f point(5.0f, 7.0f);
	const Vector2f vector(2.0f, 3.0f);
	const Point2f result = point - vector;
	REQUIRE(result.x == 3.0f);
	REQUIRE(result.y == 4.0f);
}

TEST_CASE("Point2 subtraction with UnitVector2", "[Point2]")
{
	const Point2f point(5.0f, 7.0f);
	const UnitVector2f unitVector = UnitVector2f::Up();
	const Point2f result = point - unitVector;
	REQUIRE(result.x == 5.0f);
	REQUIRE(result.y == 6.0f);
}

TEST_CASE("Point2 negation", "[Point2]")
{
	const Point2f point(3.0f, -4.0f);
	const Point2f result = -point;
	REQUIRE(result.x == -3.0f);
	REQUIRE(result.y == 4.0f);
}

TEST_CASE("Point2 addition assignment with Vector2", "[Point2]")
{
	Point2f point(1.0f, 2.0f);
	const Vector2f vector(3.0f, 4.0f);
	point += vector;
	REQUIRE(point.x == 4.0f);
	REQUIRE(point.y == 6.0f);
}

TEST_CASE("Point2 addition assignment with UnitVector2", "[Point2]")
{
	Point2f point(1.0f, 2.0f);
	const UnitVector2f unitVector = UnitVector2f::Right();
	point += unitVector;
	REQUIRE(point.x == 2.0f);
	REQUIRE(point.y == 2.0f);
}

TEST_CASE("Point2 subtraction assignment with Vector2", "[Point2]")
{
	Point2f point(5.0f, 7.0f);
	const Vector2f vector(2.0f, 3.0f);
	point -= vector;
	REQUIRE(point.x == 3.0f);
	REQUIRE(point.y == 4.0f);
}

TEST_CASE("Point2 subtraction assignment with UnitVector2", "[Point2]")
{
	Point2f point(5.0f, 7.0f);
	const UnitVector2f unitVector = UnitVector2f::Up();
	point -= unitVector;
	REQUIRE(point.x == 5.0f);
	REQUIRE(point.y == 6.0f);
}

