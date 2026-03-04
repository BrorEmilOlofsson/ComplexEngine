#include <External/Catch2/catch_amalgamated.hpp>
#include "Engine/Math/Shapes/AABB2.hpp"

using namespace CLX;

TEST_CASE("AABB2::Default Constructor", "[AABB2]")
{
    AABB2f aabb;
    REQUIRE(aabb.GetMin() == Point2f::Zero());
    REQUIRE(aabb.GetMax() == Point2f::Zero());
}

TEST_CASE("AABB2::FromMinAndMax", "[AABB2]")
{
    const AABB2f aabb = AABB2f::FromMinAndMax(Point2f(1.0f, 2.0f), Point2f(3.0f, 4.0f));
    REQUIRE(aabb.GetMin() == Point2f(1.0f, 2.0f));
    REQUIRE(aabb.GetMax() == Point2f(3.0f, 4.0f));
}

TEST_CASE("AABB2::FromMinAndExtent", "[AABB2]")
{
    const AABB2f aabb = AABB2f::FromMinAndExtent(Point2f(1.0f, 2.0f), Vector2f(2.0f, 2.0f));
    REQUIRE(aabb.GetMin() == Point2f(1.0f, 2.0f));
    REQUIRE(aabb.GetMax() == Point2f(3.0f, 4.0f));
}

TEST_CASE("AABB2::FromCenterAndExtent", "[AABB2]")
{
    const AABB2f aabb = AABB2f::FromCenterAndExtent(Point2f(2.0f, 3.0f), Vector2f(2.0f, 2.0f));
    REQUIRE(aabb.GetMin() == Point2f(1.0f, 2.0f));
    REQUIRE(aabb.GetMax() == Point2f(3.0f, 4.0f));
}

TEST_CASE("AABB2::FromDefaultAndExtent", "[AABB2]")
{
    const AABB2f aabb = AABB2f::FromDefaultAndExtent(Vector2f(2.0f, 2.0f));
    REQUIRE(aabb.GetMin() == Point2f::Zero());
    REQUIRE(aabb.GetMax() == Point2f(2.0f, 2.0f));
}

TEST_CASE("AABB2::SetMinAndExtent", "[AABB2]")
{
    AABB2f aabb;
    aabb.SetMinAndExtent(Point2f(1.0f, 2.0f), Vector2f(2.0f, 2.0f));
    REQUIRE(aabb.GetMin() == Point2f(1.0f, 2.0f));
    REQUIRE(aabb.GetMax() == Point2f(3.0f, 4.0f));
}

TEST_CASE("AABB2::SetMinAndMax", "[AABB2]")
{
    AABB2f aabb;
    aabb.SetMinAndMax(Point2f(1.0f, 2.0f), Point2f(3.0f, 4.0f));
    REQUIRE(aabb.GetMin() == Point2f(1.0f, 2.0f));
    REQUIRE(aabb.GetMax() == Point2f(3.0f, 4.0f));
}

TEST_CASE("AABB2::SetExtent", "[AABB2]")
{
    AABB2f aabb = AABB2f::FromMinAndMax(Point2f(1.0f, 2.0f), Point2f(3.0f, 4.0f));
    aabb.SetExtent(Vector2f(4.0f, 4.0f));
    REQUIRE(aabb.GetMin() == Point2f(0.0f, 1.0f));
    REQUIRE(aabb.GetMax() == Point2f(4.0f, 5.0f));
}

TEST_CASE("AABB2::SetCenter", "[AABB2]")
{
    AABB2f aabb = AABB2f::FromMinAndMax(Point2f(1.0f, 2.0f), Point2f(3.0f, 4.0f));
    aabb.SetCenter(Point2f(4.0f, 5.0f));
    REQUIRE(aabb.GetMin() == Point2f(3.0f, 4.0f));
    REQUIRE(aabb.GetMax() == Point2f(5.0f, 6.0f));
}

TEST_CASE("AABB2::SetMin", "[AABB2]")
{
    AABB2f aabb = AABB2f::FromMinAndMax(Point2f(1.0f, 2.0f), Point2f(3.0f, 4.0f));
    aabb.SetMin(Point2f(0.0f, 1.0f));
    REQUIRE(aabb.GetMin() == Point2f(0.0f, 1.0f));
    REQUIRE(aabb.GetMax() == Point2f(3.0f, 4.0f));
}

TEST_CASE("AABB2::SetMax", "[AABB2]")
{
    AABB2f aabb = AABB2f::FromMinAndMax(Point2f(1.0f, 2.0f), Point2f(3.0f, 4.0f));
    aabb.SetMax(Point2f(5.0f, 7.0f));
    REQUIRE(aabb.GetMin() == Point2f(1.0f, 2.0f));
    REQUIRE(aabb.GetMax() == Point2f(5.0f, 7.0f));
}

TEST_CASE("AABB2::Equality Operator", "[AABB2]")
{
    const AABB2f aabb1 = AABB2f::FromMinAndMax(Point2f(1.0f, 2.0f), Point2f(3.0f, 4.0f));
    const AABB2f aabb2 = AABB2f::FromMinAndMax(Point2f(1.0f, 2.0f), Point2f(3.0f, 4.0f));
    const AABB2f aabb3 = AABB2f::FromMinAndMax(Point2f(0.0f, 2.0f), Point2f(3.0f, 4.0f));
    REQUIRE(aabb1 == aabb2);
    REQUIRE_FALSE(aabb1 == aabb3);
}