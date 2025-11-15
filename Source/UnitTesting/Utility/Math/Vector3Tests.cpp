#include <External/Catch2/catch_amalgamated.hpp>

#include "Utility/Math/VectorMath.hpp"
#include "Utility/Math/Vector3.hpp"

using namespace Simple;

TEST_CASE("Vector3 addition", "[vector3]")
{
    const Vector3f a(1, 2, 3);
    const Vector3f b(4, 5, 6);
    const Vector3f result = a + b;

    REQUIRE(result.x == 5);
    REQUIRE(result.y == 7);
    REQUIRE(result.z == 9);
}

TEST_CASE("Vector3 subtraction", "[vector3]")
{
    const Vector3f a(11, 2, -13);
    const Vector3f b(4, 5, 6);
    const Vector3f result = a - b;

    REQUIRE(result.x == 7);
    REQUIRE(result.y == -3);
    REQUIRE(result.z == -19);
}

TEST_CASE("Vector3 multiplication", "[vector3]")
{
    const Vector3f a(11, 2, -13);
    const Vector3f b(4, 5, 6);
    const Vector3f result = a * b;

    REQUIRE(result.x == 44);
    REQUIRE(result.y == 10);
    REQUIRE(result.z == -78);
}

TEST_CASE("Vector3 division", "[vector3]")
{
    const Vector3f a(11, 2, -13);
    const Vector3f b(4, 5, 6);
    const Vector3f result = a / b;

    REQUIRE(result.x == 11.f / 4);
    REQUIRE(result.y == 0.4f);
    REQUIRE(result.z == -13.f / 6.f);
}

TEST_CASE("Vector3 Constants", "[vector3]")
{
    constexpr UnitVector3f right = UnitVector3f::Right();
	constexpr UnitVector3f up = UnitVector3f::Up();
    constexpr UnitVector3f forward = UnitVector3f::Forward();
	REQUIRE(right == UnitVector3f(1, 0, 0));
	REQUIRE(up == UnitVector3f(0, 1, 0));
	REQUIRE(forward == UnitVector3f(0, 0, 1));
}

TEST_CASE("Vector3 Cross", "[vector3]")
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