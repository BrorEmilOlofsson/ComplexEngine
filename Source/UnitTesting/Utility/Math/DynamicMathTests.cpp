#include <External/Catch2/catch_amalgamated.hpp>
#include "Engine/Math/DynamicPoint.hpp"
#include "Engine/Math/DynamicVector.hpp"
#include "Engine/Math/DynamicMath.hpp"
#include "Engine/Math/DynamicUnitVector.hpp"
#include "Engine/Math/Math.hpp"

using namespace CLX;

TEST_CASE("DynamicMath::GetUnitVector", "[DynamicMath]")
{
	const DynamicPointf pointA = DynamicPointf::CreateFromValues({ 1.0f, 2.0f, 3.0f });
	const DynamicPointf pointB = DynamicPointf::CreateFromValues({ 4.0f, 6.0f, 8.0f });
	const DynamicUnitVectorf unitVector = GetUnitVector(pointA, pointB);
	REQUIRE(NearlyEqual(unitVector[0], 0.42426407f));
	REQUIRE(NearlyEqual(unitVector[1], 0.56568542f));
	REQUIRE(NearlyEqual(unitVector[2], 0.70710678f));
}

TEST_CASE("DynamicMath::DistanceSquared", "[DynamicMath]")
{
	const DynamicPointf pointA = DynamicPointf::CreateFromValues({ 1.0f, 2.0f, 3.0f });
	const DynamicPointf pointB = DynamicPointf::CreateFromValues({ 4.0f, 6.0f, 8.0f });
	const float distanceSquared = DistanceSquared(pointA, pointB);
	REQUIRE(NearlyEqual(distanceSquared, 50.0f));
}

TEST_CASE("DynamicMath::Distance", "[DynamicMath]")
{
	const DynamicPointf pointA = DynamicPointf::CreateFromValues({ 1.0f, 2.0f, 3.0f });
	const DynamicPointf pointB = DynamicPointf::CreateFromValues({ 4.0f, 6.0f, 8.0f });

	const float distance = Distance(pointA, pointB);
	REQUIRE(NearlyEqual(distance, 7.07106781f));
}