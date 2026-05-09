#include <External/Catch2/catch_amalgamated.hpp>
#include "Engine/Math/CurveMath.hpp"

using namespace CLX;

TEST_CASE("CurveMath::QuadraticCurve", "[CurveMath]")
{
    Point2d a{0.0f, 0.0f};
    Point2d b{1.0f, 2.0f};
    Point2d c{2.0f, 0.0f};
    REQUIRE(QuadraticCurve(a, b, c, 0.0) == a);
    REQUIRE(QuadraticCurve(a, b, c, 1.0) == c);
    REQUIRE(QuadraticCurve(a, b, c, 0.5) == Point2d{1.0f, 1.0f});
}

TEST_CASE("CurveMath::CubicCurve", "[CurveMath]")
{

}

TEST_CASE("CurveMath::GetBezierCubicY", "[CurveMath]")
{
    const std::array<Point2d, 4> points
    {
        Point2d(0, 0),
        Point2d(0.1, 0.8),
        Point2d(0.8, 0.5),
        Point2d(1, 1)
    };

    //std::span<const Point2d> span = points;

    double yValue = GetBezierCubicY(points, 1.0);
    REQUIRE(yValue == 1.0);
}