#include <External/Catch2/catch_amalgamated.hpp>

#include "Engine/Math/RotationMath.hpp"

using namespace Simple;


TEST_CASE("Rotation matrix")
{
	{
		const Matrix4x4d matrix = CreateRotationMatrix(Rotatord(Degreesd(0), Degreesd(0), Degreesd(0)));

		REQUIRE(matrix == Matrix4x4d::Identity());
	}

	{
		const Matrix4x4d matrix = CreateRotationMatrix(Rotatord(Degreesd(90), Degreesd(0), Degreesd(0)));

		REQUIRE(NearlyEqual(UnitVector3d(matrix.GetRight()), UnitVector3d::Right()));
		REQUIRE(NearlyEqual(UnitVector3d(matrix.GetUp()), UnitVector3d::Forward()));
		REQUIRE(NearlyEqual(UnitVector3d(matrix.GetForward()), UnitVector3d::Down()));
	}

	{
		const Matrix4x4d matrix = CreateRotationMatrix(Rotatord(Degreesd(90), Degreesd(90), Degreesd(0)));

		REQUIRE(NearlyEqual(UnitVector3d(matrix.GetRight()), UnitVector3d::Backward()));
		REQUIRE(NearlyEqual(UnitVector3d(matrix.GetUp()), UnitVector3d::Right()));
		REQUIRE(NearlyEqual(UnitVector3d(matrix.GetForward()), UnitVector3d::Down()));
	}
}

TEST_CASE("RotationMath::ToWorldSpace (Matrix4x4)", "[RotationMath]")
{
	{
		Matrix4x4f parentMatrix;
		parentMatrix.SetTranslation(Point3f(10, 10, 10));

		Matrix4x4f childMatrix = Matrix4x4f::CreateRotationMatrix(RotationMatrix3f::FromXY(UnitVector3f::Right(), UnitVector3f::Forward()));
		//Matrix4x4f childMatrix = CreateMatrixFromXY(UnitVector3f::Right(), UnitVector3f::Forward());
		childMatrix.SetTranslation(Point3f(0, 0, -10));

		RotateMatrixAroundAxis(parentMatrix, UnitVector3f::Up(), ToRadians(Degreesf(90.f)));


		REQUIRE(parentMatrix.GetTranslation() == Point3f(10, 10, 10));
		REQUIRE(NearlyEqual(parentMatrix.GetRight(), UnitVector3f::Forward()));

		const Matrix4x4f world = ToWorldSpace(childMatrix, parentMatrix);

		REQUIRE(world.GetTranslation() == Point3f(20, 10, 10));
		REQUIRE(NearlyEqual(world.GetUp(), UnitVector3f::Left()));
	}
}

