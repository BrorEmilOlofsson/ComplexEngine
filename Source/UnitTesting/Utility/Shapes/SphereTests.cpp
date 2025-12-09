#include <External/Catch2/catch_amalgamated.hpp>
#include "Utility/Shapes/Sphere.hpp"

using namespace Simple;

TEST_CASE("Sphere::Default Constructor", "[Sphere]")
{
    Spheref sphere;

    REQUIRE(sphere.GetCenter() == Point3f::Zero());
    REQUIRE(sphere.GetRadius()  == Radiusf(0.0f));
}

TEST_CASE("Sphere::FromCenterAndRadius", "[Sphere]")
{
    Point3f center(1.0f, 2.0f, 3.0f);
    Radiusf radius(5.0f);
    Spheref sphere = Spheref::FromCenterAndRadius(center, radius);
    REQUIRE(sphere.GetCenter() == center);
    REQUIRE(sphere.GetRadius()  == radius);
}

TEST_CASE("Sphere::FromCenter", "[Sphere]")
{
    Point3f center(1.0f, 2.0f, 3.0f);
    Spheref sphere = Spheref::FromCenter(center);
    REQUIRE(sphere.GetCenter() == center);
    REQUIRE(sphere.GetRadius()  == Radiusf(0.0f));
}

TEST_CASE("Sphere::FromRadius", "[Sphere]")
{
    Radiusf radius(5.0f);
    Spheref sphere = Spheref::FromRadius(radius);
    REQUIRE(sphere.GetCenter() == Point3f::Zero());
    REQUIRE(sphere.GetRadius()  == radius);
}

TEST_CASE("Sphere::SetCenter", "[Sphere]")
{
    Spheref sphere;
    Point3f center(1.0f, 2.0f, 3.0f);
    sphere.SetCenter(center);
    REQUIRE(sphere.GetCenter() == center);
}

TEST_CASE("Sphere::SetRadius", "[Sphere]")
{
    Spheref sphere;
    Radiusf radius(5.0f);
    sphere.SetRadius(radius);
    REQUIRE(sphere.GetRadius() == radius);
}

TEST_CASE("Sphere::GetDiameter", "[Sphere]")
{
    Radiusf radius(5.0f);
    Spheref sphere = Spheref::FromRadius(radius);
    REQUIRE(sphere.GetDiameter() == Diameterf(10.0f));
}