#include "External/Catch2/catch_amalgamated.hpp"

#include "Utility/Math/Matrix4x4.hpp"
#include "Utility/Math/RotationMath.hpp"

using namespace Simple;

TEST_CASE("Identity Matrix")
{
	Matrix4x4f matrix = Matrix4x4f::Identity();
	REQUIRE(matrix(0, 0) == 1.f);
	REQUIRE(matrix(1, 1) == 1.f);
	REQUIRE(matrix(2, 2) == 1.f);
	REQUIRE(matrix(3, 3) == 1.f);
	REQUIRE(matrix(0, 1) == 0.f);
	REQUIRE(matrix(0, 2) == 0.f);
	REQUIRE(matrix(0, 3) == 0.f);
	REQUIRE(matrix(1, 0) == 0.f);
	REQUIRE(matrix(1, 2) == 0.f);
	REQUIRE(matrix(1, 3) == 0.f);
	REQUIRE(matrix(2, 0) == 0.f);
	REQUIRE(matrix(2, 1) == 0.f);
	REQUIRE(matrix(2, 3) == 0.f);
	REQUIRE(matrix(3, 0) == 0.f);
	REQUIRE(matrix(3, 1) == 0.f);
	REQUIRE(matrix(3, 2) == 0.f);
}


TEST_CASE("Matrix default constructor is identity")
{
	Matrix4x4f matrix;
	REQUIRE(matrix == Matrix4x4f::Identity());
}

TEST_CASE("Matrix4x4 Zero")
{
	const Matrix4x4f matrix = Matrix4x4f::Zero();
	for (unsigned int pos = 0; pos < 16; ++pos)
	{
		REQUIRE(matrix[pos] == 0.f);
	}
}

TEST_CASE("Matrix4x4 constructor with values")
{
	const Matrix4x4f matrix
	(
		{
			1.f, 2.f, 3.f, 4.f,
			5.f, 6.f, 7.f, 8.f,
			9.f, 10.f, 11.f, 12.f,
			13.f, 14.f, 15.f, 16.f
		}
	);
	REQUIRE(matrix[0] == 1.f);
	REQUIRE(matrix[1] == 2.f);
	REQUIRE(matrix[2] == 3.f);
	REQUIRE(matrix[3] == 4.f);
	REQUIRE(matrix[4] == 5.f);
	REQUIRE(matrix[5] == 6.f);
	REQUIRE(matrix[6] == 7.f);
	REQUIRE(matrix[7] == 8.f);
	REQUIRE(matrix[8] == 9.f);
	REQUIRE(matrix[9] == 10.f);
	REQUIRE(matrix[10] == 11.f);
	REQUIRE(matrix[11] == 12.f);
	REQUIRE(matrix[12] == 13.f);
	REQUIRE(matrix[13] == 14.f);
	REQUIRE(matrix[14] == 15.f);
	REQUIRE(matrix[15] == 16.f);
}

TEST_CASE("Matrix4x4 indexing")
{
	const Matrix4x4f matrix
	(
		{
			16.f, 15.f, 14.f, 13.f,
			12.f, 11.f, 10.f, 9.f,
			8.f, 7.f, 6.f, 5.f,
			4.f, 3.f, 2.f, 1.f
		}
	);

	REQUIRE(matrix[0] == 16.f);
	REQUIRE(matrix[1] == 15.f);
	REQUIRE(matrix[2] == 14.f);
	REQUIRE(matrix[3] == 13.f);
	REQUIRE(matrix[4] == 12.f);
	REQUIRE(matrix[5] == 11.f);
	REQUIRE(matrix[6] == 10.f);
	REQUIRE(matrix[7] == 9.f);
	REQUIRE(matrix[8] == 8.f);
	REQUIRE(matrix[9] == 7.f);
	REQUIRE(matrix[10] == 6.f);
	REQUIRE(matrix[11] == 5.f);
	REQUIRE(matrix[12] == 4.f);
	REQUIRE(matrix[13] == 3.f);
	REQUIRE(matrix[14] == 2.f);
	REQUIRE(matrix[15] == 1.f);

	REQUIRE(matrix(0, 0) == 16.f);
	REQUIRE(matrix(0, 1) == 15.f);
	REQUIRE(matrix(0, 2) == 14.f);
	REQUIRE(matrix(0, 3) == 13.f);
	REQUIRE(matrix(1, 0) == 12.f);
	REQUIRE(matrix(1, 1) == 11.f);
	REQUIRE(matrix(1, 2) == 10.f);
	REQUIRE(matrix(1, 3) == 9.f);
	REQUIRE(matrix(2, 0) == 8.f);
	REQUIRE(matrix(2, 1) == 7.f);
	REQUIRE(matrix(2, 2) == 6.f);
	REQUIRE(matrix(2, 3) == 5.f);
	REQUIRE(matrix(3, 0) == 4.f);
	REQUIRE(matrix(3, 1) == 3.f);
	REQUIRE(matrix(3, 2) == 2.f);
	REQUIRE(matrix(3, 3) == 1.f);
}

