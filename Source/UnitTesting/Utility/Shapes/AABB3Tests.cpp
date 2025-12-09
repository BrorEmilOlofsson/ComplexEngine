#include <External/Catch2/catch_amalgamated.hpp>
#include "Utility/Shapes/AABB3.hpp"

using namespace Simple;

TEST_CASE("AABB3::Default Constructor", "[AABB3]")
{
    AABB3d aabb;
    REQUIRE(aabb.GetMin() == Point3d::Zero());
    REQUIRE(aabb.GetMax() == Point3d::Zero());
}

TEST_CASE("AABB3::FromMinAndMax", "[AABB3]")
{
    const AABB3d aabb = AABB3d::FromMinAndMax(Point3d(1.0f, 2.0f, 3.f), Point3d(3.0f, 4.0f, 5.f));
    REQUIRE(aabb.GetMin() == Point3d(1.0f, 2.0f, 3.f));
    REQUIRE(aabb.GetMax() == Point3d(3.0f, 4.0f, 5.f));
}

TEST_CASE("AABB3::FromMinAndExtent", "[AABB3]")
{
    const AABB3d aabb = AABB3d::FromMinAndExtent(Point3d(1.0f, 2.0f, 3.f), Vector3d(2.0f, 2.0f, 2.f));
    REQUIRE(aabb.GetMin() == Point3d(1.0f, 2.0f, 3.f));
    REQUIRE(aabb.GetMax() == Point3d(3.0f, 4.0f, 5.f));
}

TEST_CASE("AABB3::FromCenterAndExtent", "[AABB3]")
{
    const AABB3d aabb = AABB3d::FromCenterAndExtent(Point3d(2.0f, 3.0f, 4.f), Vector3d(2.0f, 2.0f, 2.f));
    REQUIRE(aabb.GetMin() == Point3d(1.0f, 2.0f, 3.f));
    REQUIRE(aabb.GetMax() == Point3d(3.0f, 4.0f, 5.f));
}

TEST_CASE("AABB3::FromDefaultAndExtent", "[AABB3]")
{
    const AABB3d aabb = AABB3d::FromDefaultAndExtent(Vector3d(2.0f, 2.0f, 2.f));
    REQUIRE(aabb.GetMin() == Point3d::Zero());
    REQUIRE(aabb.GetMax() == Point3d(2.0f, 2.0f, 2.f));
}

TEST_CASE("AABB3::SetMinAndExtent", "[AABB3]")
{
    AABB3d aabb;
    aabb.SetMinAndExtent(Point3d(1.0f, 2.0f, 3.f), Vector3d(2.0f, 2.0f, 2.f));
    REQUIRE(aabb.GetMin() == Point3d(1.0f, 2.0f, 3.f));
    REQUIRE(aabb.GetMax() == Point3d(3.0f, 4.0f, 5.f));
}

TEST_CASE("AABB3::SetMinAndMax", "[AABB3]")
{
    AABB3d aabb;
    aabb.SetMinAndMax(Point3d(1.0f, 2.0f, 3.f), Point3d(3.0f, 4.0f, 5.f));
    REQUIRE(aabb.GetMin() == Point3d(1.0f, 2.0f, 3.f));
    REQUIRE(aabb.GetMax() == Point3d(3.0f, 4.0f, 5.f));
}

TEST_CASE("AABB3::SetExtent", "[AABB2]")
{
    AABB3d aabb = AABB3d::FromMinAndMax(Point3d(1.0f, 2.0f, -1.f), Point3d(3.0f, 4.0f, 2.f));
    aabb.SetExtent(Vector3d(4.0f, 4.0f, 4.f));
    REQUIRE(aabb.GetMin() == Point3d(0.0f, 1.0f, -1.5f));
    REQUIRE(aabb.GetMax() == Point3d(4.0f, 5.0f, 2.5f));
}

