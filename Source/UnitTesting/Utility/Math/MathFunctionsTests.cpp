#include <External/Catch2/catch_amalgamated.hpp>

#include "Utility/Math/Math.hpp"
#include "Utility/Math/Vector2.hpp"
#include "Utility/Math/Point2.hpp"
//#include "Engine/Global.hpp"

TEST_CASE("Math::Square", "[math]")
{
	REQUIRE(Simple::Square(2) == 4);
	REQUIRE(Simple::Square(-2) == 4);
	REQUIRE(Simple::Square(0) == 0);

	REQUIRE(Simple::Square(2.f) == 4.f);
	REQUIRE(Simple::Square(-2.f) == 4.f);
	REQUIRE(Simple::Square(0.f) == 0);
}

TEST_CASE("Math::Abs", "[math]")
{
	REQUIRE(Simple::Abs(2) == 2);
	REQUIRE(Simple::Abs(-2) == 2);
	REQUIRE(Simple::Abs(0) == 0);

	REQUIRE(Simple::Abs(2.f) == 2);
	REQUIRE(Simple::Abs(-2.f) == 2);
	REQUIRE(Simple::Abs(0.f) == 0);
}

TEST_CASE("Math::Sign", "[math]")
{
	REQUIRE(Simple::Sign(2) == 1);
	REQUIRE(Simple::Sign(-2) == -1);
	REQUIRE(Simple::Sign(0) == 0);

	REQUIRE(Simple::Sign(2.f) == 1);
	REQUIRE(Simple::Sign(-2.f) == -1);
	REQUIRE(Simple::Sign(0.0f) == 0);
}

TEST_CASE("Math::Lerp", "[math]")
{
	REQUIRE(Simple::Lerp(0, 10, 0.5f) == 5);
	REQUIRE(Simple::Lerp(0, 10, 1.0f) == 10);
	REQUIRE(Simple::Lerp(0, 10, 0.0f) == 0);
	REQUIRE(Simple::Lerp(0, 10, 1.5f) == 15);
	
	REQUIRE(Simple::Lerp(0.f, 10.f, 0.5f) == 5.f);
	REQUIRE(Simple::Lerp(0.f, 10.f, 1.0f) == 10.f);
	REQUIRE(Simple::Lerp(0.f, 10.f, 0.0f) == 0.f);
	REQUIRE(Simple::Lerp(0.f, 10.f, 1.5f) == 15.f);

	REQUIRE(Simple::Lerp(-12.f, 24.f, 0.5f) == 6.f);
}

TEST_CASE("Math::LerpClamped", "[math]")
{
	constexpr int start = -12;
	constexpr int end = 26;
	REQUIRE(Simple::LerpClamped(start, end, 0.5f) == 7);
	REQUIRE(Simple::LerpClamped(start, end, 1.0f) == end);
	REQUIRE(Simple::LerpClamped(start, end, 0.0f) == start);
	REQUIRE(Simple::LerpClamped(start, end, -1.5f) == start);
	REQUIRE(Simple::LerpClamped(start, end, 1.5f) == end);

	REQUIRE(Simple::LerpClamped(0.f, 10.f, 0.5f) == 5.f);
	REQUIRE(Simple::LerpClamped(0.f, 10.f, 1.0f) == 10.f);
	REQUIRE(Simple::LerpClamped(0.f, 10.f, 0.0f) == 0.f);
	REQUIRE(Simple::LerpClamped(0.f, 10.f, -1.5f) == 0.f);
	REQUIRE(Simple::LerpClamped(0.f, 10.f, 1.5f) == 10.f);
}

TEST_CASE("Math::Remap", "[math]")
{
	REQUIRE(Simple::Remap(5, 0, 10, -12, 26) == 7);
	REQUIRE(Simple::Remap(0, 0, 10, -12, 26) == -12);
	REQUIRE(Simple::Remap(10, 0, 10, -12, 26) == 26);

	REQUIRE(Simple::Remap(5.f, 0.f, 10.f, -12.f, 26.f) == 7.f);
	REQUIRE(Simple::Remap(0.f, 0.f, 10.f, -12.f, 26.f) == -12.f);
	REQUIRE(Simple::Remap(10.f, 0.f, 10.f, -12.f, 26.f) == 26.f);
}

TEST_CASE("Floor")
{
	REQUIRE(Simple::Floor(2.5f) == 2);
	REQUIRE(Simple::Floor(-2.5f) == -3);
	REQUIRE(Simple::Floor(0.0f) == 0);
	REQUIRE(Simple::Floor(3.9999f) == 3);
	REQUIRE(Simple::Floor(-3.9999f) == -4);
	REQUIRE(Simple::Floor(2.f) == 2);
	REQUIRE(Simple::Floor(-2.f) == -2);
	REQUIRE(Simple::Floor(0.f) == 0);
}