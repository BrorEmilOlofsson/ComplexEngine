#include <External/Catch2/catch_amalgamated.hpp>

#include "Utility/Math/RotationMath.hpp"
#include "Utility/Math/Rotator.hpp"

using namespace Simple;

//TEST_CASE("Rotator Directions")
//{
//	{
//		constexpr Rotatorf rotator(Degreesf(0.f), Degreesf(0.f), Degreesf(0.f));
//
//		REQUIRE(GetRotatorRight(rotator) == UnitVector3f::Right());
//		REQUIRE(GetRotatorUp(rotator) == UnitVector3f::Up());
//		REQUIRE(GetRotatorForward(rotator) == UnitVector3f::Forward());
//	}
//
//	{
//		constexpr Rotatorf rotator(Degreesf(90.f), Degreesf(0.f), Degreesf(0.f));
//
//		REQUIRE(NearlyEqual(GetRotatorRight(rotator), UnitVector3f::Right()));
//		REQUIRE(NearlyEqual(GetRotatorUp(rotator), UnitVector3f::Forward()));
//		REQUIRE(NearlyEqual(GetRotatorForward(rotator), UnitVector3f::Down()));
//	}
//}

TEST_CASE("Create Rotator XY")
{

	{
		Matrix4x4f matrix = CreateMatrixFromXY(UnitVector3f::Backward(), UnitVector3f::Right());
		Rotatorf rot = ToRotator(matrix);

		REQUIRE(NearlyEqual(GetRotatorRight(rot), UnitVector3f::Backward()));
		REQUIRE(NearlyEqual(GetRotatorUp(rot), UnitVector3f::Right()));
		REQUIRE(NearlyEqual(GetRotatorForward(rot), UnitVector3f::Down()));
	}

	{
		Matrix4x4f matrix = CreateMatrixFromXY(UnitVector3f::Left(), UnitVector3f::Backward());
		Rotatorf rot = ToRotator(matrix);

		REQUIRE(NearlyEqual(GetRotatorRight(rot), UnitVector3f::Left()));
		REQUIRE(NearlyEqual(GetRotatorUp(rot), UnitVector3f::Backward()));
		REQUIRE(NearlyEqual(GetRotatorForward(rot), UnitVector3f::Down()));
	}

	{
		Matrix4x4f matrix = CreateMatrixFromXY(UnitVector3f::Forward(), UnitVector3f::Left());
		Rotatorf rot = ToRotator(matrix);

		REQUIRE(NearlyEqual(GetRotatorRight(rot), UnitVector3f::Forward()));
		REQUIRE(NearlyEqual(GetRotatorUp(rot), UnitVector3f::Left()));
		REQUIRE(NearlyEqual(GetRotatorForward(rot), UnitVector3f::Down()));
	}

	{
		Matrix4x4f matrix = CreateMatrixFromXY(UnitVector3f::Right(), UnitVector3f::Forward());
		Rotatorf rot = ToRotator(matrix);

		REQUIRE(NearlyEqual(GetRotatorRight(rot), UnitVector3f::Right()));
		REQUIRE(NearlyEqual(GetRotatorUp(rot), UnitVector3f::Forward()));
		REQUIRE(NearlyEqual(GetRotatorForward(rot), UnitVector3f::Down()));
	}

	{
		Matrix4x4f matrix = CreateMatrixFromXY(UnitVector3f::Left(), UnitVector3f::Forward());
		Rotatorf rot = ToRotator(matrix);

		REQUIRE(NearlyEqual(GetRotatorRight(rot), UnitVector3f::Left()));
		REQUIRE(NearlyEqual(GetRotatorUp(rot), UnitVector3f::Forward()));
		REQUIRE(NearlyEqual(GetRotatorForward(rot), UnitVector3f::Up()));
	}

	{
		Matrix4x4f matrix = CreateMatrixFromXY(UnitVector3f::Forward(), UnitVector3f::Right());
		Rotatorf rot = ToRotator(matrix);

		REQUIRE(NearlyEqual(GetRotatorRight(rot), UnitVector3f::Forward()));
		REQUIRE(NearlyEqual(GetRotatorUp(rot), UnitVector3f::Right()));
		REQUIRE(NearlyEqual(GetRotatorForward(rot), UnitVector3f::Up()));
	}

	{
		Matrix4x4f matrix = CreateMatrixFromXY(UnitVector3f::Right(), UnitVector3f::Backward());
		Rotatorf rot = ToRotator(matrix);

		REQUIRE(NearlyEqual(GetRotatorRight(rot), UnitVector3f::Right()));
		REQUIRE(NearlyEqual(GetRotatorUp(rot), UnitVector3f::Backward()));
		REQUIRE(NearlyEqual(GetRotatorForward(rot), UnitVector3f::Up()));
	}

	{
		Matrix4x4f matrix = CreateMatrixFromXY(UnitVector3f::Backward(), UnitVector3f::Left());
		Rotatorf rot = ToRotator(matrix);

		REQUIRE(NearlyEqual(GetRotatorRight(rot), UnitVector3f::Backward()));
		REQUIRE(NearlyEqual(GetRotatorUp(rot), UnitVector3f::Left()));
		REQUIRE(NearlyEqual(GetRotatorForward(rot), UnitVector3f::Up()));
	}

	{
		Matrix4x4f matrix = CreateMatrixFromXY(UnitVector3f::Up(), UnitVector3f::Right());
		Rotatorf rot = ToRotator(matrix);

		REQUIRE(NearlyEqual(GetRotatorRight(rot), UnitVector3f::Up()));
		REQUIRE(NearlyEqual(GetRotatorUp(rot), UnitVector3f::Right()));
		REQUIRE(NearlyEqual(GetRotatorForward(rot), UnitVector3f::Backward()));
	}

	{
		Matrix4x4f matrix = CreateMatrixFromXY(UnitVector3f::Right(), UnitVector3f::Down());
		Rotatorf rot = ToRotator(matrix);

		REQUIRE(NearlyEqual(GetRotatorRight(rot), UnitVector3f::Right()));
		REQUIRE(NearlyEqual(GetRotatorUp(rot), UnitVector3f::Down()));
		REQUIRE(NearlyEqual(GetRotatorForward(rot), UnitVector3f::Backward()));
	}

	{
		Matrix4x4f matrix = CreateMatrixFromXY(UnitVector3f::Down(), UnitVector3f::Left());
		Rotatorf rot = ToRotator(matrix);

		REQUIRE(NearlyEqual(GetRotatorRight(rot), UnitVector3f::Down()));
		REQUIRE(NearlyEqual(GetRotatorUp(rot), UnitVector3f::Left()));
		REQUIRE(NearlyEqual(GetRotatorForward(rot), UnitVector3f::Backward()));
	}

	{
		Matrix4x4f matrix = CreateMatrixFromXY(UnitVector3f::Left(), UnitVector3f::Up());
		Rotatorf rot = ToRotator(matrix);

		REQUIRE(NearlyEqual(GetRotatorRight(rot), UnitVector3f::Left()));
		REQUIRE(NearlyEqual(GetRotatorUp(rot), UnitVector3f::Up()));
		REQUIRE(NearlyEqual(GetRotatorForward(rot), UnitVector3f::Backward()));
	}
}

