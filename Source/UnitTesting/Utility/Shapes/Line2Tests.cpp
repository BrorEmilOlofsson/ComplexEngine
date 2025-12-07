#include <External/Catch2/catch_amalgamated.hpp>
#include "Utility/Shapes/Line2.hpp"

using namespace Simple;

TEST_CASE("Line2::Default Constructor", "[Line2]")
{
    constexpr Line2f line;
    REQUIRE(line.GetPoint() == Point2f::Zero());
    REQUIRE(line.GetDirection() == UnitVector2f::Right());
}

TEST_CASE("Line2::FromPoints", "[Line2]")
{
    const Line2f line = Line2f::FromPoints(Point2f(0, 5), Point2f(2, 6));

    REQUIRE(line.GetPoint() == Point2f(0, 5));
    REQUIRE(line.GetDirection() == UnitVector2f(Vector2f(2, 1)));
}

TEST_CASE("Line2::FromPointAndDirection (UnitVector2)", "[Line2]")
{
    const Line2f line = Line2f::FromPointAndDirection(Point2f(1, 1), UnitVector2f::Left());
    REQUIRE(line.GetPoint() == Point2f(1, 1));
    REQUIRE(line.GetDirection() == UnitVector2f::Left());
}

TEST_CASE("Line2::FromPointAndDirection (Vector2)", "[Line2]")
{
    const Line2f line = Line2f::FromPointAndDirection(Point2f(1, 1), Vector2f(-3, 4));
    REQUIRE(line.GetPoint() == Point2f(1, 1));
    REQUIRE(line.GetDirection() == UnitVector2f(Vector2f(-3, 4)));
}

TEST_CASE("Line2::SetPoint", "[Line2]")
{
    Line2f line = Line2f::FromPoints(Point2f(0, 0), Point2f(1, 1));
    line.SetPoint(Point2f(2, 2));
    REQUIRE(line.GetPoint() == Point2f(2, 2));
}

TEST_CASE("Line2::SetDirection", "[Line2]")
{
    Line2f line = Line2f::FromPoints(Point2f(0, 0), Point2f(1, 1));
    line.SetDirection(UnitVector2f::Down());
    REQUIRE(line.GetDirection() == UnitVector2f::Down());
}