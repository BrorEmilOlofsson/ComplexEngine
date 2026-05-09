#include <External/Catch2/catch_amalgamated.hpp>

#include "Engine/Math/UnitVector2.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Utility/Approximation.hpp"

using namespace CLX;

TEST_CASE("UnitVector2::Default Constructor", "[UnitVector2]")
{
	const UnitVector2d vector;
	REQUIRE(vector.X() == 0.0f);
	REQUIRE(vector.Y() == 1.0f);
}

TEST_CASE("UnitVector2::Parameterized Constructor", "[UnitVector2]")
{
	{
		// (3,4) normalizes to (0.6, 0.8)
		const UnitVector2d vector(3.0f, 4.0f);
		REQUIRE(NearlyEqual(vector, UnitVector2d(0.6f, 0.8f)));
	}
	{
		// 45 degree vector
		const UnitVector2d vector(0.7071f, 0.7071f);
		REQUIRE(NearlyEqual(vector, UnitVector2d(0.7071f, 0.7071f)));
	}
}

TEST_CASE("UnitVector2::Constants", "[UnitVector2]")
{
	constexpr UnitVector2d right = UnitVector2d::Right();
	constexpr UnitVector2d up = UnitVector2d::Up();
	constexpr UnitVector2d left = UnitVector2d::Left();
	constexpr UnitVector2d down = UnitVector2d::Down();

	REQUIRE(right == UnitVector2d(1, 0));
	REQUIRE(up == UnitVector2d(0, 1));
	REQUIRE(left == UnitVector2d(-1, 0));
	REQUIRE(down == UnitVector2d(0, -1));
}

TEST_CASE("UnitVector2::Vector2 Constructor", "[UnitVector2]")
{
	constexpr Vector2d vec(5.f, 5.f);
	const UnitVector2d unitVec(vec);

	// Normalized (5,5) -> (1/sqrt(2), 1/sqrt(2)) ~= 0.7071067
	REQUIRE(unitVec == Approx(UnitVector2d(0.7071067, 0.7071067)));
}

TEST_CASE("UnitVector2::Setters", "[UnitVector2]")
{
	{
		UnitVector2d vector = UnitVector2d::Up();
		vector.SetX(1.f);

		REQUIRE(vector.X() == Approx(0.7071));
		REQUIRE(vector.Y() == Approx(0.7071));
	}
	{
		UnitVector2d vector = UnitVector2d::Right();
		vector.SetY(1.f);

		REQUIRE(vector.X() == Approx(0.7071));
		REQUIRE(vector.Y() == Approx(0.7071));
	}
}

TEST_CASE("UnitVector2::Negation Operator", "[UnitVector2]")
{
	constexpr UnitVector2d vector = UnitVector2d::Right();
	constexpr UnitVector2d negatedVector = -vector;
	REQUIRE(negatedVector.X() == -1.0f);
	REQUIRE(negatedVector.Y() == 0.0f);
}

TEST_CASE("UnitVector2::Equality Operator", "[UnitVector2]")
{
	constexpr UnitVector2d vectorA = UnitVector2d::Up();
	constexpr UnitVector2d vectorB = UnitVector2d::Up();
	constexpr UnitVector2d vectorC = UnitVector2d::Right();
	REQUIRE(vectorA == vectorB);
	REQUIRE_FALSE(vectorA == vectorC);
}

TEST_CASE("UnitVector2::Addition Operator (UnitVector2 + UnitVector2)", "[UnitVector2]")
{
	constexpr UnitVector2d vectorA = UnitVector2d::Right();
	constexpr UnitVector2d vectorB = UnitVector2d::Up();
	constexpr Vector2d result = vectorA + vectorB;
	REQUIRE(result.x == 1.0f);
	REQUIRE(result.y == 1.0f);
}

TEST_CASE("UnitVector2::Addition Operator (Vector2 + UnitVector2)", "[UnitVector2]")
{
	constexpr Vector2d vectorA(4.0f, 5.0f);
	constexpr UnitVector2d vectorB = UnitVector2d::Up();
	constexpr Vector2d result = vectorA + vectorB;
	REQUIRE(result.x == 4.0f);
	REQUIRE(result.y == 6.0f);
}

TEST_CASE("UnitVector2::Addition Operator (UnitVector2 + Vector2)", "[UnitVector2]")
{
	constexpr UnitVector2d vectorA = UnitVector2d::Up();
	constexpr Vector2d vectorB(4.0f, 5.0f);
	constexpr Vector2d result = vectorA + vectorB;
	REQUIRE(result.x == 4.0f);
	REQUIRE(result.y == 6.0f);
}

TEST_CASE("UnitVector2::Subtraction Operator (UnitVector2 - UnitVector2)", "[UnitVector2]")
{
	constexpr UnitVector2d vectorA = UnitVector2d::Right();
	constexpr UnitVector2d vectorB = UnitVector2d::Up();
	constexpr Vector2d result = vectorA - vectorB;
	REQUIRE(result.x == 1.0f);
	REQUIRE(result.y == -1.0f);
}

TEST_CASE("UnitVector2::Subtraction Operator (UnitVector2 - Vector2)", "[UnitVector2]")
{
	constexpr UnitVector2d vectorA = UnitVector2d::Up();
	constexpr Vector2d vectorB(1.0f, 2.0f);
	constexpr Vector2d result = vectorA - vectorB;
	REQUIRE(result.x == -1.0f);
	REQUIRE(result.y == -1.0f);
}

TEST_CASE("UnitVector2::Subtraction Operator (Vector2 - UnitVector2)", "[UnitVector2]")
{
	constexpr Vector2d vectorA(4.0f, 5.0f);
	constexpr UnitVector2d vectorB = UnitVector2d::Up();
	constexpr Vector2d result = vectorA - vectorB;
	REQUIRE(result.x == 4.0f);
	REQUIRE(result.y == 4.0f);
}

TEST_CASE("UnitVector2::Multiplication Operator (UnitVector2 * Scalar)", "[UnitVector2]")
{
	constexpr UnitVector2d vector = UnitVector2d::Right();
	constexpr double scalar = 3.0f;
	constexpr Vector2d result1 = vector * scalar;
	constexpr Vector2d result2 = scalar * vector;
	REQUIRE(result1.x == 3.0f);
	REQUIRE(result1.y == 0.0f);
	REQUIRE(result2.x == 3.0f);
	REQUIRE(result2.y == 0.0f);
}

TEST_CASE("UnitVector2::Division Operator (UnitVector2 / Scalar)", "[UnitVector2]")
{
	constexpr UnitVector2d vector = UnitVector2d::Up();
	constexpr double scalar = 2.0f;
	constexpr Vector2d result1 = vector / scalar;
	REQUIRE(result1.x == 0.0f);
	REQUIRE(result1.y == 0.5f);
}