TEST_CASE("ToLocalSpace")
{
	// No rotation
	{
		constexpr Matrix4x4f parentMatrixInWorldSpace = Matrix4x4f::CreateTranslationMatrix(Point3f(10, 10, 10));
		constexpr Matrix4x4f childMatrixInWorldSpace = Matrix4x4f::CreateTranslationMatrix(Point3f(15, -5, 15));

		constexpr Matrix4x4f local = ToLocalSpace(childMatrixInWorldSpace, parentMatrixInWorldSpace);
		constexpr Matrix4x4f world = ToWorldSpace(local, parentMatrixInWorldSpace);

		constexpr Matrix4x4f expectedLocal = Matrix4x4f::CreateTranslationMatrix(Point3f(5, -15, 5));
		REQUIRE(local == expectedLocal);
		REQUIRE(world == childMatrixInWorldSpace);
	}

	// Rotation
	{
		//constexpr Matrix4x4f parentMatrixInWorldSpace = CreateMatrixFromXY(UnitVector3f::Forward(), UnitVector3f::Up()) * Matrix4x4f::CreateTranslationMatrix(Point3f(10, 10, 10));
		constexpr Matrix4x4f parentMatrixInWorldSpace = Matrix4x4f::CreateTRMatrix(Point3f(10, 10, 10), RotationMatrix3f::FromXY(UnitVector3f::Forward(), UnitVector3f::Up()));
		constexpr Matrix4x4f childMatrixInWorldSpace = Matrix4x4f::CreateTranslationMatrix(Point3f(15, 15, 5));

		constexpr Matrix4x4f local = ToLocalSpace(childMatrixInWorldSpace, parentMatrixInWorldSpace);
		constexpr Matrix4x4f world = ToWorldSpace(local, parentMatrixInWorldSpace);

		constexpr Matrix4x4f expectedLocal
		{
			{
				0.f, 0.f, -1.f, 0.f,
				0.f, 1.f, 0.0f, 0.f,
				1.f, 0.f, 0.0f, 0.f,
				-5.f, 5.f, -5.f, 1.f
			}
		};

		REQUIRE(local == expectedLocal);
		REQUIRE(world == childMatrixInWorldSpace);
	}

	// Rotation
	{
		//constexpr Matrix4x4f parentMatrixInWorldSpace = CreateMatrixFromXY(UnitVector3f::Forward(), UnitVector3f::Up()) * Matrix4x4f::CreateTranslationMatrix(Point3f(10, 10, 10));
		constexpr Matrix4x4f parentMatrixInWorldSpace = Matrix4x4f::CreateTRMatrix(Point3f(10, 10, 10), RotationMatrix3f::FromXY(UnitVector3f::Forward(), UnitVector3f::Up()));
		//constexpr Matrix4x4f childMatrixInWorldSpace = CreateMatrixFromXY(UnitVector3f::Backward(), UnitVector3f::Down()) * Matrix4x4f::CreateTranslationMatrix(Point3f(15, 15, 5));
		constexpr Matrix4x4f childMatrixInWorldSpace = Matrix4x4f::CreateTRMatrix(Point3f(15, 15, 5), RotationMatrix3f::FromXY(UnitVector3f::Backward(), UnitVector3f::Down()));;

		constexpr Matrix4x4f local = ToLocalSpace(childMatrixInWorldSpace, parentMatrixInWorldSpace);
		constexpr Matrix4x4f world = ToWorldSpace(local, parentMatrixInWorldSpace);

		constexpr Matrix4x4f expectedLocal
		{
			{
				-1.f, 0.f, 0.f, 0.f,
				0.f, -1.f, 0.0f, 0.f,
				0.f, 0.f, 1.0f, 0.f,
				-5.f, 5.f, -5.f, 1.f
			}
		};

		REQUIRE(local == expectedLocal);
		REQUIRE(world == childMatrixInWorldSpace);
	}

	// Rotation
	{
		//Matrix4x4f parentMatrixInWorldSpace = CreateMatrixFromXY(UnitVector3f::Backward(), UnitVector3f::Left());
		Matrix4x4f parentMatrixInWorldSpace = Matrix4x4f::CreateRotationMatrix(RotationMatrix3f::FromXY(UnitVector3f::Backward(), UnitVector3f::Left()));
		parentMatrixInWorldSpace.SetTranslation(Point3f(10, 10, 10));

		Matrix4x4f childMatrixInWorldSpace;
		childMatrixInWorldSpace.SetTranslation(Point3f(15, 15, 5));


		const Matrix4x4f local = ToLocalSpace(childMatrixInWorldSpace, parentMatrixInWorldSpace);

		REQUIRE(local.GetTranslation() == Point3f(5, -5, 5));
	}
}

TEST_CASE("Transform vector local to world space")
{
	{
		constexpr UnitVector3f forward = UnitVector3f::Forward();

		const Matrix4x4f matrix = Matrix4x4f::CreateTRMatrix(Point3f(5, 5, 5), RotationMatrix3f::FromXY(UnitVector3f::Backward(), UnitVector3f::Left()));

		const UnitVector3f worldVector = ToWorldSpace(forward, matrix);

		REQUIRE(worldVector == UnitVector3f::Up());
	}
	{
		constexpr Vector3f forward(0, 0, 1);

		constexpr Matrix4x4f matrix = Matrix4x4f::CreateTRMatrix(Point3f(5, 5, 5), RotationMatrix3f::FromYZ(UnitVector3f::Backward(), UnitVector3f::Left()));

		const Vector3f worldVector = ToWorldSpace(forward, matrix);

		REQUIRE(worldVector == Vector3f(-1, 0, 0));
	}
}