TEST_CASE("RotationMath::RotateVectorAroundAxis")
{
	{
		constexpr Vector3f vector(1, 0, 0);

		constexpr UnitVector3f axis = UnitVector3f::Up();

		const Vector3f v = RotateVectorAroundAxis(vector, axis, ToRadians(Degreesf(90)));

		REQUIRE(NearlyEqual(v, Vector3f(0, 0, -1)));
	}

	{
		constexpr Vector3f point(1, 0, 0);
		constexpr UnitVector3f axis = UnitVector3f::Up();
		const Vector3f v = RotateVectorAroundAxis(point, axis, ToRadians(Degreesf(180)));

		REQUIRE(NearlyEqual(v, Vector3f(-1, 0, 0)));
	}
}

TEST_CASE("RotationMath::RotateMatrixAroundAxis")
{
	{
		constexpr UnitVector3f up = UnitVector3f::Up();

		Matrix4x4f matrix = CreateMatrixFromXZ(UnitVector3f::Right(), UnitVector3f::Forward());

		RotateMatrixAroundAxis(matrix, up, Degreesf(90.f));

		REQUIRE(NearlyEqual(matrix.GetRight(), UnitVector3f::Forward()));
		REQUIRE(NearlyEqual(matrix.GetUp(), UnitVector3f::Up()));
		REQUIRE(NearlyEqual(matrix.GetForward(), UnitVector3f::Left()));
	}

	{
		constexpr UnitVector3f up = UnitVector3f::Up();

		const Matrix4x4f matrix = CreateMatrixFromXZ(UnitVector3f::Forward(), UnitVector3f::Left());

		const Matrix4x4f rotated = GetRotatedMatrixAroundAxis(matrix, up, Degreesf(45.f));

		REQUIRE(NearlyEqual(rotated.GetRight(), UnitVector3f(-1, 0, 1)));
		REQUIRE(NearlyEqual(rotated.GetUp(), UnitVector3f::Up()));
		REQUIRE(NearlyEqual(rotated.GetForward(), UnitVector3f(-1, 0, -1)));
	}

	{
		/*const UnitVector3f rotationAxis = GetNormalized(Vector3f(1, 0, 1));

		const Matrix4x4f matrix = MakeMatrixFromXZ(UnitVector3f::Right(), GetNormalized(Vector3f(0, -1, 1)));

		const Matrix4x4f rotated = RotateMatrixAroundAxis(matrix, rotationAxis, Degreesf(45.f));

		REQUIRE(NearlyEqual(GetNormalized(rotated.GetRight()), GetNormalized(Vector3f(-1, 0, 1))));
		REQUIRE(NearlyEqual(GetNormalized(rotated.GetUp()), UnitVector3f::Up()));
		REQUIRE(NearlyEqual(GetNormalized(rotated.GetForward()), GetNormalized(Vector3f(-1, 0, -1))));*/
	}
}