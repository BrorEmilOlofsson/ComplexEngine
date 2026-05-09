#include <External/Catch2/catch_amalgamated.hpp>
#include "Engine/Math/Shapes/Cylinder.hpp"

using namespace CLX;

TEST_CASE("Cylinder::Default Constructor", "[Cylinder]") 
{
    Cylinderd cylinder;
    REQUIRE(cylinder.GetCenter() == Point3d(0.0, 0.0, 0.0));
    REQUIRE(cylinder.GetAxis() == UnitVector3d::Up());
    REQUIRE(cylinder.GetRadius() == Radiusd(0.5));
    REQUIRE(cylinder.GetHeight() == 1.0f);
}

TEST_CASE("Cylinder::FromCenterAndRadiusAndAxisAndHeight", "[Cylinder]")
{
    Point3d center{ 1.0, 2.0, 3.0 };
    Radiusd radius{ 5.0 };
    UnitVector3d axis = UnitVector3d::Right();
    double height = 10.0;
    Cylinderd cylinder = Cylinderd::FromCenterAndRadiusAndAxisAndHeight(center, radius, axis, height);
    REQUIRE(cylinder.GetCenter() == center);
    REQUIRE(cylinder.GetRadius() == radius);
    REQUIRE(cylinder.GetAxis() == axis);
    REQUIRE(cylinder.GetHeight() == height);
}

TEST_CASE("Cylinder::GetLowerPoint", "[Cylinder]")
{
    Point3d center{ 0.0, 0.0, 0.0 };
    Radiusd radius{ 2.0 };
    UnitVector3d axis = UnitVector3d::Up();
    double height = 6.0;
    Cylinderd cylinder(center, radius, axis, height);
    Point3d expectedLowerPoint{ 0.0, -3.0, 0.0 };
    REQUIRE(cylinder.GetLowerPoint() == expectedLowerPoint);
}

TEST_CASE("Cylinder::GetUpperPoint", "[Cylinder]")
{
    Point3d center{ 0.0, 0.0, 0.0 };
    Radiusd radius{ 2.0 };
    UnitVector3d axis = UnitVector3d::Up();
    double height = 6.0;
    Cylinderd cylinder(center, radius, axis, height);
    Point3d expectedUpperPoint{ 0.0, 3.0, 0.0 };
    REQUIRE(cylinder.GetUpperPoint() == expectedUpperPoint);
}

TEST_CASE("Cylinder::Setters", "[Cylinder]")
{
    Cylinderd cylinder;
    Point3d newCenter{ 4.0, 5.0, 6.0 };
    Radiusd newRadius{ 3.0 };
    UnitVector3d newAxis = UnitVector3d::Forward();
    double newHeight = 8.0;
    cylinder.SetCenter(newCenter);
    cylinder.SetRadius(newRadius);
    cylinder.SetAxis(newAxis);
    cylinder.SetHeight(newHeight);
    REQUIRE(cylinder.GetCenter() == newCenter);
    REQUIRE(cylinder.GetRadius() == newRadius);
    REQUIRE(cylinder.GetAxis() == newAxis);
    REQUIRE(cylinder.GetHeight() == newHeight);
}

TEST_CASE("Cylinder::Equality Operator", "[Cylinder]")
{
    Cylinderd cylinder1 = Cylinderd::FromCenterAndRadiusAndAxisAndHeight(Point3d(1.0, 2.0, 3.0), Radiusd(4.0), UnitVector3d::Up(), 5.0);
    Cylinderd cylinder2 = Cylinderd::FromCenterAndRadiusAndAxisAndHeight(Point3d(1.0, 2.0, 3.0), Radiusd(4.0), UnitVector3d::Up(), 5.0);
    Cylinderd cylinder3 = Cylinderd::FromCenterAndRadiusAndAxisAndHeight(Point3d(0.0, 0.0, 0.0), Radiusd(4.0), UnitVector3d::Up(), 5.0);
    REQUIRE(cylinder1 == cylinder2);
    REQUIRE_FALSE(cylinder1 == cylinder3);
}