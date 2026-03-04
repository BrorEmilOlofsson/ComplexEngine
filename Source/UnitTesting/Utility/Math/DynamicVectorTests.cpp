#include <External/Catch2/catch_amalgamated.hpp>

#include "Engine/Math/DynamicVector.hpp"
#include "Engine/Math/Math.hpp"

using namespace Simple;

TEST_CASE("DynamicVector::Default Constructor", "[DynamicVector]")
{
	const DynamicVectorf vector;
	REQUIRE(vector.GetDimensionCount() == 0);
}

TEST_CASE("DynamicVector::CreateFromDimensionCount", "[DynamicVector]")
{
	const DynamicVectorf vector = DynamicVectorf::CreateFromDimensionCount(5);
	REQUIRE(vector.GetDimensionCount() == 5);
	REQUIRE(vector[0] == 0.f);
	REQUIRE(vector[1] == 0.f);
	REQUIRE(vector[2] == 0.f);
	REQUIRE(vector[3] == 0.f);
	REQUIRE(vector[4] == 0.f);
}

TEST_CASE("DynamicVector::CreateFromValues (Initializer List)", "[DynamicVector]")
{
	const DynamicVectorf vector = DynamicVectorf::CreateFromValues({ 5.f, 2.f, -1.f });
	REQUIRE(vector.GetDimensionCount() == 3);
	REQUIRE(vector[0] == 5.f);
	REQUIRE(vector[1] == 2.f);
	REQUIRE(vector[2] == -1.f);
}

TEST_CASE("DynamicVector::CreateFromValues (Variadic)", "[DynamicVector]")
{
	const DynamicVectorf vector = DynamicVectorf::CreateFromValues(5.f, 2.f, -1.f);
	REQUIRE(vector.GetDimensionCount() == 3);
	REQUIRE(vector[0] == 5.f);
	REQUIRE(vector[1] == 2.f);
	REQUIRE(vector[2] == -1.f);
}

TEST_CASE("DynamicVector::SetDimensionCount", "[DynamicVector]")
{
	DynamicVectorf vector = DynamicVectorf::CreateFromValues(1.f, 2.f);
	REQUIRE(vector.GetDimensionCount() == 2);
	vector.SetDimensionCount(4);
	REQUIRE(vector.GetDimensionCount() == 4);
	REQUIRE(vector[0] == 1.f);
	REQUIRE(vector[1] == 2.f);
	REQUIRE(vector[2] == 0.f);
	REQUIRE(vector[3] == 0.f);
}

TEST_CASE("DynamicVector::AddDimension", "[DynamicVector]")
{
	DynamicVectorf vector = DynamicVectorf::CreateFromValues({ 1.0f, 2.0f });
	REQUIRE(vector.GetDimensionCount() == 2);
	vector.AddDimension();
	REQUIRE(vector.GetDimensionCount() == 3);
	REQUIRE(vector[0] == 1.0f);
	REQUIRE(vector[1] == 2.0f);
	REQUIRE(vector[2] == 0.0f);
}

TEST_CASE("DynamicVector::Addition operator (DynamicVector + DynamicVector)", "[DynamicVector]")
{
	const DynamicVectorf v1 = DynamicVectorf::CreateFromValues(5.f, 2.f, -1.f);
	const DynamicVectorf v2 = DynamicVectorf::CreateFromValues(2.f, -1.f, 2.f);
	const DynamicVectorf result = v1 + v2;

	REQUIRE(result[0] == 7.f);
	REQUIRE(result[1] == 1.f);
	REQUIRE(result[2] == 1.f);
}

TEST_CASE("DynamicVector::Subtraction operator (DynamicVector - DynamicVector)", "[DynamicVector]")
{
	const DynamicVectorf v1 = DynamicVectorf::CreateFromValues({ 5.f, 2.f, -1.f });
	const DynamicVectorf v2 = DynamicVectorf::CreateFromValues({ 2.f, -1.f, 2.f });
	const DynamicVectorf result = v1 - v2;

	REQUIRE(result[0] == 3.f);
	REQUIRE(result[1] == 3.f);
	REQUIRE(result[2] == -3.f);
}

TEST_CASE("DynamicVector::Negation operator (-DynamicVector)", "[DynamicVector]")
{
	const DynamicVectorf v = DynamicVectorf::CreateFromValues({ 5.f, -2.f, 1.f });
	const DynamicVectorf result = -v;
	REQUIRE(result[0] == -5.f);
	REQUIRE(result[1] == 2.f);
	REQUIRE(result[2] == -1.f);
}

TEST_CASE("DynamicVector::Multiplication Operator (DynamicUnitVector * Scalar)", "[DynamicUnitVector]")
{
	const DynamicVectorf unitVector = DynamicVectorf::CreateFromValues(0.6f, 0.8f);
	const float scalar = 5.0f;
	const DynamicVectorf result = unitVector * scalar;
	REQUIRE(result.GetDimensionCount() == 2);
	REQUIRE(NearlyEqual(result[0], 3.0f));
	REQUIRE(NearlyEqual(result[1], 4.0f));
}

