#include <External/Catch2/catch_amalgamated.hpp>

#include "Utility/Math/DynamicUnitVector.hpp"
#include "Utility/Math/Math.hpp"

using namespace Simple;

TEST_CASE("DynamicUnitVector::Default Constructor", "[DynamicUnitVector]")
{
	const DynamicUnitVectorf unitVector;
	REQUIRE(unitVector.GetDimensionCount() == 1);
	REQUIRE(unitVector[0] == 1.0f);
}

TEST_CASE("DynamicUnitVector::CreateFromDimensionCount", "[DynamicUnitVector]")
{
	constexpr std::size_t dimensionCount = 3;
	const DynamicUnitVectorf unitVector = DynamicUnitVectorf::CreateFromDimensionCount(dimensionCount);
	REQUIRE(unitVector.GetDimensionCount() == dimensionCount);
	REQUIRE(unitVector[0] == 1.0f);
	for (std::size_t i = 1; i < dimensionCount; ++i)
	{
		REQUIRE(unitVector[i] == 0.0f);
	}
}

TEST_CASE("DynamicUnitVector::CreateFromValues (InitializerList)", "[DynamicUnitVector]")
{
	const DynamicUnitVectorf unitVector = DynamicUnitVectorf::CreateFromValues({ 3.0f, 4.0f });
	REQUIRE(unitVector.GetDimensionCount() == 2);
	REQUIRE(NearlyEqual(unitVector[0], 0.6f));
	REQUIRE(NearlyEqual(unitVector[1], 0.8f));
}

TEST_CASE("DynamicUnitVector::CreateFromValues (Variadic)", "[DynamicUnitVector]")
{
	const DynamicUnitVectorf unitVector = DynamicUnitVectorf::CreateFromValues(3.0f, 4.0f);
	REQUIRE(unitVector.GetDimensionCount() == 2);
	REQUIRE(NearlyEqual(unitVector[0], 0.6f));
	REQUIRE(NearlyEqual(unitVector[1], 0.8f));
}

TEST_CASE("DynamicUnitVector::CreateFromVector", "[DynamicUnitVector]")
{
	const DynamicVectorf vector = DynamicVectorf::CreateFromValues(0.0f, 5.0f, 0.0f);
	const DynamicUnitVectorf unitVector = DynamicUnitVectorf::CreateFromVector(vector);
	REQUIRE(unitVector.GetDimensionCount() == 3);
	REQUIRE(NearlyEqual(unitVector[0], 0.0f));
	REQUIRE(NearlyEqual(unitVector[1], 1.0f));
	REQUIRE(NearlyEqual(unitVector[2], 0.0f));
}

TEST_CASE("DynamicUnitVector::AddDimension", "[DynamicUnitVector]")
{
	DynamicUnitVectorf unitVector = DynamicUnitVectorf::CreateFromValues(1.0f, 0.0f);
	REQUIRE(unitVector.GetDimensionCount() == 2);
	unitVector.AddDimension();
	REQUIRE(unitVector.GetDimensionCount() == 3);
	REQUIRE(NearlyEqual(unitVector[0], 1.0f));
	REQUIRE(NearlyEqual(unitVector[1], 0.0f));
	REQUIRE(NearlyEqual(unitVector[2], 0.0f));
}

TEST_CASE("DynamicUnitVector::Equality Operator", "[DynamicUnitVector]")
{
	const DynamicUnitVectorf unitVectorA = DynamicUnitVectorf::CreateFromValues(1.0f, 0.0f);
	const DynamicUnitVectorf unitVectorB = DynamicUnitVectorf::CreateFromValues(1.0f, 0.0f);
	const DynamicUnitVectorf unitVectorC = DynamicUnitVectorf::CreateFromValues(0.0f, 1.0f);
	REQUIRE(unitVectorA == unitVectorB);
	REQUIRE(!(unitVectorA == unitVectorC));
}

