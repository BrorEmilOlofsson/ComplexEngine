#include <External/Catch2/catch_amalgamated.hpp>
#include "Utility/Shapes/AABB3.hpp"

using namespace Simple;

TEST_CASE("AABB3::Default Constructor", "[AABB2]")
{
    AABB3f aabb;
    REQUIRE(aabb.GetMin() == Point3f::Zero());
    REQUIRE(aabb.GetMax() == Point3f::Zero());
}

TEST_CASE("AABB3::FromMinAndMax", "[AABB2]")
{
    const AABB3f aabb = AABB3f::FromMinAndMax(Point3f(1.0f, 2.0f, 3.f), Point3f(3.0f, 4.0f, 5.f));
    REQUIRE(aabb.GetMin() == Point3f(1.0f, 2.0f, 3.f));
    REQUIRE(aabb.GetMax() == Point3f(3.0f, 4.0f, 5.f));
}

TEST_CASE("AABB3::FromMinAndExtent", "[AABB2]")
{
    const AABB3f aabb = AABB3f::FromMinAndExtent(Point3f(1.0f, 2.0f, 3.f), Vector3f(2.0f, 2.0f, 2.f));
    REQUIRE(aabb.GetMin() == Point3f(1.0f, 2.0f, 3.f));
    REQUIRE(aabb.GetMax() == Point3f(3.0f, 4.0f, 5.f));
}

TEST_CASE("AABB3::FromCenterAndExtent", "[AABB2]")
{
    const AABB3f aabb = AABB3f::FromCenterAndExtent(Point3f(2.0f, 3.0f, 4.f), Vector3f(2.0f, 2.0f, 2.f));
    REQUIRE(aabb.GetMin() == Point3f(1.0f, 2.0f, 3.f));
    REQUIRE(aabb.GetMax() == Point3f(3.0f, 4.0f, 5.f));
}

TEST_CASE("AABB3::FromDefaultAndExtent", "[AABB2]")
{
    const AABB3f aabb = AABB3f::FromDefaultAndExtent(Vector3f(2.0f, 2.0f, 2.f));
    REQUIRE(aabb.GetMin() == Point3f::Zero());
    REQUIRE(aabb.GetMax() == Point3f(2.0f, 2.0f, 2.f));
}

TEST_CASE("AABB3::SetMinAndExtent", "[AABB2]")
{
    AABB3f aabb;
    aabb.SetMinAndExtent(Point3f(1.0f, 2.0f, 3.f), Vector3f(2.0f, 2.0f, 2.f));
    REQUIRE(aabb.GetMin() == Point3f(1.0f, 2.0f, 3.f));
    REQUIRE(aabb.GetMax() == Point3f(3.0f, 4.0f, 5.f));
}

TEST_CASE("AABB3::SetMinAndMax", "[AABB2]")
{
    AABB3f aabb;
    aabb.SetMinAndMax(Point3f(1.0f, 2.0f, 3.f), Point3f(3.0f, 4.0f, 5.f));
    REQUIRE(aabb.GetMin() == Point3f(1.0f, 2.0f, 3.f));
    REQUIRE(aabb.GetMax() == Point3f(3.0f, 4.0f, 5.f));
}

TEST_CASE("AABB3::SetExtent", "[AABB2]")
{
    AABB3f aabb = AABB3f::FromMinAndMax(Point3f(1.0f, 2.0f, -1.f), Point3f(3.0f, 4.0f, 2.f));
    aabb.SetExtent(Vector3f(4.0f, 4.0f, 4.f));
    REQUIRE(aabb.GetMin() == Point3f(0.0f, 1.0f, -1.5f));
    REQUIRE(aabb.GetMax() == Point3f(4.0f, 5.0f, 2.5f));
}

TEST_CASE("AABB3::SetCenter", "[AABB2]")
{
    AABB3f aabb = AABB3f::FromMinAndMax(Point3f(1.0f, 2.0f, 3.f), Point3f(3.0f, 4.0f, 5.f));
    aabb.SetCenter(Point3f(4.0f, 5.0f, 6.f));
    REQUIRE(aabb.GetMin() == Point3f(3.0f, 4.0f, 5.f));
    REQUIRE(aabb.GetMax() == Point3f(5.0f, 6.0f, 7.f));
}

TEST_CASE("AABB3::SetMin", "[AABB2]")
{
    AABB3f aabb = AABB3f::FromMinAndMax(Point3f(1.0f, 2.0f, 3.f), Point3f(3.0f, 4.0f, 5.f));
    aabb.SetMin(Point3f(0.0f, 1.0f, 2.f));
    REQUIRE(aabb.GetMin() == Point3f(0.0f, 1.0f, 2.f));
    REQUIRE(aabb.GetMax() == Point3f(3.0f, 4.0f, 5.f));
}

TEST_CASE("AABB3::SetMax", "[AABB2]")
{
    AABB3f aabb = AABB3f::FromMinAndMax(Point3f(1.0f, 2.0f, 3.f), Point3f(3.0f, 4.0f, 5.f));
    aabb.SetMax(Point3f(5.0f, 7.0f, 10.f));
    REQUIRE(aabb.GetMin() == Point3f(1.0f, 2.0f, 3.f));
    REQUIRE(aabb.GetMax() == Point3f(5.0f, 7.0f, 10.f));
}

TEST_CASE("AABB3::Equality Operator", "[AABB2]")
{
    const AABB3f aabb1 = AABB3f::FromMinAndMax(Point3f(1.0f, 2.0f, 3.f), Point3f(3.0f, 4.0f, 5.f));
    const AABB3f aabb2 = AABB3f::FromMinAndMax(Point3f(1.0f, 2.0f, 3.f), Point3f(3.0f, 4.0f, 5.f));
    const AABB3f aabb3 = AABB3f::FromMinAndMax(Point3f(0.0f, 2.0f, 3.f), Point3f(3.0f, 4.0f, 5.f));
    REQUIRE(aabb1 == aabb2);
    REQUIRE_FALSE(aabb1 == aabb3);
}