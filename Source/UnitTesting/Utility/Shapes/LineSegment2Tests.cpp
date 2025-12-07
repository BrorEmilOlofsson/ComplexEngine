#include <External/Catch2/catch_amalgamated.hpp>
#include "Utility/Shapes/LineSegment2.hpp"

using namespace Simple;

TEST_CASE("LineSegment2::Default Constructor", "[LineSegment2]")
{
    constexpr LineSegment2d lineSegment;
    REQUIRE(lineSegment.StartPoint() == Point2d::Zero());
    REQUIRE(lineSegment.EndPoint() == Point2d::Zero());
}

TEST_CASE("LineSegment2::FromPoints", "[LineSegment2]")
{
    const Point2d startPoint(1.0, 2.0);
    const Point2d endPoint(3.0, 4.0);
    const LineSegment2d lineSegment = LineSegment2d::FromPoints(startPoint, endPoint);
    REQUIRE(lineSegment.StartPoint() == startPoint);
    REQUIRE(lineSegment.EndPoint() == endPoint);
}

TEST_CASE("LineSegment2::StartPoint", "[LineSegment2]")
{
    LineSegment2d lineSegment = LineSegment2d::FromPoints(Point2d(0.0, 0.0), Point2d(5.0, 5.0));
    Point2d& startPoint = lineSegment.StartPoint();
    startPoint = Point2d(1.0, 1.0);
    REQUIRE(lineSegment.StartPoint() == Point2d(1.0, 1.0));
    REQUIRE(lineSegment.EndPoint() == Point2d(5.0, 5.0));
}

TEST_CASE("LineSegment2::EndPoint", "[LineSegment2]")
{
    LineSegment2d lineSegment = LineSegment2d::FromPoints(Point2d::Zero(), Point2d(5.0, 5.0));
    lineSegment.EndPoint() = Point2d(4.0, 4.0);
    REQUIRE(lineSegment.StartPoint() == Point2d::Zero());
    REQUIRE(lineSegment.EndPoint() == Point2d(4.0, 4.0));
}