#include "External/Catch2/catch_amalgamated.hpp"

#include "Utility/Math/Matrix4x4.hpp"
#include "Utility/Math/RotationMath.hpp"

using namespace Simple;

TEST_CASE("Identity Matrix")
{
	Matrix4x4f matrix = Matrix4x4f::Identity();
	REQUIRE(matrix(1, 1) == 1.f);
	REQUIRE(matrix(2, 2) == 1.f);
	REQUIRE(matrix(3, 3) == 1.f);
	REQUIRE(matrix(4, 4) == 1.f);
	REQUIRE(matrix(1, 2) == 0.f);
	REQUIRE(matrix(1, 3) == 0.f);
	REQUIRE(matrix(1, 4) == 0.f);
	REQUIRE(matrix(2, 1) == 0.f);
	REQUIRE(matrix(2, 3) == 0.f);
	REQUIRE(matrix(2, 4) == 0.f);
	REQUIRE(matrix(3, 1) == 0.f);
	REQUIRE(matrix(3, 2) == 0.f);
	REQUIRE(matrix(3, 4) == 0.f);
	REQUIRE(matrix(4, 1) == 0.f);
	REQUIRE(matrix(4, 2) == 0.f);
	REQUIRE(matrix(4, 3) == 0.f);
}

TEST_CASE("Matrix4x4 rotation axes unchanged")
{
	{ // Rotate on X axis
		Matrix4x4f matrix;

		const UnitVector3f right = matrix.GetRight();
		matrix = Matrix4x4f::CreateRotationAroundX(Degreesf(10.f)) * matrix;
		REQUIRE(right == matrix.GetRight());
	}

	{ // Rotate on Y axis
		Matrix4x4f matrix;

		const UnitVector3f up = matrix.GetUp();
		matrix = Matrix4x4f::CreateRotationAroundY(Degreesf(10.f)) * matrix;
		REQUIRE(up == matrix.GetUp());
	}

	{ // Rotate on Z axis
		Matrix4x4f matrix;

		const UnitVector3f forward = matrix.GetForward();
		matrix = Matrix4x4f::CreateRotationAroundZ(Degreesf(10.f)) * matrix;
		REQUIRE(forward == matrix.GetForward());
	}

	{
		Matrix4x4f matrix;

		matrix = Matrix4x4f::CreateRotationAroundY(Degreesf(72.f)) * matrix;
		matrix = Matrix4x4f::CreateRotationAroundZ(Degreesf(56.f)) * matrix;

		const UnitVector3f right = matrix.GetRight();

		matrix = Matrix4x4f::CreateRotationAroundX(Degreesf(10.f)) * matrix;

		REQUIRE(right == matrix.GetRight());
	}

	{
		Matrix4x4f matrix;

		matrix = Matrix4x4f::CreateRotationAroundX(Degreesf(72.f)) * matrix;
		matrix = Matrix4x4f::CreateRotationAroundZ(Degreesf(56.f)) * matrix;

		const UnitVector3f up = matrix.GetUp();

		matrix = Matrix4x4f::CreateRotationAroundY(Degreesf(10.f)) * matrix;

		REQUIRE(up == matrix.GetUp());
	}

	{
		Matrix4x4f matrix;

		matrix = Matrix4x4f::CreateRotationAroundY(Degreesf(72.f)) * matrix;
		matrix = Matrix4x4f::CreateRotationAroundX(Degreesf(56.f)) * matrix;

		const UnitVector3f forward = matrix.GetForward();

		matrix = Matrix4x4f::CreateRotationAroundZ(Degreesf(10.f)) * matrix;

		REQUIRE(forward == matrix.GetForward());
	}
}

TEST_CASE("Create Matrix XY")
{
	{
		constexpr UnitVector3f x = UnitVector3f::Right();
		constexpr UnitVector3f y = UnitVector3f::Up();

		const Matrix4x4f matrix = CreateMatrixFromXY(x, y);
		const UnitVector3f z = matrix.GetForward();
		REQUIRE(NearlyEqual(z, UnitVector3f::Forward()));
	}
}

TEST_CASE("Create Matrix YZ")
{
	{
		constexpr UnitVector3f y = UnitVector3f::Up();
		constexpr UnitVector3f z = UnitVector3f::Forward();

		const Matrix4x4f matrix = CreateMatrixFromYZ(y, z);
		const UnitVector3f x = matrix.GetRight();
		REQUIRE(NearlyEqual(x, UnitVector3f::Right()));
	}
}

