#include <External/Catch2/catch_amalgamated.hpp>

#include "Utility/Math/Vector2.hpp"

using namespace Simple;

TEST_CASE("Vector2 Default Construction", "[vector2]")
{
	constexpr Vector2f vecf;
	REQUIRE(vecf.x == 0.0f);
	REQUIRE(vecf.y == 0.0f);
	constexpr Vector2d vecd;
	REQUIRE(vecd.x == 0.0);
	REQUIRE(vecd.y == 0.0);
	constexpr Vector2i veci;
	REQUIRE(veci.x == 0);
	REQUIRE(veci.y == 0);
}

TEST_CASE("Vector2 Construction", "[vector2]")
{
	constexpr Vector2f vec1(3.0f, 4.0f);
	REQUIRE(vec1.x == 3.0f);
	REQUIRE(vec1.y == 4.0f);
	constexpr Vector2d vec2(5.0, 6.0);
	REQUIRE(vec2.x == 5.0);
	REQUIRE(vec2.y == 6.0);
	constexpr Vector2i vec3(7, 8);
	REQUIRE(vec3.x == 7);
	REQUIRE(vec3.y == 8);
}

TEST_CASE("Vector2 Constants", "[vector2]")
{
	constexpr Vector2f zero = Vector2f::Zero();
	constexpr Vector2f one = Vector2f::One();
	REQUIRE(zero == Vector2f(0.0f, 0.0f));
	REQUIRE(one == Vector2f(1.0f, 1.0f));
}

TEST_CASE("Vector2 Equality Operator", "[vector2]")
{
	constexpr Vector2i vecA(1, 2);
	constexpr Vector2i vecB(1, 2);
	constexpr Vector2i vecC(2, 3);
	REQUIRE(vecA == vecB);
	REQUIRE(vecA != vecC);
}

TEST_CASE("Vector2 Type Conversion", "[vector2]")
{
	constexpr Vector2i vecInt(3, 4);
	constexpr Vector2f vecFloat = static_cast<Vector2f>(vecInt);
	REQUIRE(vecFloat.x == 3.0f);
	REQUIRE(vecFloat.y == 4.0f);
	constexpr Vector2d vecDouble = static_cast<Vector2d>(vecInt);
	REQUIRE(vecDouble.x == 3.0);
	REQUIRE(vecDouble.y == 4.0);
}

TEST_CASE("Vector2 addition", "[vector2]")
{
	constexpr Vector2i vecA(1, 2);
	constexpr Vector2i vecB(3, 4);
	constexpr Vector2i result = vecA + vecB;
	REQUIRE(result == Vector2i(4, 6));
}

TEST_CASE("Vector2 subtraction", "[vector2]")
{
	constexpr Vector2i vecA(5, 7);
	constexpr Vector2i vecB(2, 3);
	constexpr Vector2i result = vecA - vecB;
	REQUIRE(result == Vector2i(3, 4));
}

TEST_CASE("Vector2 addition assignment", "[vector2]")
{
	Vector2i vecA(1, 2);
	const Vector2i vecB(3, 4);
	vecA += vecB;
	REQUIRE(vecA == Vector2i(4, 6));
}

TEST_CASE("Vector2 subtraction assignment", "[vector2]")
{
	Vector2i vecA(5, 7);
	const Vector2i vecB(2, 3);
	vecA -= vecB;
	REQUIRE(vecA == Vector2i(3, 4));
}

TEST_CASE("Vector2 component-wise multiplication", "[vector2]")
{
	constexpr Vector2i vecA(2, 3);
	constexpr Vector2i vecB(4, 5);
	constexpr Vector2i result = vecA * vecB;
	REQUIRE(result == Vector2i(8, 15));
}

TEST_CASE("Vector2 component-wise division", "[vector2]")
{
	constexpr Vector2i vecA(8, 15);
	constexpr Vector2i vecB(2, 3);
	constexpr Vector2i result = vecA / vecB;
	REQUIRE(result == Vector2i(4, 5));
}

TEST_CASE("Vector2 scalar multiplication", "[vector2]")
{
	constexpr Vector2f vec(2, 3);
	constexpr float scalar = 4;
	constexpr Vector2f result1 = vec * scalar;
	constexpr Vector2f result2 = scalar * vec;
	REQUIRE(result1 == Vector2f(8, 12));
	REQUIRE(result2 == Vector2f(8, 12));
}

TEST_CASE("Vector2 scalar division", "[vector2]")
{
	constexpr Vector2f vec(8, 12);
	constexpr float scalar = 4;
	constexpr Vector2f result = vec / scalar;
	REQUIRE(result == Vector2f(2, 3));
}

TEST_CASE("Vector2 scalar multiplication assignment", "[vector2]")
{
	Vector2f vec(2, 3);
	const float scalar = 4;
	vec *= scalar;
	REQUIRE(vec == Vector2f(8, 12));
}

TEST_CASE("Vector2 scalar division assignment", "[vector2]")
{
	Vector2f vec(8, 12);
	const float scalar = 4;
	vec /= scalar;
	REQUIRE(vec == Vector2f(2, 3));
}

TEST_CASE("Vector2 negation", "[vector2]")
{
	constexpr Vector2i vec(3, -4);
	constexpr Vector2i result = -vec;
	REQUIRE(result == Vector2i(-3, 4));
}