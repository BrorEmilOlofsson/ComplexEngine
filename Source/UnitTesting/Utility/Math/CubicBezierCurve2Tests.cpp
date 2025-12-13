#include <External/Catch2/catch_amalgamated.hpp>
#include "Utility/Math/CubicBezierCurve2.hpp"

using namespace Simple;

TEST_CASE("CubicBezierCurve2::AddAnchorPoint", "[CubicBezierCurve2]")
{
    SECTION("Adding first anchor point")
    {
        CubicBezierCurve2d curve;
        Point2d p1{ 5.0f, 5.0f };
        curve.AddAnchorPoint(p1);
        REQUIRE(curve.GetPoints().size() == 1);
        REQUIRE(curve.GetPoints().front() == Point2d(5.0, 5.0));
    }
    SECTION("Adding second anchor point")
    {
        CubicBezierCurve2d curve;
        Point2d p1{ 0.0f, 0.0f };
        Point2d p2{ 4.0f, 4.0f };
        curve.AddAnchorPoint(p1);
        curve.AddAnchorPoint(p2);
        REQUIRE(curve.GetPoints().size() == 4);
        REQUIRE(curve.GetPoints()[0] == Point2d(0.0, 0.0));
        REQUIRE(curve.GetPoints()[1] == Point2d(1.0, 1.0));
        REQUIRE(curve.GetPoints()[2] == Point2d(3.0, 3.0));
        REQUIRE(curve.GetPoints()[3] == Point2d(4.0, 4.0));
    }
    SECTION("Adding third anchor point")
    {
        CubicBezierCurve2d curve;
        Point2d p1{ 0.0f, 0.0f };
        Point2d p2{ 4.0f, 4.0f };
        Point2d p3{ 8.0f, 0.0f };
        curve.AddAnchorPoint(p1);
        curve.AddAnchorPoint(p2);
        curve.AddAnchorPoint(p3);
        REQUIRE(curve.GetPoints().size() == 7);
        REQUIRE(curve.GetPoints()[0] == Point2d(0.0, 0.0));
        REQUIRE(curve.GetPoints()[1] == Point2d(1.0, 1.0));
        REQUIRE(curve.GetPoints()[2] == Point2d(3.0, 3.0));
        REQUIRE(curve.GetPoints()[3] == Point2d(4.0, 4.0));
        REQUIRE(curve.GetPoints()[4] == Point2d(5.0, 5.0));
        REQUIRE(curve.GetPoints()[5] == Point2d(6.5, 2.5));
        REQUIRE(curve.GetPoints()[6] == Point2d(8.0, 0.0));
    }
}