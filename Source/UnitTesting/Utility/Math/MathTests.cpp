#include <External/Catch2/catch_amalgamated.hpp>

#include "Utility/Math/Math.hpp"

using namespace Simple;

TEST_CASE("Math::Square", "[Math]")
{
	REQUIRE(Square(2) == 4);
	REQUIRE(Square(-2) == 4);
	REQUIRE(Square(0) == 0);

	REQUIRE(Square(2.f) == 4.f);
	REQUIRE(Square(-2.f) == 4.f);
	REQUIRE(Square(0.f) == 0);
}

TEST_CASE("Math::Abs", "[Math]")
{
	REQUIRE(Abs(2) == 2);
	REQUIRE(Abs(-2) == 2);
	REQUIRE(Abs(0) == 0);

	REQUIRE(Abs(2.f) == 2);
	REQUIRE(Abs(-2.f) == 2);
	REQUIRE(Abs(0.f) == 0);
}

TEST_CASE("Math::Sign", "[Math]")
{
	REQUIRE(Sign(2) == 1);
	REQUIRE(Sign(-2) == -1);
	REQUIRE(Sign(0) == 0);

	REQUIRE(Sign(2.f) == 1);
	REQUIRE(Sign(-2.f) == -1);
	REQUIRE(Sign(0.0f) == 0);
}

TEST_CASE("Math::Lerp", "[Math]")
{
	REQUIRE(Lerp(0, 10, 0.5f) == 5);
	REQUIRE(Lerp(0, 10, 1.0f) == 10);
	REQUIRE(Lerp(0, 10, 0.0f) == 0);
	REQUIRE(Lerp(0, 10, 1.5f) == 15);
	
	REQUIRE(Lerp(0.f, 10.f, 0.5f) == 5.f);
	REQUIRE(Lerp(0.f, 10.f, 1.0f) == 10.f);
	REQUIRE(Lerp(0.f, 10.f, 0.0f) == 0.f);
	REQUIRE(Lerp(0.f, 10.f, 1.5f) == 15.f);

	REQUIRE(Lerp(-12.f, 24.f, 0.5f) == 6.f);
}

TEST_CASE("Math::LerpClamped", "[Math]")
{
	constexpr int start = -12;
	constexpr int end = 26;
	REQUIRE(LerpClamped(start, end, 0.5f) == 7);
	REQUIRE(LerpClamped(start, end, 1.0f) == end);
	REQUIRE(LerpClamped(start, end, 0.0f) == start);
	REQUIRE(LerpClamped(start, end, -1.5f) == start);
	REQUIRE(LerpClamped(start, end, 1.5f) == end);

	REQUIRE(LerpClamped(0.f, 10.f, 0.5f) == 5.f);
	REQUIRE(LerpClamped(0.f, 10.f, 1.0f) == 10.f);
	REQUIRE(LerpClamped(0.f, 10.f, 0.0f) == 0.f);
	REQUIRE(LerpClamped(0.f, 10.f, -1.5f) == 0.f);
	REQUIRE(LerpClamped(0.f, 10.f, 1.5f) == 10.f);
}

TEST_CASE("Math::Remap", "[Math]")
{
	REQUIRE(Remap(5, 0, 10, -12, 26) == 7);
	REQUIRE(Remap(0, 0, 10, -12, 26) == -12);
	REQUIRE(Remap(10, 0, 10, -12, 26) == 26);

	REQUIRE(Remap(5.f, 0.f, 10.f, -12.f, 26.f) == 7.f);
	REQUIRE(Remap(0.f, 0.f, 10.f, -12.f, 26.f) == -12.f);
	REQUIRE(Remap(10.f, 0.f, 10.f, -12.f, 26.f) == 26.f);
}

TEST_CASE("Floor")
{
	REQUIRE(Floor(2.5f) == 2);
	REQUIRE(Floor(-2.5f) == -3);
	REQUIRE(Floor(0.0f) == 0);
	REQUIRE(Floor(3.9999f) == 3);
	REQUIRE(Floor(-3.9999f) == -4);
	REQUIRE(Floor(2.f) == 2);
	REQUIRE(Floor(-2.f) == -2);
	REQUIRE(Floor(0.f) == 0);
}