#include "External/Catch2/catch_amalgamated.hpp"

#include "Engine/Math/Matrix4x4.hpp"
#include "Engine/Math/RotationMath.hpp"

using namespace CLX;

TEST_CASE("Identity Matrix", "[Matrix4x4]")
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


TEST_CASE("Matrix4x4::Default Constructor", "[Matrix4x4]")
{
	Matrix4x4f matrix;
	REQUIRE(matrix == Matrix4x4f::Identity());
}

TEST_CASE("Matrix4x4::Zero", "[Matrix4x4]")
{
	const Matrix4x4f matrix = Matrix4x4f::Zero();
	for (unsigned int pos = 0; pos < 16; ++pos)
	{
		REQUIRE(matrix[pos] == 0.f);
	}
}

TEST_CASE("Matrix4x4 constructor with values", "[Matrix4x4]")
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

TEST_CASE("Matrix4x4 indexing", "[Matrix4x4]")
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

TEST_CASE("Matrix4x4 multiplication", "[Matrix4x4]")
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

TEST_CASE("Matrix4x4 Multiplication with Vector", "[Matrix4x4]")
{
	{
		// Identity matrix
		constexpr Matrix4x4f matrix = Matrix4x4f::Identity();

		constexpr Vector3f vector(1.f, 2.f, 3.f);

		constexpr Vector3f result = matrix * vector;
		REQUIRE(result == vector);
	}

	{
		constexpr Matrix4x4f matrix = Matrix4x4f::CreateRotationMatrix(RotationMatrix3f::FromXZ(UnitVector3f::Forward(), UnitVector3f::Left()));

		constexpr Vector3f vector(1.f, 0.f, 0.f);

		constexpr Vector3f result1 = matrix * vector;
		constexpr Vector3f result2 = vector * matrix;

		REQUIRE(result1 == Vector3(0.f, 0.f, 1.f));
		REQUIRE(result2 == Vector3(0.f, 0.f, 1.f));
	}
}


TEST_CASE("Matrix4x4 Equals", "[Matrix4x4]")
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

TEST_CASE("Matrix4x4 SetTranslation", "[Matrix4x4]")
{
	Matrix4x4f matrix;
	const Point3f translation(1.f, 2.f, 3.f);
	matrix.SetTranslation(translation);
	REQUIRE(matrix.GetTranslation() == translation);
	REQUIRE(matrix[12] == 1.f);
	REQUIRE(matrix[13] == 2.f);
	REQUIRE(matrix[14] == 3.f);
}

TEST_CASE("Matrix4x4 SetScale", "[Matrix4x4]")
{
	Matrix4x4f matrix;
	const Vector3f scale(2.f, 3.f, 4.f);
	matrix.SetScale(scale);
	REQUIRE(matrix.GetScale() == scale);
	REQUIRE(matrix[0] == 2.f);
	REQUIRE(matrix[5] == 3.f);
	REQUIRE(matrix[10] == 4.f);
}

TEST_CASE("Matrix4x4 GetTranslation", "[Matrix4x4]")
{
	const Matrix4x4f matrix
	(
		{
			1.f, 0.f, 0.f, 0.f,
			0.f, 1.f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			7.f, 8.f, 9.f, 1.f
		}
	);
	const Point3f translation = matrix.GetTranslation();
	REQUIRE(translation == Point3f(7.f, 8.f, 9.f));
}

TEST_CASE("Matrix4x4 GetScale", "[Matrix4x4]")
{
	const Matrix4x4f matrix
	(
		{
			2.f, 0.f, 0.f, 0.f,
			0.f, 3.f, 0.f, 0.f,
			0.f, 0.f, 4.f, 0.f,
			0.f, 0.f, 0.f, 1.f
		}
	);
	const Vector3f scale = matrix.GetScale();
	REQUIRE(scale == Vector3f(2.f, 3.f, 4.f));
}

TEST_CASE("Matrix4x4 Get Scaled Vectors", "[Matrix4x4]")
{
	const Matrix4x4f matrix
	(
		{
			7.f, 0.f, 0.f, 0.f,
			0.f, 0.f, -5.f, 0.f,
			0.f, 2.f, 0.f, 0.f,
			0.f, 0.f, 0.f, 1.f
		}
	);

	Vector3f right = matrix.GetRightScaled();
	Vector3f up = matrix.GetUpScaled();
	Vector3f forward = matrix.GetForwardScaled();

	REQUIRE(right == Vector3f(7.f, 0.f, 0.f));
	REQUIRE(up == Vector3f(0.f, 0.f, -5.f));
	REQUIRE(forward == Vector3f(0.f, 2.f, 0.f));
}

