#include <External/Catch2/catch_amalgamated.hpp>

#include "Utility/Math/DynamicVector.hpp"

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

TEST_CASE("DynamicVector::CreateFromValues", "[DynamicVector]")
{
	const DynamicVectorf vector = DynamicVectorf::CreateFromValues({ 5.f, 2.f, -1.f });
	REQUIRE(vector.GetDimensionCount() == 3);
	REQUIRE(vector[0] == 5.f);
	REQUIRE(vector[1] == 2.f);
	REQUIRE(vector[2] == -1.f);
}

TEST_CASE("DynamicVector::Addition operator (DynamicVector+DynamicVector)", "[DynamicVector]")
{
	const DynamicVectorf v1 = DynamicVectorf::CreateFromValues({ 5.f, 2.f, -1.f });
	const DynamicVectorf v2 = DynamicVectorf::CreateFromValues({ 2.f, -1.f, 2.f });
	const DynamicVectorf result = v1 + v2;

	REQUIRE(result[0] == 7.f);
	REQUIRE(result[1] == 1.f);
	REQUIRE(result[2] == 1.f);
}

TEST_CASE("DynamicVector::Subtraction operator (DynamicVector-DynamicVector)", "[DynamicVector]")
{
	const DynamicVectorf v1 = DynamicVectorf::CreateFromValues({ 5.f, 2.f, -1.f });
	const DynamicVectorf v2 = DynamicVectorf::CreateFromValues({ 2.f, -1.f, 2.f });
	const DynamicVectorf result = v1 - v2;

	REQUIRE(result[0] == 3.f);
	REQUIRE(result[1] == 3.f);
	REQUIRE(result[2] == -3.f);
}