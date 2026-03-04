#include <External/Catch2/catch_amalgamated.hpp>
#include "Engine/Math/UnitVector3.hpp"

using namespace Simple;

TEST_CASE("UnitVector3::default constructor", "[UnitVector3]")
{
	const UnitVector3f vector;
	REQUIRE(vector.X() == 0.0f);
	REQUIRE(vector.Y() == 0.0f);
	REQUIRE(vector.Z() == 1.0f);
}

TEST_CASE("UnitVector3::parameterized constructor", "[UnitVector3]")
{
	const UnitVector3f vector(0.57735f, 0.57735f, 0.57735f);
	REQUIRE(NearlyEqual(vector.X(), 0.57735f));
	REQUIRE(NearlyEqual(vector.Y(), 0.57735f));
	REQUIRE(NearlyEqual(vector.Z(), 0.57735f));
}

TEST_CASE("UnitVector3::Constants", "[UnitVector3]")
{
	constexpr UnitVector3f right = UnitVector3f::Right();
	constexpr UnitVector3f up = UnitVector3f::Up();
	constexpr UnitVector3f forward = UnitVector3f::Forward();
	constexpr UnitVector3f left = UnitVector3f::Left();
	constexpr UnitVector3f down = UnitVector3f::Down();
	constexpr UnitVector3f backward = UnitVector3f::Backward();
	REQUIRE(right == UnitVector3f(1, 0, 0));
	REQUIRE(up == UnitVector3f(0, 1, 0));
	REQUIRE(forward == UnitVector3f(0, 0, 1));
	REQUIRE(left == UnitVector3f(-1, 0, 0));
	REQUIRE(down == UnitVector3f(0, -1, 0));
	REQUIRE(backward == UnitVector3f(0, 0, -1));
}

TEST_CASE("UnitVector3::Vector3 constructor", "[UnitVector3]")
{
	{

		constexpr Vector3f vec(0.7071f, 0.0f, 0.7071f);
		const UnitVector3f unitVec(vec);
		REQUIRE(NearlyEqual(unitVec.X(), 0.7071f));
		REQUIRE(NearlyEqual(unitVec.Y(), 0.0f));
		REQUIRE(NearlyEqual(unitVec.Z(), 0.7071f));
	}

	{
		constexpr Vector3f vec(5.f, 5.f, 5.f);
		const UnitVector3f unitVec(vec);

		REQUIRE(NearlyEqual(unitVec.X(), 0.577350f));
		REQUIRE(NearlyEqual(unitVec.Y(), 0.577350f));
		REQUIRE(NearlyEqual(unitVec.Z(), 0.577350f));

	}

}

TEST_CASE("UnitVector3::Setters", "[UnitVector3]")
{
	{

		UnitVector3f vector = UnitVector3f::Forward();
		vector.SetX(1.f);

		REQUIRE(NearlyEqual(vector.X(), 0.7071f));
		REQUIRE(vector.Y() == 0.0f);
		REQUIRE(NearlyEqual(vector.Z(), 0.7071f));
	}
	{
		UnitVector3f vector = UnitVector3f::Right();
		vector.SetY(1.f);
		REQUIRE(NearlyEqual(vector.X(), 0.7071f));
		REQUIRE(NearlyEqual(vector.Y(), 0.7071f));
		REQUIRE(vector.Z() == 0.0f);
	}
	{
		UnitVector3f vector = UnitVector3f::Up();
		vector.SetZ(1.f);
		REQUIRE(vector.X() == 0.0f);
		REQUIRE(NearlyEqual(vector.Y(), 0.7071f));
		REQUIRE(NearlyEqual(vector.Z(), 0.7071f));
	}
}

TEST_CASE("UnitVector3::Negation Operator", "[UnitVector3]")
{
	constexpr UnitVector3f vector = UnitVector3f::Right();
	constexpr UnitVector3f negatedVector = -vector;
	REQUIRE(negatedVector.X() == -1.0f);
	REQUIRE(negatedVector.Y() == 0.0f);
	REQUIRE(negatedVector.Z() == 0.0f);
}

TEST_CASE("UnitVector3::Equality Operator", "[UnitVector3]")
{
	constexpr UnitVector3f vectorA = UnitVector3f::Up();
	constexpr UnitVector3f vectorB = UnitVector3f::Up();
	constexpr UnitVector3f vectorC = UnitVector3f::Forward();
	REQUIRE(vectorA == vectorB);
	REQUIRE(vectorA != vectorC);
}

TEST_CASE("UnitVector3::Addition Operator UnitVector3+UnitVector3", "[UnitVector3]")
{
	constexpr UnitVector3f vectorA = UnitVector3f::Right();
	constexpr UnitVector3f vectorB = UnitVector3f::Up();
	constexpr Vector3f result = vectorA + vectorB;
	REQUIRE(result.x == 1.0f);
	REQUIRE(result.y == 1.0f);
	REQUIRE(result.z == 0.0f);
}

