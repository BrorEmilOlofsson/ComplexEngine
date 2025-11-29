#include <External/Catch2/catch_amalgamated.hpp>

#include "Utility/Math/UnitVector2.hpp"
#include "Utility/Math/Vector2.hpp"

using namespace Simple;

TEST_CASE("UnitVector2::default constructor", "[unitvector2]")
{
	const UnitVector2f vector;
	REQUIRE(vector.X() == 0.0f);
	REQUIRE(vector.Y() == 1.0f);
}

TEST_CASE("UnitVector2::parameterized constructor", "[unitvector2]")
{
	{
		// (3,4) normalizes to (0.6, 0.8)
		const UnitVector2f vector(3.0f, 4.0f);
		REQUIRE(NearlyEqual(vector, UnitVector2f(0.6f, 0.8f)));
	}
	{
		// 45 degree vector
		const UnitVector2f vector(0.7071f, 0.7071f);
		REQUIRE(NearlyEqual(vector, UnitVector2f(0.7071f, 0.7071f)));
	}
}

TEST_CASE("UnitVector2::Constants", "[unitvector2]")
{
	constexpr UnitVector2f right = UnitVector2f::Right();
	constexpr UnitVector2f up = UnitVector2f::Up();
	constexpr UnitVector2f left = UnitVector2f::Left();
	constexpr UnitVector2f down = UnitVector2f::Down();

	REQUIRE(right == UnitVector2f(1, 0));
	REQUIRE(up == UnitVector2f(0, 1));
	REQUIRE(left == UnitVector2f(-1, 0));
	REQUIRE(down == UnitVector2f(0, -1));
}

TEST_CASE("UnitVector2::Vector2 constructor", "[unitvector2]")
{
	constexpr Vector2f vec(5.f, 5.f);
	const UnitVector2f unitVec(vec);

	// Normalized (5,5) -> (1/sqrt(2), 1/sqrt(2)) ~= 0.7071067
	REQUIRE(NearlyEqual(unitVec, UnitVector2f(0.7071067f, 0.7071067f), 0.0002f));
}

TEST_CASE("UnitVector2::Setters", "[unitvector2]")
{
	{
		UnitVector2f vector = UnitVector2f::Up();
		vector.SetX(1.f);

		REQUIRE(NearlyEqual(vector.X(), 0.7071f, 0.0001f));
		REQUIRE(NearlyEqual(vector.Y(), 0.7071f, 0.0001f));
	}
	{
		UnitVector2f vector = UnitVector2f::Right();
		vector.SetY(1.f);

		REQUIRE(NearlyEqual(vector.X(), 0.7071f, 0.0001f));
		REQUIRE(NearlyEqual(vector.Y(), 0.7071f, 0.0001f));
	}
}

TEST_CASE("UnitVector2::Negation Operator", "[unitvector2]")
{
	constexpr UnitVector2f vector = UnitVector2f::Right();
	constexpr UnitVector2f negatedVector = -vector;
	REQUIRE(negatedVector.X() == -1.0f);
	REQUIRE(negatedVector.Y() == 0.0f);
}

TEST_CASE("UnitVector2::Equality Operator", "[unitvector2]")
{
	constexpr UnitVector2f vectorA = UnitVector2f::Up();
	constexpr UnitVector2f vectorB = UnitVector2f::Up();
	constexpr UnitVector2f vectorC = UnitVector2f::Right();
	REQUIRE(vectorA == vectorB);
	REQUIRE_FALSE(vectorA == vectorC);
}

TEST_CASE("UnitVector2::Addition Operator UnitVector2+UnitVector2", "[unitvector2]")
{
	constexpr UnitVector2f vectorA = UnitVector2f::Right();
	constexpr UnitVector2f vectorB = UnitVector2f::Up();
	constexpr Vector2f result = vectorA + vectorB;
	REQUIRE(result.x == 1.0f);
	REQUIRE(result.y == 1.0f);
}

TEST_CASE("UnitVector2::Addition Operator Vector2+UnitVector2", "[unitvector2]")
{
	constexpr Vector2f vectorA(4.0f, 5.0f);
	constexpr UnitVector2f vectorB = UnitVector2f::Up();
	constexpr Vector2f result = vectorA + vectorB;
	REQUIRE(result.x == 4.0f);
	REQUIRE(result.y == 6.0f);
}

TEST_CASE("UnitVector2::Addition Operator UnitVector2+Vector2", "[unitvector2]")
{
	constexpr UnitVector2f vectorA = UnitVector2f::Up();
	constexpr Vector2f vectorB(4.0f, 5.0f);
	constexpr Vector2f result = vectorA + vectorB;
	REQUIRE(result.x == 4.0f);
	REQUIRE(result.y == 6.0f);
}

TEST_CASE("UnitVector2::Subtraction Operator UnitVector2-UnitVector2", "[unitvector2]")
{
	constexpr UnitVector2f vectorA = UnitVector2f::Right();
	constexpr UnitVector2f vectorB = UnitVector2f::Up();
	constexpr Vector2f result = vectorA - vectorB;
	REQUIRE(result.x == 1.0f);
	REQUIRE(result.y == -1.0f);
}

TEST_CASE("UnitVector2::Subtraction Operator UnitVector2-Vector2", "[unitvector2]")
{
	constexpr UnitVector2f vectorA = UnitVector2f::Up();
	constexpr Vector2f vectorB(1.0f, 2.0f);
	constexpr Vector2f result = vectorA - vectorB;
	REQUIRE(result.x == -1.0f);
	REQUIRE(result.y == -1.0f);
}

TEST_CASE("UnitVector2::Subtraction Operator Vector2-UnitVector2", "[unitvector2]")
{
	constexpr Vector2f vectorA(4.0f, 5.0f);
	constexpr UnitVector2f vectorB = UnitVector2f::Up();
	constexpr Vector2f result = vectorA - vectorB;
	REQUIRE(result.x == 4.0f);
	REQUIRE(result.y == 4.0f);
}

TEST_CASE("UnitVector2::Multiplication by Scalar", "[unitvector2]")
{
	constexpr UnitVector2f vector = UnitVector2f::Right();
	constexpr float scalar = 3.0f;
	constexpr Vector2f result1 = vector * scalar;
	constexpr Vector2f result2 = scalar * vector;
	REQUIRE(result1.x == 3.0f);
	REQUIRE(result1.y == 0.0f);
	REQUIRE(result2.x == 3.0f);
	REQUIRE(result2.y == 0.0f);
}

TEST_CASE("UnitVector2::Division by Scalar", "[unitvector2]")
{
	constexpr UnitVector2f vector = UnitVector2f::Up();
	constexpr float scalar = 2.0f;
	constexpr Vector2f result1 = vector / scalar;
	REQUIRE(result1.x == 0.0f);
	REQUIRE(result1.y == 0.5f);
}