TEST_CASE("Matrix4x4 multiplication")
{
	constexpr Matrix4x4f matrixA
	(
		{
			1.f, 2.f, 3.f, 4.f,
			5.f, 6.f, 7.f, 8.f,
			9.f, 10.f, 11.f, 12.f,
			13.f, 14.f, 15.f, 16.f
		}
	);

	constexpr Matrix4x4f matrixB
	(
		{
			16.f, 15.f, 14.f, 13.f,
			12.f, 11.f, 10.f, 9.f,
			8.f, 7.f, 6.f, 5.f,
			4.f, 3.f, 2.f, 1.f
		}
	);

	constexpr Matrix4x4f result = matrixA * matrixB;

	REQUIRE(result[0] == 80.f);
	REQUIRE(result[1] == 70.f);
	REQUIRE(result[2] == 60.f);
	REQUIRE(result[3] == 50.f);
	REQUIRE(result[4] == 240.f);
	REQUIRE(result[5] == 214.f);
	REQUIRE(result[6] == 188.f);
	REQUIRE(result[7] == 162.f);
	REQUIRE(result[8] == 400.f);
	REQUIRE(result[9] == 358.f);
	REQUIRE(result[10] == 316.f);
	REQUIRE(result[11] == 274.f);
	REQUIRE(result[12] == 560.f);
	REQUIRE(result[13] == 502.f);
	REQUIRE(result[14] == 444.f);
	REQUIRE(result[15] == 386.f);
}

TEST_CASE("Matrix4x4 Multiplication with Vector")
{
	{
		// Identity matrix
		constexpr Matrix4x4f matrix = Matrix4x4f::Identity();

		constexpr Vector3f vector(1.f, 2.f, 3.f);

		constexpr Vector3f result = matrix * vector;
		REQUIRE(result == vector);
	}

	{
		constexpr Matrix4x4f matrix = CreateMatrixFromXZ(UnitVector3f::Forward(), UnitVector3f::Left());

		constexpr Vector3f vector(1.f, 0.f, 0.f);

		constexpr Vector3f result1 = matrix * vector;
		constexpr Vector3f result2 = vector * matrix;

		REQUIRE(result1 == Vector3(0.f, 0.f, 1.f));
		REQUIRE(result2 == Vector3(0.f, 0.f, 1.f));
	}
}


TEST_CASE("Matrix4x4 Equals")
{
	const Matrix4x4f matrixA
	(
		{
			1.f, 2.f, 3.f, 4.f,
			5.f, 6.f, 7.f, 8.f,
			9.f, 10.f, 11.f, 12.f,
			13.f, 14.f, 15.f, 16.f
		}
	);

	const Matrix4x4f matrixB
	(
		{
			1.f, 2.f, 3.f, 4.f,
			5.f, 6.f, 7.f, 8.f,
			9.f, 10.f, 11.f, 12.f,
			13.f, 14.f, 15.f, 16.f
		}
	);

	const Matrix4x4f matrixC
	(
		{
			1.f, 2.f, 12.f, 4.f,
			5.f, 6.f, 7.f, 8.f,
			9.f, 10.f, 11.f, 12.f,
			13.f, 14.f, 15.f, 16.f
		}
	);

	REQUIRE(matrixA == matrixB);
	REQUIRE(matrixA != matrixC);
}

TEST_CASE("Matrix4x4 SetTranslation")
{
	Matrix4x4f matrix;
	const Point3f translation(1.f, 2.f, 3.f);
	matrix.SetTranslation(translation);
	REQUIRE(matrix.GetTranslation() == translation);
	REQUIRE(matrix[12] == 1.f);
	REQUIRE(matrix[13] == 2.f);
	REQUIRE(matrix[14] == 3.f);
}

TEST_CASE("Matrix4x4 SetScale")
{
	Matrix4x4f matrix;
	const Vector3f scale(2.f, 3.f, 4.f);
	matrix.SetScale(scale);
	REQUIRE(matrix.GetScale() == scale);
	REQUIRE(matrix[0] == 2.f);
	REQUIRE(matrix[5] == 3.f);
	REQUIRE(matrix[10] == 4.f);
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