TEST_CASE("UnitVector3::Addition Operator Vector3+UnitVector3", "[UnitVector3]")
{
	constexpr Vector3f vectorA(1.0f, 2.0f, 3.0f);
	constexpr UnitVector3f vectorB = UnitVector3f::Forward();
	constexpr Vector3f result = vectorA + vectorB;
	REQUIRE(result.x == 1.0f);
	REQUIRE(result.y == 2.0f);
	REQUIRE(result.z == 4.0f);
}

TEST_CASE("UnitVector3::Addition Operator UnitVector3+Vector3", "[UnitVector3]")
{
	constexpr UnitVector3f vectorA = UnitVector3f::Up();
	constexpr Vector3f vectorB(4.0f, 5.0f, 6.0f);
	constexpr Vector3f result = vectorA + vectorB;
	REQUIRE(result.x == 4.0f);
	REQUIRE(result.y == 6.0f);
	REQUIRE(result.z == 6.0f);
}

TEST_CASE("UnitVector3::Addition Assignment Operator", "[UnitVector3]")
{
	Vector3f vectorA(1.0f, 1.0f, 1.0f);
	const UnitVector3f vectorB = UnitVector3f::Right();
	vectorA += vectorB;
	REQUIRE(vectorA.x == 2.0f);
	REQUIRE(vectorA.y == 1.0f);
	REQUIRE(vectorA.z == 1.0f);
}

TEST_CASE("UnitVector3::Subtraction Operator UnitVector3-UnitVector3", "[UnitVector3]")
{
	constexpr UnitVector3f vectorA = UnitVector3f::Right();
	constexpr UnitVector3f vectorB = UnitVector3f::Up();
	constexpr Vector3f result = vectorA - vectorB;
	REQUIRE(result.x == 1.0f);
	REQUIRE(result.y == -1.0f);
	REQUIRE(result.z == 0.0f);
}

TEST_CASE("UnitVector3::Subtraction Operator (UnitVector3-Vector3)", "[UnitVector3]")
{
	constexpr UnitVector3f vectorA = UnitVector3f::Forward();
	constexpr Vector3f vectorB(1.0f, 2.0f, 3.0f);
	constexpr Vector3f result = vectorA - vectorB;
	REQUIRE(result.x == -1.0f);
	REQUIRE(result.y == -2.0f);
	REQUIRE(result.z == -2.0f);
}

TEST_CASE("UnitVector3::Subtraction Operator (Vector3-UnitVector3)", "[UnitVector3]")
{
	constexpr Vector3f vectorA(4.0f, 5.0f, 6.0f);
	constexpr UnitVector3f vectorB = UnitVector3f::Up();
	constexpr Vector3f result = vectorA - vectorB;
	REQUIRE(result.x == 4.0f);
	REQUIRE(result.y == 4.0f);
	REQUIRE(result.z == 6.0f);
}

TEST_CASE("UnitVector3::Subtraction Assignment Operator", "[UnitVector3]")
{
	Vector3f vectorA(3.0f, 3.0f, 3.0f);
	const UnitVector3f vectorB = UnitVector3f::Forward();
	vectorA -= vectorB;
	REQUIRE(vectorA.x == 3.0f);
	REQUIRE(vectorA.y == 3.0f);
	REQUIRE(vectorA.z == 2.0f);
}

TEST_CASE("UnitVector3::Multiplication by Scalar", "[UnitVector3]")
{
	constexpr UnitVector3f vector = UnitVector3f::Right();
	constexpr float scalar = 3.0f;
	constexpr Vector3f result1 = vector * scalar;
	constexpr Vector3f result2 = scalar * vector;
	REQUIRE(result1.x == 3.0f);
	REQUIRE(result1.y == 0.0f);
	REQUIRE(result1.z == 0.0f);
	REQUIRE(result2.x == 3.0f);
	REQUIRE(result2.y == 0.0f);
	REQUIRE(result2.z == 0.0f);
}

TEST_CASE("UnitVector3::Division by Scalar", "[UnitVector3]")
{
	constexpr UnitVector3f vector = UnitVector3f::Up();
	constexpr float scalar = 2.0f;
	constexpr Vector3f result1 = vector / scalar;
	REQUIRE(result1.x == 0.0f);
	REQUIRE(result1.y == 0.5f);
	REQUIRE(result1.z == 0.0f);
}

TEST_CASE("UnitVector3::NearlyEqual", "[UnitVector3]")
{
	constexpr UnitVector3d v1 = UnitVector3d::Up();
	UnitVector3d v2 = UnitVector3d::Up();
	v2.SetX(0.00001);
	REQUIRE(NearlyEqual(v1, v2));
}