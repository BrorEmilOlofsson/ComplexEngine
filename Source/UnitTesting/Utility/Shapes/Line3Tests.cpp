#include <External/Catch2/catch_amalgamated.hpp>
#include "Utility/Shapes/Line3.hpp"

using namespace Simple;


TEST_CASE("Line3::Default Constructor", "[Line3]")
{
    Line3f line;
    REQUIRE(line.GetPoint() == Point3f::Zero());
    REQUIRE(line.GetDirection() == UnitVector3f::Forward());
}

TEST_CASE("Line3::FromPoints", "[Line3]")
{
    Line3f line = Line3f::FromPoints(Point3f(0, 0, 5), Point3f(2, 1, 6));
    REQUIRE(line.GetPoint() == Point3f(0, 0, 5));
    REQUIRE(line.GetDirection() == UnitVector3f(Vector3f(2, 1, 1)));
}

TEST_CASE("Line3::FromPointAndDirection (UnitVector3)", "[Line3]")
{
    Line3f line = Line3f::FromPointAndDirection(Point3f(1, 1, 1), UnitVector3f::Left());
    REQUIRE(line.GetPoint() == Point3f(1, 1, 1));
    REQUIRE(line.GetDirection() == UnitVector3f::Left());
}

TEST_CASE("Line3::FromPointAndDirection (Vector3)", "[Line3]")
{
    Line3f line = Line3f::FromPointAndDirection(Point3f(1, 1, 1), Vector3f(-3, 4, 0));
    REQUIRE(line.GetPoint() == Point3f(1, 1, 1));
    REQUIRE(line.GetDirection() == UnitVector3f(Vector3f(-3, 4, 0)));
}

TEST_CASE("Line3::SetPoint", "[Line3]")
{
    Line3f line = Line3f::FromPoints(Point3f(0, 0, 0), Point3f(1, 1, 1));
    line.SetPoint(Point3f(2, 2, 2));
    REQUIRE(line.GetPoint() == Point3f(2, 2, 2));
}

TEST_CASE("Line3::SetDirection", "[Line3]")
{
    Line3f line = Line3f::FromPoints(Point3f(0, 0, 0), Point3f(1, 1, 1));
    line.SetDirection(UnitVector3f::Down());
    REQUIRE(line.GetDirection() == UnitVector3f::Down());
}