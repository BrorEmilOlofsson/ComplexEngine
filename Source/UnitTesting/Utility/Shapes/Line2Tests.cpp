#include <External/Catch2/catch_amalgamated.hpp>
#include "Utility/Shapes/Line2.hpp"

using namespace Simple;

TEST_CASE("Line2::Default Constructor", "[Line2]")
{
    constexpr Line2d line;
    REQUIRE(line.GetPoint() == Point2d::Zero());
    REQUIRE(line.GetDirection() == UnitVector2d::Right());
}

TEST_CASE("Line2::FromPoints", "[Line2]")
{
    const Line2d line = Line2d::FromPoints(Point2d(0, 5), Point2d(2, 6));

    REQUIRE(line.GetPoint() == Point2d(0, 5));
    REQUIRE(line.GetDirection() == UnitVector2d(Vector2d(2, 1)));
}

TEST_CASE("Line2::FromPointAndDirection (UnitVector2)", "[Line2]")
{
    const Line2d line = Line2d::FromPointAndDirection(Point2d(1, 1), UnitVector2d::Left());
    REQUIRE(line.GetPoint() == Point2d(1, 1));
    REQUIRE(line.GetDirection() == UnitVector2d::Left());
}

TEST_CASE("Line2::FromPointAndDirection (Vector2)", "[Line2]")
{
    const Line2d line = Line2d::FromPointAndDirection(Point2d(1, 1), Vector2d(-3, 4));
    REQUIRE(line.GetPoint() == Point2d(1, 1));
    REQUIRE(line.GetDirection() == UnitVector2d(Vector2d(-3, 4)));
}

TEST_CASE("Line2::SetPoint", "[Line2]")
{
    Line2d line = Line2d::FromPoints(Point2d(0, 0), Point2d(1, 1));
    line.SetPoint(Point2d(2, 2));
    REQUIRE(line.GetPoint() == Point2d(2, 2));
}

TEST_CASE("Line2::SetDirection", "[Line2]")
{
    Line2d line = Line2d::FromPoints(Point2d(0, 0), Point2d(1, 1));
    line.SetDirection(UnitVector2d::Down());
    REQUIRE(line.GetDirection() == UnitVector2d::Down());
}

TEST_CASE("Line2::Equality Operator", "[Line2]")
{
    Line2d line1 = Line2d::FromPoints(Point2d(0, 0), Point2d(1, 1));
    Line2d line2 = Line2d::FromPoints(Point2d(0, 0), Point2d(1, 1));
    Line2d line3 = Line2d::FromPoints(Point2d(0, 0), Point2d(2, 1));
    REQUIRE(line1 == line2);
    REQUIRE_FALSE(line1 == line3);
}