TEST_CASE("AABB3::SetCenter", "[AABB3]")
{
    AABB3d aabb = AABB3d::FromMinAndMax(Point3d(1.0f, 2.0f, 3.f), Point3d(3.0f, 4.0f, 5.f));
    aabb.SetCenter(Point3d(4.0f, 5.0f, 6.f));
    REQUIRE(aabb.GetMin() == Point3d(3.0f, 4.0f, 5.f));
    REQUIRE(aabb.GetMax() == Point3d(5.0f, 6.0f, 7.f));
}

TEST_CASE("AABB3::SetMin", "[AABB3]")
{
    AABB3d aabb = AABB3d::FromMinAndMax(Point3d(1.0f, 2.0f, 3.f), Point3d(3.0f, 4.0f, 5.f));
    aabb.SetMin(Point3d(0.0f, 1.0f, 2.f));
    REQUIRE(aabb.GetMin() == Point3d(0.0f, 1.0f, 2.f));
    REQUIRE(aabb.GetMax() == Point3d(3.0f, 4.0f, 5.f));
}

TEST_CASE("AABB3::SetMax", "[AABB3]")
{
    AABB3d aabb = AABB3d::FromMinAndMax(Point3d(1.0f, 2.0f, 3.f), Point3d(3.0f, 4.0f, 5.f));
    aabb.SetMax(Point3d(5.0f, 7.0f, 10.f));
    REQUIRE(aabb.GetMin() == Point3d(1.0f, 2.0f, 3.f));
    REQUIRE(aabb.GetMax() == Point3d(5.0f, 7.0f, 10.f));
}

TEST_CASE("AABB3::Equality Operator", "[AABB3]")
{
    const AABB3d aabb1 = AABB3d::FromMinAndMax(Point3d(1.0f, 2.0f, 3.f), Point3d(3.0f, 4.0f, 5.f));
    const AABB3d aabb2 = AABB3d::FromMinAndMax(Point3d(1.0f, 2.0f, 3.f), Point3d(3.0f, 4.0f, 5.f));
    const AABB3d aabb3 = AABB3d::FromMinAndMax(Point3d(0.0f, 2.0f, 3.f), Point3d(3.0f, 4.0f, 5.f));
    REQUIRE(aabb1 == aabb2);
    REQUIRE_FALSE(aabb1 == aabb3);
}

TEST_CASE("AABB3::CreateAABB3FromPoints (Span)", "[AABB3]")
{
    const std::array<Point3d, 3> points = {
        Point3d(1.0f, 2.0f, 3.f),
        Point3d(4.0f, 5.0f, 6.f),
        Point3d(-1.0f, -2.0f, -3.f)
    };
    const AABB3d aabb = CreateAABB3FromPoints(std::span<const Point3d>(points));
    REQUIRE(aabb.GetMin() == Point3d(-1.0f, -2.0f, -3.f));
    REQUIRE(aabb.GetMax() == Point3d(4.0f, 5.0f, 6.f));
}

TEST_CASE("AABB3::CreateAABB3FromPoints (Range)", "[AABB3]")
{
    const std::vector<Point3d> points = {
        Point3d(1.0f, 2.0f, 3.f),
        Point3d(4.0f, 5.0f, 6.f),
        Point3d(-1.0f, -2.0f, -3.f)
    };
    const AABB3d aabb = CreateAABB3FromPoints(points);
    REQUIRE(aabb.GetMin() == Point3d(-1.0f, -2.0f, -3.f));
    REQUIRE(aabb.GetMax() == Point3d(4.0f, 5.0f, 6.f));
}

TEST_CASE("AABB3FKF")
{
    struct TestVertex
    {
        Point3d position;
    };

    const std::vector<TestVertex> points = {
       TestVertex{ Point3d(4.0, 2.0, 3.0)},
       TestVertex{ Point3d(1.0, -5.0, 6.0) },
       TestVertex{Point3d(-1.0, -2.0, -3.0)}
    };

    auto t = points | std::ranges::views::transform([](auto vertex) { return vertex.position; });
    const AABB3d aabb = CreateAABB3FromPoints(t);
    REQUIRE(aabb.GetMin() == Point3d(-1.0f, -5.0f, -3.f));
    REQUIRE(aabb.GetMax() == Point3d(4.0f, 2.0f, 6.f));

}