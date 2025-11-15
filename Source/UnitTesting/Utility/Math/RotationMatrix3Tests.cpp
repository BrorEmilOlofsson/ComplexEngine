#include <External/Catch2/catch_amalgamated.hpp>
#include "Utility/Math/RotationMatrix3.hpp"

using namespace Simple;

TEST_CASE("RotationMatrix3 - Default constructor is identity")
{
	RotationMatrix3f rm;
	REQUIRE(rm == RotationMatrix3f::Identity());
}

TEST_CASE("RotationMatrix3 - Identity function is correct")
{
	RotationMatrix3f rm = RotationMatrix3f::Identity();
	REQUIRE(rm == RotationMatrix3f::Identity());
}

TEST_CASE("RotationMatrix3 - Constructor with values is correct")
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

TEST_CASE("RotationMatrix3 - FromAxes is correct")
{
	const RotationMatrix3f rm = RotationMatrix3f::FromAxes
	(
		UnitVector3f::Right(),
		UnitVector3f::Up(),
		UnitVector3f::Forward()
	);
	REQUIRE(rm == RotationMatrix3f::Identity());
}

TEST_CASE("RotationMatrix3 - Get axes")
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

TEST_CASE("RotationMatrix3 - Throws if not pure rotation")
{
	{
		const RotationMatrix3f rm = RotationMatrix3f::FromAxes
		(
			UnitVector3f::Right(),
			UnitVector3f::Up(),
			UnitVector3f::Forward()
		);
	}
	{
		REQUIRE_THROWS_AS(RotationMatrix3f::FromAxes
		(
			UnitVector3f::Right(),
			UnitVector3f::Up(),
			-UnitVector3f::Forward() // Inverted axis
		), std::invalid_argument);
	}
}