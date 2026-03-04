#include <External/Catch2/catch_amalgamated.hpp>
#include "Engine/Math/Shapes/Line3.hpp"

using namespace CLX;


TEST_CASE("Line3::Default Constructor", "[Line3]")
{
    Line3d line;
    REQUIRE(line.GetPoint() == Point3d::Zero());
    REQUIRE(line.GetDirection() == UnitVector3d::Forward());
}

TEST_CASE("Line3::FromPoints", "[Line3]")
{
    Line3d line = Line3d::FromPoints(Point3d(0, 0, 5), Point3d(2, 1, 6));
    REQUIRE(line.GetPoint() == Point3d(0, 0, 5));
    REQUIRE(line.GetDirection() == UnitVector3d(Vector3d(2, 1, 1)));
}

TEST_CASE("Line3::FromPointAndDirection (UnitVector3)", "[Line3]")
{
    Line3d line = Line3d::FromPointAndDirection(Point3d(1, 1, 1), UnitVector3d::Left());
    REQUIRE(line.GetPoint() == Point3d(1, 1, 1));
    REQUIRE(line.GetDirection() == UnitVector3d::Left());
}

TEST_CASE("Line3::FromPointAndDirection (Vector3)", "[Line3]")
{
    Line3d line = Line3d::FromPointAndDirection(Point3d(1, 1, 1), Vector3d(-3, 4, 0));
    REQUIRE(line.GetPoint() == Point3d(1, 1, 1));
    REQUIRE(line.GetDirection() == UnitVector3d(Vector3d(-3, 4, 0)));
}

TEST_CASE("Line3::SetPoint", "[Line3]")
{
    Line3d line = Line3d::FromPoints(Point3d(0, 0, 0), Point3d(1, 1, 1));
    line.SetPoint(Point3d(2, 2, 2));
    REQUIRE(line.GetPoint() == Point3d(2, 2, 2));
}

TEST_CASE("Line3::SetDirection", "[Line3]")
{
    Line3d line = Line3d::FromPoints(Point3d(0, 0, 0), Point3d(1, 1, 1));
    line.SetDirection(UnitVector3d::Down());
    REQUIRE(line.GetDirection() == UnitVector3d::Down());
}

TEST_CASE("Line3::Equality Operator", "[Line3]")
{
    Line3d line1 = Line3d::FromPoints(Point3d(0, 0, 0), Point3d(1, 1, 1));
    Line3d line2 = Line3d::FromPoints(Point3d(0, 0, 0), Point3d(1, 1, 1));
    Line3d line3 = Line3d::FromPoints(Point3d(0, 0, 0), Point3d(1, 2, 1));
    REQUIRE(line1 == line2);
    REQUIRE_FALSE(line1 == line3);
}