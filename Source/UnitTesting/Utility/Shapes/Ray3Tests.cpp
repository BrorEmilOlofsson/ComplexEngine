#include <External/Catch2/catch_amalgamated.hpp>
#include "Engine/Math/Shapes/Ray3.hpp"

using namespace CLX;

TEST_CASE("Ray3::Default Constructor", "[Ray3]")
{
    constexpr Ray3d ray;
    REQUIRE(ray.GetOrigin() == Point3d::Zero());
    REQUIRE(ray.GetDirection() == UnitVector3d::Forward());
}

TEST_CASE("Ray3::FromOriginAndDirection", "[Ray3]")
{
    constexpr Point3d origin(1.0, 2.0, 3.0);
    constexpr UnitVector3d direction = UnitVector3d::Forward();
    constexpr Ray3d ray = Ray3d::FromOriginAndDirection(origin, direction);
    REQUIRE(ray.GetOrigin() == origin);
    REQUIRE(ray.GetDirection() == direction);
}

TEST_CASE("Ray3::FromOriginAndPoint", "[Ray3]")
{
    constexpr Point3d origin(1.0, 2.0, 3.0);
    constexpr Point3d point(4.0, 6.0, 8.0);
    const Ray3d ray = Ray3d::FromOriginAndPoint(origin, point);
    REQUIRE(ray.GetOrigin() == origin);
    REQUIRE(ray.GetDirection() == UnitVector3d(3.0, 4.0, 5.0));
}

TEST_CASE("Ray3::SetOrigin", "[Ray3]")
{
    Ray3d ray = Ray3d::FromOriginAndDirection(Point3d(2.0, 7.0, 1.0), UnitVector3d::Left());
    const Point3d newOrigin(3.0, 4.0, 5.0);
    ray.SetOrigin(newOrigin);
    REQUIRE(ray.GetOrigin() == newOrigin);
    REQUIRE(ray.GetDirection() == UnitVector3d::Left());
}

TEST_CASE("Ray3::SetDirection", "[Ray3]")
{
    Ray3d ray = Ray3d::FromOriginAndDirection(Point3d(2.0, 7.0, 1.0), UnitVector3d::Up());
    const UnitVector3d newDirection = UnitVector3d::Down();
    ray.SetDirection(newDirection);
    REQUIRE(ray.GetOrigin() == Point3d(2.0, 7.0, 1.0));
    REQUIRE(ray.GetDirection() == newDirection);
}

TEST_CASE("Ray3::GetPointAtDistance", "[Ray3]")
{
    Ray3d ray = Ray3d::FromOriginAndDirection(Point3d(1.0, 1.0, 1.0), UnitVector3d::Right());
    const Point3d pointAtDistance5 = ray.GetPointAtDistance(5.0);
    REQUIRE(pointAtDistance5 == Point3d(6.0, 1.0, 1.0));
    ray.SetDirection(UnitVector3d::Up());
    const Point3d pointAtDistance3 = ray.GetPointAtDistance(3.0);
    REQUIRE(pointAtDistance3 == Point3d(1.0, 4.0, 1.0));
    ray.SetDirection(UnitVector3d::Forward());
    const Point3d pointAtDistance2 = ray.GetPointAtDistance(2.0);
    REQUIRE(pointAtDistance2 == Point3d(1.0, 1.0, 3.0));
}

TEST_CASE("Ray3::Equality Operator", "[Ray3]")
{
    const Ray3d ray1 = Ray3d::FromOriginAndDirection(Point3d(0.0, 0.0, 0.0), UnitVector3d::Forward());
    const Ray3d ray2 = Ray3d::FromOriginAndDirection(Point3d(0.0, 0.0, 0.0), UnitVector3d::Forward());
    const Ray3d ray3 = Ray3d::FromOriginAndDirection(Point3d(1.0, 0.0, 0.0), UnitVector3d::Forward());
    REQUIRE(ray1 == ray2);
    REQUIRE_FALSE(ray1 == ray3);
}