#include <External/Catch2/catch_amalgamated.hpp>
#include "Utility/Shapes/Sphere.hpp"

using namespace Simple;

TEST_CASE("Sphere::Default Constructor", "[Sphere]")
{
    Sphered sphere;

    REQUIRE(sphere.GetCenter() == Point3d::Zero());
    REQUIRE(sphere.GetRadius()  == Radiusd(0.0f));
}

TEST_CASE("Sphere::FromCenterAndRadius", "[Sphere]")
{
    Point3d center(1.0f, 2.0f, 3.0f);
    Radiusd radius(5.0f);
    Sphered sphere = Sphered::FromCenterAndRadius(center, radius);
    REQUIRE(sphere.GetCenter() == center);
    REQUIRE(sphere.GetRadius()  == radius);
}

TEST_CASE("Sphere::FromCenter", "[Sphere]")
{
    Point3d center(1.0f, 2.0f, 3.0f);
    Sphered sphere = Sphered::FromCenter(center);
    REQUIRE(sphere.GetCenter() == center);
    REQUIRE(sphere.GetRadius()  == Radiusd(0.0f));
}

TEST_CASE("Sphere::FromRadius", "[Sphere]")
{
    Radiusd radius(5.0f);
    Sphered sphere = Sphered::FromRadius(radius);
    REQUIRE(sphere.GetCenter() == Point3d::Zero());
    REQUIRE(sphere.GetRadius()  == radius);
}

TEST_CASE("Sphere::SetCenter", "[Sphere]")
{
    Sphered sphere;
    Point3d center(1.0f, 2.0f, 3.0f);
    sphere.SetCenter(center);
    REQUIRE(sphere.GetCenter() == center);
}

TEST_CASE("Sphere::SetRadius", "[Sphere]")
{
    Sphered sphere;
    Radiusd radius(5.0f);
    sphere.SetRadius(radius);
    REQUIRE(sphere.GetRadius() == radius);
}

TEST_CASE("Sphere::GetDiameter", "[Sphere]")
{
    Radiusd radius(5.0f);
    Sphered sphere = Sphered::FromRadius(radius);
    REQUIRE(sphere.GetDiameter() == Diameterd(10.0f));
}

TEST_CASE("Sphere::Equality Operator", "[Sphere]")
{
    Sphered sphere1 = Sphered::FromCenterAndRadius(Point3d(1.0f, 2.0f, 3.0f), Radiusd(5.0f));
    Sphered sphere2 = Sphered::FromCenterAndRadius(Point3d(1.0f, 2.0f, 3.0f), Radiusd(5.0f));
    Sphered sphere3 = Sphered::FromCenterAndRadius(Point3d(4.0f, 5.0f, 6.0f), Radiusd(5.0f));
    REQUIRE(sphere1 == sphere2);
    REQUIRE_FALSE(sphere1 == sphere3);
}