#include <External/Catch2/catch_amalgamated.hpp>
#include "Engine/Math/Shapes/Plane.hpp"

using namespace CLX;

TEST_CASE("Plane::Default Constructor", "[Plane]")
{
    constexpr Planed plane;
    REQUIRE(plane.GetPoint() == Point3d::Zero());
    REQUIRE(plane.GetNormal() == UnitVector3d::Up());
}

TEST_CASE("Plane::FromPointAndNormal", "[Plane]")
{
    constexpr Point3d point(1.0f, 2.0f, 3.0f);
    constexpr UnitVector3d normal = UnitVector3d::Forward();
    const Planed plane = Planed::FromPointAndNormal(point, normal);
    REQUIRE(plane.GetPoint() == point);
    REQUIRE(plane.GetNormal() == normal);
}

TEST_CASE("Plane::FromPoints", "[Plane]")
{
    constexpr Point3d p0(0.0f, 0.0f, 0.0f);
    constexpr Point3d p1(1.0f, 0.0f, 0.0f);
    constexpr Point3d p2(0.0f, 1.0f, 0.0f);
    const Planed plane = Planed::FromPoints(p0, p1, p2);
    REQUIRE(plane.GetPoint() == p0);
    REQUIRE(plane.GetNormal() == UnitVector3d::Forward());
}

TEST_CASE("Plane::SetPoint", "[Plane]")
{
    Planed plane;
    constexpr Point3d newPoint(4.0f, 5.0f, 6.0f);
    plane.SetPoint(newPoint);
    REQUIRE(plane.GetPoint() == newPoint);
}

TEST_CASE("Plane::SetNormal", "[Plane]")
{
    Planed plane;
    constexpr UnitVector3d newNormal = UnitVector3d::Right();
    plane.SetNormal(newNormal);
    REQUIRE(plane.GetNormal() == newNormal);
}

TEST_CASE("Plane::Equality Operator", "[Plane]")
{
    const Planed plane1 = Planed::FromPointAndNormal(Point3d(1.0f, 2.0f, 3.0f), UnitVector3d::Up());
    const Planed plane2 = Planed::FromPointAndNormal(Point3d(1.0f, 2.0f, 3.0f), UnitVector3d::Up());
    const Planed plane3 = Planed::FromPointAndNormal(Point3d(1.0f, 2.0f, 3.0f), UnitVector3d::Right());
    REQUIRE(plane1 == plane2);
    REQUIRE_FALSE(plane1 == plane3);
}