#include <External/Catch2/catch_amalgamated.hpp>
#include "Engine/Math/GeometryMath.hpp"

using namespace Simple;

TEST_CASE("GeometryMath::Equality Operator (Radius)", "[GeometryMath]")
{
    constexpr Radiusd radiusA{ 5.0f };
    constexpr Radiusd radiusB{ 5.0f };
    constexpr Radiusd radiusC{ 3.0f };
    REQUIRE(radiusA == radiusB);
    REQUIRE(radiusA != radiusC);
}

TEST_CASE("GeometryMath::Multiplication Operator (Radius * Scalar)", "[GeometryMath]")
{
    constexpr Radiusd radiusA{ 3.0 };
    constexpr double scalar = 2.0;
    const auto result = radiusA * scalar;
    REQUIRE(result == Radiusd(6.0));
}