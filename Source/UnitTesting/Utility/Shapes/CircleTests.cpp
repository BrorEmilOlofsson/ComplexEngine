#include <External/Catch2/catch_amalgamated.hpp>
#include "Engine/Math/Shapes/Circle.hpp"

using namespace Simple;

TEST_CASE("Circle::Default Constructor", "[Circle]")
{
    Circled circle;
    REQUIRE(circle.GetCenter() == Point2d::Zero());
    REQUIRE(circle.GetRadius()  == Radiusd(0.0f));
}

TEST_CASE("Circle::FromCenterAndRadius", "[Circle]")
{
    const Circled circle = Circled::FromCenterAndRadius(Point2d::One(), Radiusd(5.f));
    REQUIRE(circle.GetCenter() == Point2d::One());
    REQUIRE(circle.GetRadius() == Radiusd(5.f));
}

TEST_CASE("Circle::FromCenter", "[Circle]")
{
    const Circled circle = Circled::FromCenter(Point2d::One());
    REQUIRE(circle.GetCenter() == Point2d::One());
    REQUIRE(circle.GetRadius() == Radiusd(0.f));
}

TEST_CASE("Circle::FromRadius", "[Circle]")
{
    const Circled circle = Circled::FromRadius(Radiusd(5.f));
    REQUIRE(circle.GetCenter() == Point2d::Zero());
    REQUIRE(circle.GetRadius() == Radiusd(5.f));
}

TEST_CASE("Circle::SetCenter", "[Circle]")
{
    Circled circle;
    circle.SetCenter(Point2d::One());
    REQUIRE(circle.GetCenter() == Point2d::One());
}

TEST_CASE("Circle::SetRadius", "[Circle]")
{
    Circled circle;
    circle.SetRadius(Radiusd(5.f));
    REQUIRE(circle.GetRadius() == Radiusd(5.f));
}

TEST_CASE("Circle::GetDiameter", "[Circle]")
{
    const Circled circle = Circled::FromRadius(Radiusd(5.f));
    REQUIRE(circle.GetDiameter() == Diameterd(10.f));
}

TEST_CASE("Circle::GetCircumference", "[Circle]")
{
    const Circled circle = Circled::FromRadius(Radiusd(5.f));
    REQUIRE(circle.GetCircumference() == Circumferenced(2.0 * PI<double> * 5.f));
}

TEST_CASE("Circle::Equality Operator", "[Circle]")
{
    Circled circle1 = Circled::FromCenterAndRadius(Point2d::One(), Radiusd(5.f));
    Circled circle2 = Circled::FromCenterAndRadius(Point2d::One(), Radiusd(5.f));
    Circled circle3 = Circled::FromCenterAndRadius(Point2d::One(), Radiusd(7.f));
    REQUIRE(circle1 == circle2);
    REQUIRE_FALSE(circle1 == circle3);
}