#include <External/Catch2/catch_amalgamated.hpp>

#include "Utility/Math/VectorMath.hpp"
#include "Utility/Math/Vector3.hpp"

TEST_CASE("Vector3 addition", "[vector3]")
{
    const Simple::Vector3f a(1, 2, 3);
    const Simple::Vector3f b(4, 5, 6);
    const Simple::Vector3f result = a + b;

    REQUIRE(result.x == 5);
    REQUIRE(result.y == 7);
    REQUIRE(result.z == 9);
}

TEST_CASE("Vector3 subtraction", "[vector3]")
{
    const Simple::Vector3f a(11, 2, -13);
    const Simple::Vector3f b(4, 5, 6);
    const Simple::Vector3f result = a - b;

    REQUIRE(result.x == 7);
    REQUIRE(result.y == -3);
    REQUIRE(result.z == -19);
}

TEST_CASE("Vector3 multiplication", "[vector3]")
{
    const Simple::Vector3f a(11, 2, -13);
    const Simple::Vector3f b(4, 5, 6);
    const Simple::Vector3f result = a * b;

    REQUIRE(result.x == 44);
    REQUIRE(result.y == 10);
    REQUIRE(result.z == -78);
}

TEST_CASE("Vector3 division", "[vector3]")
{
    const Simple::Vector3f a(11, 2, -13);
    const Simple::Vector3f b(4, 5, 6);
    const Simple::Vector3f result = a / b;

    REQUIRE(result.x == 11.f / 4);
    REQUIRE(result.y == 0.4f);
    REQUIRE(result.z == -13.f / 6.f);
}

TEST_CASE("Vector3 Constants", "[vector3]")
{
    constexpr Simple::UnitVector3f right = Simple::UnitVector3f::Right();
	constexpr Simple::UnitVector3f up = Simple::UnitVector3f::Up();
    constexpr Simple::UnitVector3f forward = Simple::UnitVector3f::Forward();
	REQUIRE(right == Simple::UnitVector3f(1, 0, 0));
	REQUIRE(up == Simple::UnitVector3f(0, 1, 0));
	REQUIRE(forward == Simple::UnitVector3f(0, 0, 1));
}

TEST_CASE("Vector3 Cross", "[vector3]")
{
    constexpr Simple::UnitVector3f right = Simple::UnitVector3f::Right();
    constexpr Simple::UnitVector3f up = Simple::UnitVector3f::Up();
	constexpr Simple::UnitVector3f forward = Simple::UnitVector3f::Forward();
	constexpr Simple::UnitVector3f left = Simple::UnitVector3f::Left();
	constexpr Simple::UnitVector3f down = Simple::UnitVector3f::Down();
	constexpr Simple::UnitVector3f backward = Simple::UnitVector3f::Backward();

    {
        const Simple::UnitVector3f result = Simple::Cross(right, up);
        REQUIRE(result == Simple::UnitVector3f::Forward());
    }
	{
        const Simple::UnitVector3f result = Simple::Cross(up, right);
		REQUIRE(result == Simple::UnitVector3f::Backward());
	}
    {
        const Simple::UnitVector3f result = Simple::Cross(up, forward);
		REQUIRE(result == Simple::UnitVector3f::Right());
    }
    {
        const Simple::UnitVector3f result = Simple::Cross(forward, up);
        REQUIRE(result == Simple::UnitVector3f::Left());
	}
	{
		const Simple::UnitVector3f result = Simple::Cross(forward, right);
		REQUIRE(result == Simple::UnitVector3f::Up());
	}
    {
        const Simple::UnitVector3f result = Simple::Cross(right, forward);
        REQUIRE(result == Simple::UnitVector3f::Down());
    }
    {
        const Simple::UnitVector3f result = Simple::Cross(backward, right);
		REQUIRE(result == Simple::UnitVector3f::Down());
    }
}