TEST_CASE("Create Matrix XZ")
{
	{
		constexpr UnitVector3f x = UnitVector3f::Right();
		constexpr UnitVector3f z = UnitVector3f::Forward();

		const Matrix4x4f matrix = CreateMatrixFromXZ(x, z);
		const UnitVector3f y = matrix.GetUp();
		REQUIRE(NearlyEqual(y, UnitVector3f::Up()));
	}
}


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

TEST_CASE("Transform hierarchy")
{
	{
		Matrix4x4f parentMatrix;
		parentMatrix.SetTranslation(Point3f(10, 10, 10));

		Matrix4x4f childMatrix = CreateMatrixFromXY(UnitVector3f::Right(), UnitVector3f::Forward());
		childMatrix.SetTranslation(Point3f(0, 0, -10));

		RotateMatrixAroundAxis(parentMatrix, UnitVector3f::Up(), ToRadians(Degreesf(90.f)));
		

		REQUIRE(parentMatrix.GetTranslation() == Point3f(10, 10, 10));
		REQUIRE(NearlyEqual(parentMatrix.GetRight(), UnitVector3f::Forward()));

		const Matrix4x4f world = ToWorldSpace(childMatrix, parentMatrix);

		REQUIRE(world.GetTranslation() == Point3f(20, 10, 10));
		REQUIRE(NearlyEqual(world.GetUp(), UnitVector3f::Left()));
	}
}

TEST_CASE("Transform world to local space")
{
	// No rotation
	{ 
		Matrix4x4f parentMatrix;
		parentMatrix.SetTranslation(Point3f(10, 10, 10));

		Matrix4x4f childMatrix;
		childMatrix.SetTranslation(Point3f(15, -5, 15));


		const Matrix4x4f local = ToLocalSpace(childMatrix, parentMatrix);
		const Matrix4x4f world = ToWorldSpace(local, parentMatrix);
		REQUIRE(local.GetTranslation() == Point3f(5, -15, 5));
		REQUIRE(world == childMatrix);
	}

	// Rotation
	{
		Matrix4x4f parentMatrixInWorldSpace = CreateMatrixFromXY(UnitVector3f::Forward(), UnitVector3f::Up());
		parentMatrixInWorldSpace.SetTranslation(Point3f(10, 10, 10));

		Matrix4x4f childMatrixInWorldSpace;
		childMatrixInWorldSpace.SetTranslation(Point3f(15, 15, 5));


		const Matrix4x4f local = ToLocalSpace(childMatrixInWorldSpace, parentMatrixInWorldSpace);

		REQUIRE(local.GetTranslation() == Point3f(-5, 5, -5));
	}

	// Rotation
	{
		Matrix4x4f parentMatrixInWorldSpace = CreateMatrixFromXY(UnitVector3f::Backward(), UnitVector3f::Left());
		parentMatrixInWorldSpace.SetTranslation(Point3f(10, 10, 10));

		Matrix4x4f childMatrixInWorldSpace;
		childMatrixInWorldSpace.SetTranslation(Point3f(15, 15, 5));


		const Matrix4x4f local = ToLocalSpace(childMatrixInWorldSpace, parentMatrixInWorldSpace);

		REQUIRE(local.GetTranslation() == Point3f(5, -5, 5));
	}
}

TEST_CASE("Transform vector local to world space")
{
	constexpr Vector3f forward(0, 0, 1);

	Matrix4x4f matrix = CreateMatrixFromYZ(UnitVector3f::Backward(), UnitVector3f::Left());
	matrix.SetTranslation(Point3f(5, 5, 5));

	const Vector3f worldVector = ToWorldSpace(forward, matrix);

	REQUIRE(worldVector == Vector3f(-1, 0, 0));
}

TEST_CASE("GetFastInverse")
{

	Matrix4x4f world = CreateMatrixFromXY(UnitVector3f::Forward(), UnitVector3f::Up());
	world.SetTranslation(Point3f(10.f, 5.f, -3.f));

	Matrix4x4f view = Matrix4x4f::GetFastInverse(world);
	Matrix4x4f test = view * world;

	REQUIRE(test == Matrix4x4f::Identity());
}