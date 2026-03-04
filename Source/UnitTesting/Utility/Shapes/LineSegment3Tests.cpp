#include <External/Catch2/catch_amalgamated.hpp>
#include "Engine/Math/Shapes/LineSegment3.hpp"

using namespace Simple;

TEST_CASE("LineSegment3::Default Constructor", "[LineSegment3]")
{
    constexpr LineSegment3d lineSegment;
    REQUIRE(lineSegment.StartPoint() == Point3d::Zero());
    REQUIRE(lineSegment.EndPoint() == Point3d::Zero());
}

TEST_CASE("LineSegment3::FromPoints", "[LineSegment3]")
{
    const Point3d startPoint(1.0, 2.0, 3.0);
    const Point3d endPoint(4.0, 5.0, 6.0);
    const LineSegment3d lineSegment = LineSegment3d::FromPoints(startPoint, endPoint);
    REQUIRE(lineSegment.StartPoint() == startPoint);
    REQUIRE(lineSegment.EndPoint() == endPoint);
}

TEST_CASE("LineSegment3::StartPoint", "[LineSegment3]")
{
    LineSegment3d lineSegment = LineSegment3d::FromPoints(Point3d(0.0, 0.0, 0.0), Point3d(5.0, 5.0, 5.0));
    Point3d& startPoint = lineSegment.StartPoint();
    startPoint = Point3d(1.0, 1.0, 1.0);
    REQUIRE(lineSegment.StartPoint() == Point3d(1.0, 1.0, 1.0));
    REQUIRE(lineSegment.EndPoint() == Point3d(5.0, 5.0, 5.0));
}

TEST_CASE("LineSegment3::EndPoint", "[LineSegment3]")
{
    LineSegment3d lineSegment = LineSegment3d::FromPoints(Point3d::Zero(), Point3d(5.0, 5.0, 5.0));
    lineSegment.EndPoint() = Point3d(4.0, 4.0, 4.0);
    REQUIRE(lineSegment.StartPoint() == Point3d::Zero());
    REQUIRE(lineSegment.EndPoint() == Point3d(4.0, 4.0, 4.0));
}

TEST_CASE("LineSegment3::Equality Operator", "[LineSegment3]")
{
    const LineSegment3d lineSegment1 = LineSegment3d::FromPoints(Point3d(1.0, 2.0, 3.0), Point3d(4.0, 5.0, 6.0));
    const LineSegment3d lineSegment2 = LineSegment3d::FromPoints(Point3d(1.0, 2.0, 3.0), Point3d(4.0, 5.0, 6.0));
    const LineSegment3d lineSegment3 = LineSegment3d::FromPoints(Point3d(0.0, 0.0, 0.0), Point3d(4.0, 5.0, 6.0));
    REQUIRE(lineSegment1 == lineSegment2);
    REQUIRE_FALSE(lineSegment1 == lineSegment3);
}