TEST_CASE("DynamicVector::Multiplication Operator (Scalar * DynamicUnitVector)", "[DynamicUnitVector]")
{
	const DynamicVectorf unitVector = DynamicVectorf::CreateFromValues(0.6f, 0.8f);
	const float scalar = 5.0f;
	const DynamicVectorf result = scalar * unitVector;
	REQUIRE(result.GetDimensionCount() == 2);
	REQUIRE(NearlyEqual(result[0], 3.0f));
	REQUIRE(NearlyEqual(result[1], 4.0f));
}

TEST_CASE("DynamicVector::Division Operator (DynamicUnitVector / Scalar)", "[DynamicUnitVector]")
{
	const DynamicVectorf unitVector = DynamicVectorf::CreateFromValues(3.0f, 4.0f);
	const float scalar = 5.0f;
	const DynamicVectorf result = unitVector / scalar;
	REQUIRE(result.GetDimensionCount() == 2);
	REQUIRE(NearlyEqual(result[0], 0.6f));
	REQUIRE(NearlyEqual(result[1], 0.8f));
}

TEST_CASE("DynamicVector::Equality Operator", "[DynamicVector]")
{
	const DynamicVectorf vectorA = DynamicVectorf::CreateFromValues(1.0f, 2.0f, 3.0f);
	const DynamicVectorf vectorB = DynamicVectorf::CreateFromValues(1.0f, 2.0f, 3.0f);
	const DynamicVectorf vectorC = DynamicVectorf::CreateFromValues(4.0f, 5.0f, 6.0f);
	REQUIRE(vectorA == vectorB);
	REQUIRE(!(vectorA == vectorC));
}

TEST_CASE("DynamicVector::Inequality Operator", "[DynamicVector]")
{
	const DynamicVectorf vectorA = DynamicVectorf::CreateFromValues(1.0f, 2.0f, 3.0f);
	const DynamicVectorf vectorB = DynamicVectorf::CreateFromValues(1.0f, 2.0f, 3.0f);
	const DynamicVectorf vectorC = DynamicVectorf::CreateFromValues(4.0f, 5.0f, 6.0f);
	REQUIRE(!(vectorA != vectorB));
	REQUIRE(vectorA != vectorC);
}

TEST_CASE("DynamicVector::Addition Assignment Operator", "[DynamicVector]")
{
	DynamicVectorf v = DynamicVectorf::CreateFromValues(1.f, 2.f, 3.f);
	const DynamicVectorf addend = DynamicVectorf::CreateFromValues(4.f, 5.f, 6.f);
	v += addend;
	REQUIRE(v[0] == 5.f);
	REQUIRE(v[1] == 7.f);
	REQUIRE(v[2] == 9.f);
}

TEST_CASE("DynamicVector::Subtraction Assignment Operator", "[DynamicVector]")
{
	DynamicVectorf v = DynamicVectorf::CreateFromValues(5.f, 7.f, 9.f);
	const DynamicVectorf subtrahend = DynamicVectorf::CreateFromValues(4.f, 5.f, 6.f);
	v -= subtrahend;
	REQUIRE(v[0] == 1.f);
	REQUIRE(v[1] == 2.f);
	REQUIRE(v[2] == 3.f);
}

TEST_CASE("DynamicVector::Multiplication Assignment Operator (DynamicVector *= DynamicVector)", "[DynamicVector]")
{
	DynamicVectorf v = DynamicVectorf::CreateFromValues(1.f, 2.f, 3.f);
	const DynamicVectorf b = DynamicVectorf::CreateFromValues(2.f, 9.f, 1.f);
	v *= b;
	REQUIRE(v[0] == 2.f);
	REQUIRE(v[1] == 18.f);
	REQUIRE(v[2] == 3.f);
}

TEST_CASE("DynamicVector::Multiplication Assignment Operator (DynamicVector *= Scalar)", "[DynamicVector]")
{
	DynamicVectorf v = DynamicVectorf::CreateFromValues(1.f, 2.f, 3.f);
	const float scalar = 3.f;
	v *= scalar;
	REQUIRE(v[0] == 3.f);
	REQUIRE(v[1] == 6.f);
	REQUIRE(v[2] == 9.f);
}

TEST_CASE("DynamicVector::Division Assignment Operator (DynamicVector /= DynamicVector)", "[DynamicVector]")
{
	DynamicVectorf v = DynamicVectorf::CreateFromValues(3.f, 6.f, 9.f);
	const DynamicVectorf b = DynamicVectorf::CreateFromValues(1.f, 2.f, 3.f);
	v /= b;
	REQUIRE(v[0] == 3.f);
	REQUIRE(v[1] == 3.f);
	REQUIRE(v[2] == 3.f);
}

TEST_CASE("DynamicVector::Division Assignment Operator (DynamicVector /= Scalar)", "[DynamicVector]")
{
	DynamicVectorf v = DynamicVectorf::CreateFromValues(3.f, 6.f, 9.f);
	const float scalar = 3.f;
	v /= scalar;
	REQUIRE(v[0] == 1.f);
	REQUIRE(v[1] == 2.f);
	REQUIRE(v[2] == 3.f);
}