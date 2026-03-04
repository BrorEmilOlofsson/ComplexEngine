#include <External/Catch2/catch_amalgamated.hpp>
#include "Engine/Math/RotationMatrix3.hpp"

using namespace Simple;

TEST_CASE("RotationMatrix3::Default constructor", "[RotationMatrix3]")
{
	RotationMatrix3f rm;
	REQUIRE(rm == RotationMatrix3f::Identity());
}

TEST_CASE("RotationMatrix3::Identity", "[RotationMatrix3]")
{
	RotationMatrix3f rm = RotationMatrix3f::Identity();
	REQUIRE(rm == RotationMatrix3f::Identity());
}

TEST_CASE("RotationMatrix3 - Constructor with values is correct", "[RotationMatrix3]")
{
	const RotationMatrix3f rm
	(
		{
			1.f, 0.f, 0.f,
			0.f, 1.f, 0.f,
			0.f, 0.f, 1.f
		}
	);
	REQUIRE(rm == RotationMatrix3f::Identity());
}

TEST_CASE("RotationMatrix3 - FromAxes is correct", "[RotationMatrix3]")
{
	const RotationMatrix3f rm = RotationMatrix3f::FromAxes
	(
		UnitVector3f::Right(),
		UnitVector3f::Up(),
		UnitVector3f::Forward()
	);
	REQUIRE(rm == RotationMatrix3f::Identity());
}

TEST_CASE("RotationMatrix3 - Get axes", "[RotationMatrix3]")
{
	const RotationMatrix3f rm = RotationMatrix3f::FromAxes
	(
		UnitVector3f::Right(),
		UnitVector3f::Up(),
		UnitVector3f::Forward()
	);
	REQUIRE(rm.GetRight() == UnitVector3f::Right());
	REQUIRE(rm.GetUp() == UnitVector3f::Up());
	REQUIRE(rm.GetForward() == UnitVector3f::Forward());
}

TEST_CASE("RotationMatrix3 - Throws if not pure rotation", "[RotationMatrix3]")
{
	{
		constexpr RotationMatrix3f rm = RotationMatrix3f::FromAxes
		(
			UnitVector3f::Right(),
			UnitVector3f::Up(),
			UnitVector3f::Forward()
		);
	}
}