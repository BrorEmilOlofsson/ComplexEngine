#include <External/Catch2/catch_amalgamated.hpp>
#include "Utility/Shapes/Ray2.hpp"

using namespace Simple;

TEST_CASE("Ray2::Default Constructor", "[Ray2]")
{
    Ray2d ray;
    REQUIRE(ray.GetOrigin() == Point2d::Zero());
    REQUIRE(ray.GetDirection() == UnitVector2d::Right());
}

TEST_CASE("Ray2::FromOriginAndDirection", "[Ray2]")
{
    Point2d origin(1.0, 2.0);
    UnitVector2d direction(0.0, 1.0);
    Ray2d ray = Ray2d::FromOriginAndDirection(origin, direction);
    REQUIRE(ray.GetOrigin() == origin);
    REQUIRE(ray.GetDirection() == direction);
}

TEST_CASE("Ray2::FromOriginAndPoint", "[Ray2]")
{
    constexpr Point2d origin(1.0, 2.0);
    constexpr Point2d point(4.0, 6.0);
    const Ray2d ray = Ray2d::FromOriginAndPoint(origin, point);
    REQUIRE(ray.GetOrigin() == origin);
    REQUIRE(ray.GetDirection() == UnitVector2d(3.0, 4.0));
}

TEST_CASE("Ray2::SetOrigin", "[Ray2]")
{
    Ray2d ray = Ray2d::FromOriginAndDirection(Point2d(2.0, 7.0), UnitVector2d::Left());
    const Point2d newOrigin(3.0, 4.0);
    ray.SetOrigin(newOrigin);
    REQUIRE(ray.GetOrigin() == newOrigin);
    REQUIRE(ray.GetDirection() == UnitVector2d::Left());
}

TEST_CASE("Ray2::SetDirection", "[Ray2]")
{
    Ray2d ray = Ray2d::FromOriginAndDirection(Point2d(2.0, 7.0), UnitVector2d::Up());
    const UnitVector2d newDirection = UnitVector2d::Down();
    ray.SetDirection(newDirection);
    REQUIRE(ray.GetOrigin() == Point2d(2.0, 7.0));
    REQUIRE(ray.GetDirection() == newDirection);
}

TEST_CASE("Ray2::GetPointAtDistance", "[Ray2]")
{
    Ray2d ray = Ray2d::FromOriginAndDirection(Point2d(1.0, 1.0), UnitVector2d::Right());
    const Point2d pointAtDistance5 = ray.GetPointAtDistance(5.0);
    REQUIRE(pointAtDistance5 == Point2d(6.0, 1.0));
    ray.SetDirection(UnitVector2d::Up());
    const Point2d pointAtDistance3 = ray.GetPointAtDistance(3.0);
    REQUIRE(pointAtDistance3 == Point2d(1.0, 4.0));
}

TEST_CASE("Ray2::Equality Operator", "[Ray2]")
{
    Ray2d ray1 = Ray2d::FromOriginAndDirection(Point2d(0.0, 0.0), UnitVector2d::Right());
    Ray2d ray2 = Ray2d::FromOriginAndDirection(Point2d(0.0, 0.0), UnitVector2d::Right());
    Ray2d ray3 = Ray2d::FromOriginAndDirection(Point2d(1.0, 1.0), UnitVector2d::Up());
    REQUIRE(ray1 == ray2);
    REQUIRE_FALSE(ray1 == ray3);
}