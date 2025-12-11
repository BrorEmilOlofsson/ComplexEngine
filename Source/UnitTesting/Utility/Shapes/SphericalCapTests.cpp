#include <External/Catch2/catch_amalgamated.hpp>
#include "Utility/Shapes/SphericalCap.hpp"

using namespace Simple;

TEST_CASE("SphericalCap::Default Constructor", "[ShapeMath]")
{
    SphericalCapd cap;
    REQUIRE(cap.GetSphereCenter() == Point3d::Zero());
    REQUIRE(cap.GetSphereRadius() == Radiusd(0.0));
    REQUIRE(cap.GetNormal() == UnitVector3d::Up());
    REQUIRE(cap.GetHeight() == Heightd(0.0));
}

TEST_CASE("SphericalCap::FromSphereAndNormalAndHeight", "[ShapeMath]")
{
    const Sphered sphere = Sphered::FromCenterAndRadius(Point3d(1.0, 2.0, 3.0), Radiusd(5.0));
    const UnitVector3d normal = UnitVector3d::Forward();
    const Heightd height = Heightd(2.0);
    const SphericalCapd cap = SphericalCapd::FromSphereAndNormalAndHeight(sphere, normal, height);
    REQUIRE(cap.GetSphereCenter() == sphere.GetCenter());
    REQUIRE(cap.GetSphereRadius() == sphere.GetRadius());
    REQUIRE(cap.GetNormal() == normal);
    REQUIRE(cap.GetHeight() == height);
}

TEST_CASE("SphericalCap::GetApex", "[ShapeMath]")
{
    const Sphered sphere = Sphered::FromCenterAndRadius(Point3d(1.0, 1.0, 1.0), Radiusd(5.0));
    const UnitVector3d normal = UnitVector3d::Down();
    const Heightd height = Heightd(3.0);
    const SphericalCapd cap = SphericalCapd::FromSphereAndNormalAndHeight(sphere, normal, height);
    const Point3d expectedApex = Point3d(1.0, -4.0, 1.0);
    const Point3d apex = cap.GetApex();
    REQUIRE(apex == expectedApex);
}

TEST_CASE("SphericalCap::GetBaseCenter", "[ShapeMath]")
{
    const Sphered sphere = Sphered::FromCenterAndRadius(Point3d(1.0, 1.0, 1.0), Radiusd(5.0));
    const UnitVector3d normal = UnitVector3d::Down();
    const Heightd height = Heightd(3.0);
    const SphericalCapd cap = SphericalCapd::FromSphereAndNormalAndHeight(sphere, normal, height);
    const Point3d expectedBaseCenter = Point3d(1.0, -1.0, 1.0);
    const Point3d baseCenter = cap.GetBaseCenter();
    REQUIRE(baseCenter == expectedBaseCenter);
}

TEST_CASE("SphericalCap::SetSphereCenter", "[ShapeMath]")
{
    SphericalCapd cap;
    const Point3d newCenter(2.0, 3.0, 4.0);
    cap.SetSphereCenter(newCenter);
    REQUIRE(cap.GetSphereCenter() == newCenter);
}

TEST_CASE("SphericalCap::SetSphereRadius", "[ShapeMath]")
{
    SphericalCapd cap;
    const Radiusd newRadius(7.0);
    cap.SetSphereRadius(newRadius);
    REQUIRE(cap.GetSphereRadius() == newRadius);
}

TEST_CASE("SphericalCap::SetNormal", "[ShapeMath]")
{
    SphericalCapd cap;
    const UnitVector3d newNormal = UnitVector3d::Left();
    cap.SetNormal(newNormal);
    REQUIRE(cap.GetNormal() == newNormal);
}

TEST_CASE("SphericalCap::SetHeight", "[ShapeMath]")
{
    SphericalCapd cap;
    const Heightd newHeight(4.0);
    cap.SetHeight(newHeight);
    REQUIRE(cap.GetHeight() == newHeight);
}

TEST_CASE("SphericalCap::Equality Operator", "[ShapeMath]")
{
    const Sphered sphere = Sphered::FromCenterAndRadius(Point3d(0.0, 0.0, 0.0), Radiusd(5.0));
    const UnitVector3d normal = UnitVector3d::Up();
    const Heightd height = Heightd(2.0);
    const SphericalCapd cap1 = SphericalCapd::FromSphereAndNormalAndHeight(sphere, normal, height);
    const SphericalCapd cap2 = SphericalCapd::FromSphereAndNormalAndHeight(sphere, normal, height);
    REQUIRE(cap1 == cap2);
}