TEST_CASE("Matrix4x4 Get Unscaled Vectors")
{
	const Matrix4x4f matrix
	(
		{
			0.f, 0.f, -3.f, 0.f,
			0.f, 4.f, 0.f, 0.f,
			5.f, 0.f, 0.f, 0.f,
			0.f, 0.f, 0.f, 1.f
		}
	);
	UnitVector3f right = matrix.GetRight();
	UnitVector3f up = matrix.GetUp();
	UnitVector3f forward = matrix.GetForward();
	REQUIRE(right == UnitVector3f(0.f, 0.f, -1.f));
	REQUIRE(up == UnitVector3f(0.f, 1.f, 0.f));
	REQUIRE(forward == UnitVector3f(1.f, 0.f, 0.f));
}

TEST_CASE("Matrix4x4 ToTransposed", "[Matrix4x4]")
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
	const Matrix4x4f transposedMatrix = Matrix4x4f::ToTransposed(matrix);

	REQUIRE(matrix[0] == transposedMatrix[0]);
	REQUIRE(matrix[1] == transposedMatrix[4]);
	REQUIRE(matrix[2] == transposedMatrix[8]);
	REQUIRE(matrix[3] == transposedMatrix[12]);
	REQUIRE(matrix[4] == transposedMatrix[1]);
	REQUIRE(matrix[5] == transposedMatrix[5]);
	REQUIRE(matrix[6] == transposedMatrix[9]);
	REQUIRE(matrix[7] == transposedMatrix[13]);
	REQUIRE(matrix[8] == transposedMatrix[2]);
	REQUIRE(matrix[9] == transposedMatrix[6]);
	REQUIRE(matrix[10] == transposedMatrix[10]);
	REQUIRE(matrix[11] == transposedMatrix[14]);
	REQUIRE(matrix[12] == transposedMatrix[3]);
	REQUIRE(matrix[13] == transposedMatrix[7]);
	REQUIRE(matrix[14] == transposedMatrix[11]);
	REQUIRE(matrix[15] == transposedMatrix[15]);
}

TEST_CASE("Matrix4x4 CreateTranslationMatrix", "[Matrix4x4]")
{
	constexpr Point3f translation(3.f, 4.f, 5.f);
	constexpr Matrix4x4f translationMatrix = Matrix4x4f::CreateTranslationMatrix(translation);
	constexpr Matrix4x4f expectedMatrix
	(
		{
			1.f, 0.f, 0.f, 0.f,
			0.f, 1.f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			3.f, 4.f, 5.f, 1.f
		}
	);
	REQUIRE(translationMatrix == expectedMatrix);
}

TEST_CASE("Matrix4x4 CreateScaleMatrix", "[Matrix4x4]")
{
	constexpr Vector3f scale(2.f, 3.f, 4.f);
	constexpr Matrix4x4f scaleMatrix = Matrix4x4f::CreateScaleMatrix(scale);
	constexpr Matrix4x4f expectedMatrix
	(
		{
			2.f, 0.f, 0.f, 0.f,
			0.f, 3.f, 0.f, 0.f,
			0.f, 0.f, 4.f, 0.f,
			0.f, 0.f, 0.f, 1.f
		}
	);
	REQUIRE(scaleMatrix == expectedMatrix);
}

TEST_CASE("Matrix4x4 CreateRotationMatrix", "[Matrix4x4]")
{

	constexpr RotationMatrix3f rotationMatrix
	(
		{
			0, 1, 0,
			0, 0, 1,
			1, 0, 0
		}
	);
	constexpr Matrix4x4f m = Matrix4x4f::CreateRotationMatrix(rotationMatrix);

	constexpr Matrix4x4f expectedMatrix
	(
		{
			0, 1, 0, 0,
			0, 0, 1, 0,
			1, 0, 0, 0,
			0, 0, 0, 1
		}
	);
	REQUIRE(m == expectedMatrix);
}

TEST_CASE("Matrix4x4 CreateTRSMatrix", "[Matrix4x4]")
{
	constexpr Point3f translation = Point3f(10, 20, 30);
	const Matrix4x4f rotationMatrix = Matrix4x4f::CreateRotationAroundZ(Degreesf(45.f));
	constexpr Vector3f scale = Vector3f(2, 3, 4);
	// rs is correct
	const Matrix4x4f trs = Matrix4x4f::CreateTRSMatrix(translation, rotationMatrix, scale);

	const Matrix4x4f expectedMatrix
	(
		{
			1.4142135f,  1.4142135f, 0.f, 0.f,
			-2.1213202f,  2.1213202f, 0.f, 0.f,
			0.f,         0.f,        4.f, 0.f,
			10.f,        20.f,       30.f, 1.f
		}
	);

	REQUIRE(NearlyEqual(trs, expectedMatrix));
}


TEST_CASE("GetFastInverse", "[Matrix4x4]")
{

	Matrix4x4f world = Matrix4x4f::CreateRotationMatrix(RotationMatrix3f::FromXY(UnitVector3f::Forward(), UnitVector3f::Up()));
	world.SetTranslation(Point3f(10.f, 5.f, -3.f));

	Matrix4x4f view = Matrix4x4f::GetFastInverse(world);
	Matrix4x4f test = view * world;

	REQUIRE(test == Matrix4x4f::Identity());
}

TEST_CASE("Matrix4x4 rotation axes unchanged", "[Matrix4x4]")
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