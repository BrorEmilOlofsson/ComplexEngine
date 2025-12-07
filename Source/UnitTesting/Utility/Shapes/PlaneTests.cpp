#include <External/Catch2/catch_amalgamated.hpp>
#include "Utility/Shapes/Plane.hpp"

using namespace Simple;

TEST_CASE("Plane::Default Constructor", "[Plane]")
{
    constexpr Planef plane;
    REQUIRE(plane.GetPoint() == Point3f::Zero());
    REQUIRE(plane.GetNormal() == UnitVector3f::Up());
}

TEST_CASE("Plane::FromPointAndNormal", "[Plane]")
{
    constexpr Point3f point(1.0f, 2.0f, 3.0f);
    constexpr UnitVector3f normal = UnitVector3f::Forward();
    const Planef plane = Planef::FromPointAndNormal(point, normal);
    REQUIRE(plane.GetPoint() == point);
    REQUIRE(plane.GetNormal() == normal);
}

TEST_CASE("Plane::FromPoints", "[Plane]")
{
    constexpr Point3f p0(0.0f, 0.0f, 0.0f);
    constexpr Point3f p1(1.0f, 0.0f, 0.0f);
    constexpr Point3f p2(0.0f, 1.0f, 0.0f);
    const Planef plane = Planef::FromPoints(p0, p1, p2);
    REQUIRE(plane.GetPoint() == p0);
    REQUIRE(plane.GetNormal() == UnitVector3f::Forward());
}

TEST_CASE("Plane::SetPoint", "[Plane]")
{
    Planef plane;
    constexpr Point3f newPoint(4.0f, 5.0f, 6.0f);
    plane.SetPoint(newPoint);
    REQUIRE(plane.GetPoint() == newPoint);
}

TEST_CASE("Plane::SetNormal", "[Plane]")
{
    Planef plane;
    constexpr UnitVector3f newNormal = UnitVector3f::Right();
    plane.SetNormal(newNormal);
    REQUIRE(plane.GetNormal() == newNormal);
}