TEST_CASE("DynamicUnitVector::Inequality Operator", "[DynamicUnitVector]")
{
	const DynamicUnitVectorf unitVectorA = DynamicUnitVectorf::CreateFromValues(1.0f, 0.0f);
	const DynamicUnitVectorf unitVectorB = DynamicUnitVectorf::CreateFromValues(1.0f, 0.0f);
	const DynamicUnitVectorf unitVectorC = DynamicUnitVectorf::CreateFromValues(0.0f, 1.0f);
	REQUIRE(!(unitVectorA != unitVectorB));
	REQUIRE(unitVectorA != unitVectorC);
}

TEST_CASE("DynamicUnitVector::Addition Operator (DynamicUnitVector + DynamicVector)", "[DynamicUnitVector]")
{
	const DynamicUnitVectorf unitVector = DynamicUnitVectorf::CreateFromValues(1.0f, 0.0f);
	const DynamicVectorf vector = DynamicVectorf::CreateFromValues(2.0f, 3.0f);
	const DynamicVectorf result = unitVector + vector;
	REQUIRE(result.GetDimensionCount() == 2);
	REQUIRE(result[0] == 3.0f);
	REQUIRE(result[1] == 3.0f);
}

TEST_CASE("DynamicUnitVector::Addition Operator (DynamicVector + DynamicUnitVector)", "[DynamicUnitVector]")
{
	const DynamicUnitVectorf unitVector = DynamicUnitVectorf::CreateFromValues(1.0f, 0.0f);
	const DynamicVectorf vector = DynamicVectorf::CreateFromValues(2.0f, 3.0f);
	const DynamicVectorf result = vector + unitVector;
	REQUIRE(result.GetDimensionCount() == 2);
	REQUIRE(result[0] == 3.0f);
	REQUIRE(result[1] == 3.0f);
}

TEST_CASE("DynamicUnitVector::Subtraction Operator (DynamicUnitVector - DynamicVector)", "[DynamicUnitVector]")
{
	const DynamicUnitVectorf unitVector = DynamicUnitVectorf::CreateFromValues(1.0f, 0.0f);
	const DynamicVectorf vector = DynamicVectorf::CreateFromValues(0.5f, 0.5f);
	const DynamicVectorf result = unitVector - vector;
	REQUIRE(result.GetDimensionCount() == 2);
	REQUIRE(NearlyEqual(result[0], 0.5f));
	REQUIRE(NearlyEqual(result[1], -0.5f));
}

TEST_CASE("DynamicUnitVector::Subtraction Operator (DynamicVector - DynamicUnitVector)", "[DynamicUnitVector]")
{
	const DynamicUnitVectorf unitVector = DynamicUnitVectorf::CreateFromValues(1.0f, 0.0f);
	const DynamicVectorf vector = DynamicVectorf::CreateFromValues(0.5f, 0.5f);
	const DynamicVectorf result = vector - unitVector;
	REQUIRE(result.GetDimensionCount() == 2);
	REQUIRE(NearlyEqual(result[0], -0.5f));
	REQUIRE(NearlyEqual(result[1], 0.5f));
}

TEST_CASE("DynamicUnitVector::Multiplication Operator (DynamicUnitVector * Scalar)", "[DynamicUnitVector]")
{
	const DynamicUnitVectorf unitVector = DynamicUnitVectorf::CreateFromValues(0.6f, 0.8f);
	const float scalar = 5.0f;
	const DynamicVectorf result = unitVector * scalar;
	REQUIRE(result.GetDimensionCount() == 2);
	REQUIRE(NearlyEqual(result[0], 3.0f));
	REQUIRE(NearlyEqual(result[1], 4.0f));
}

TEST_CASE("DynamicUnitVector::Multiplication Operator (Scalar * DynamicUnitVector)", "[DynamicUnitVector]")
{
	const DynamicUnitVectorf unitVector = DynamicUnitVectorf::CreateFromValues(0.6f, 0.8f);
	const float scalar = 5.0f;
	const DynamicVectorf result = scalar * unitVector;
	REQUIRE(result.GetDimensionCount() == 2);
	REQUIRE(NearlyEqual(result[0], 3.0f));
	REQUIRE(NearlyEqual(result[1], 4.0f));
}