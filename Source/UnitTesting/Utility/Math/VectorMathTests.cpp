#include <External/Catch2/catch_amalgamated.hpp>

#include "Utility/Math/VectorMath.hpp"

using namespace Simple;

TEST_CASE("VectorMath::Vector3 Dot Product", "[VectorMath]")
{
	const Vector3f a(1.0f, 2.0f, 3.0f);
	const Vector3f b(4.0f, -5.0f, 6.0f);
	const float result = Dot(a, b);
	REQUIRE(result == (1.0f * 4.0f + 2.0f * -5.0f + 3.0f * 6.0f));
}

TEST_CASE("VectorMath::Vector3 Cross", "[VectorMath]")
{
    constexpr UnitVector3f right = UnitVector3f::Right();
    constexpr UnitVector3f up = UnitVector3f::Up();
    constexpr UnitVector3f forward = UnitVector3f::Forward();
    constexpr UnitVector3f left = UnitVector3f::Left();
    constexpr UnitVector3f down = UnitVector3f::Down();
    constexpr UnitVector3f backward = UnitVector3f::Backward();

    {
        const UnitVector3f result = Cross(right, up);
        REQUIRE(result == UnitVector3f::Forward());
    }
    {
        const UnitVector3f result = Cross(up, right);
        REQUIRE(result == UnitVector3f::Backward());
    }
    {
        const UnitVector3f result = Cross(up, forward);
        REQUIRE(result == UnitVector3f::Right());
    }
    {
        const UnitVector3f result = Cross(forward, up);
        REQUIRE(result == UnitVector3f::Left());
    }
    {
        const UnitVector3f result = Cross(forward, right);
        REQUIRE(result == UnitVector3f::Up());
    }
    {
        const UnitVector3f result = Cross(right, forward);
        REQUIRE(result == UnitVector3f::Down());
    }
    {
        const UnitVector3f result = Cross(backward, right);
        REQUIRE(result == UnitVector3f::Down());
    }
}

TEST_CASE("VectorMath::GetUnitVector")
{
    constexpr Point3f p1(10, 20.f, 100.f);
    constexpr Point3f p2(10.f, 21.f, 101.f);

    REQUIRE(GetUnitVector(p1, p2) == UnitVector3f(Vector3f(0, 1, 1)));
}


TEST_CASE("VectorMath::GetReflectionVector")
{
    {
        constexpr UnitVector3f forward = UnitVector3f::Forward();
        constexpr UnitVector3f backward = UnitVector3f::Backward();
        constexpr UnitVector3f reflection = GetReflectionVector(forward, backward);
        REQUIRE(reflection == backward);
    }
    {
        constexpr Vector3f direction(1, 1, 1);
        constexpr UnitVector3f normal = UnitVector3f::Backward();
        const UnitVector3f reflection = GetReflectionVector(direction, normal);
        REQUIRE(NearlyEqual(reflection, UnitVector3f(1, 1, -1)));
    }
}

TEST_CASE("VectorMath::GetPerpendicularVector")
{
    constexpr UnitVector3f forward = UnitVector3f::Forward();

    const UnitVector3f perp = GetPerpendicularVector(forward);

    REQUIRE(Dot(forward, perp) == 0.f);
}

TEST_CASE("VectorMath::InterpPoint")
{
    constexpr Point3f point1(10, 0, 0);
    constexpr Point3f target;

    const Point3f p = InterpPoint(point1, target, 10.f, 0.1f);

    REQUIRE(p == Point3f(9, 0, 0));
}

TEST_CASE("VectorMath::CalculateAngle")
{
    {
        const UnitVector3f forward = UnitVector3f::Forward();
        const UnitVector3f right = UnitVector3f::Right();

        const Radiansf angle = CalculateAngle(forward, right);
        REQUIRE(angle == ToRadians(Degreesf(90)));
    }

    {
        const UnitVector3f vec1(0, 1, 0);
        const UnitVector3f vec2(1, 0, 1);

        const Radiansf angle = CalculateAngle(vec1, vec2);
        REQUIRE(angle == ToRadians(Degreesf(90)));
    }

    {
        const UnitVector3f vec1(1, 0, 0);
        const UnitVector3f vec2(1, 1, 0);

        const Radiansf angle = CalculateAngle(vec1, vec2);
        REQUIRE(angle == ToRadians(Degreesf(45)));
    }
}


TEST_CASE("Average Point")
{

    constexpr Point2f p1 = Point2f::Zero();
    constexpr Point2f p2 = Point2f::One();

    constexpr Point2f p3 = AveragePoint(p1, p2);

}

TEST_CASE("VectorMath::ToClipCoords")
{
    constexpr Point2i point(0, 50);
    constexpr Vector2ui size(100u, 100u);

    constexpr Point2f clipCoords = ToClipCoords(point, size);

    REQUIRE(clipCoords == Point2f(-1, 0));
}