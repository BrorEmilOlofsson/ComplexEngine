#pragma once
#include "Utility/Math/Vector3.hpp"
#include "Utility/Math/Vector4.hpp"
#include "Utility/Math/UnitVector3.hpp"
#include "Utility/Math/Point3.hpp"
#include "Utility/Math/Math.hpp"
#include "Utility/Math/VectorMath.hpp"
#include "Utility/Math/Angle.hpp"
#include "Utility/Math/Matrix3x3.hpp"
#include "Utility/Math/RotationMatrix3.hpp"
#include "Utility/Math/Quaternion.hpp"
#include <string>
#include <cassert>
#include <iomanip>

namespace Simple
{

	template<typename T>
	class alignas(16) Matrix4x4 final
	{
		static constexpr unsigned int Dimensions = 4;
		static constexpr unsigned int ValueCount = Dimensions * Dimensions;
	public:

		constexpr Matrix4x4();
		constexpr Matrix4x4(const std::array<T, ValueCount>& values) noexcept;

		[[nodiscard]] constexpr T& operator()(const unsigned int row, const unsigned int column);
		[[nodiscard]] constexpr const T& operator()(const unsigned int row, const unsigned int column) const;
		[[nodiscard]] constexpr T& operator[](const unsigned int index) noexcept;
		[[nodiscard]] constexpr const T& operator[](const unsigned int index) const noexcept;

		constexpr void SetTranslation(const Point3<T>& translation);
		constexpr void SetScale(const Vector3<T>& scale);
		constexpr void SetRight(const UnitVector3<T>& right);
		constexpr void SetUp(const UnitVector3<T>& up);
		constexpr void SetForward(const UnitVector3<T>& forward);
		constexpr void SetRight(const Vector3<T>& right);
		constexpr void SetUp(const Vector3<T>& up);
		constexpr void SetForward(const Vector3<T>& forward);
		constexpr void SetRotationMatrix(const Matrix3x3<T>& rotationMatrix);
		constexpr void SetRotationMatrix(const RotationMatrix3<T>& rotationMatrix);

		[[nodiscard]] constexpr const T* GetDataPtr() const;
		[[nodiscard]] constexpr T* GetDataPtr();

		[[nodiscard]] constexpr Point3<T> GetTranslation() const;
		[[nodiscard]] constexpr Vector3<T> GetScale() const;
		[[nodiscard]] constexpr UnitVector3<T> GetRight() const;
		[[nodiscard]] constexpr UnitVector3<T> GetUp() const;
		[[nodiscard]] constexpr UnitVector3<T> GetForward() const;
		[[nodiscard]] constexpr Vector3<T> GetRightScaled() const;
		[[nodiscard]] constexpr Vector3<T> GetUpScaled() const;
		[[nodiscard]] constexpr Vector3<T> GetForwardScaled() const;
		[[nodiscard]] constexpr RotationMatrix3<T> GetRotationMatrix() const;
		[[nodiscard]] constexpr Quaternion<T> GetQuaternion() const;

		[[nodiscard]] static constexpr Matrix4x4<T> Identity();
		[[nodiscard]] static constexpr Matrix4x4<T> Zero();
		[[nodiscard]] static constexpr Matrix4x4<T> CreateRotationAroundX(const Radians<T> angle);
		[[nodiscard]] static constexpr Matrix4x4<T> CreateRotationAroundY(const Radians<T> angle);
		[[nodiscard]] static constexpr Matrix4x4<T> CreateRotationAroundZ(const Radians<T> angle);
		[[nodiscard]] static constexpr Matrix4x4<T> CreateRotationAroundX(const Degrees<T> angle);
		[[nodiscard]] static constexpr Matrix4x4<T> CreateRotationAroundY(const Degrees<T> angle);
		[[nodiscard]] static constexpr Matrix4x4<T> CreateRotationAroundZ(const Degrees<T> angle);
		[[nodiscard]] static constexpr Matrix4x4<T> CreateRotationMatrix(const Matrix4x4<T>& matrix);
		[[nodiscard]] static constexpr Matrix4x4<T> CreateTranslationMatrix(const Point3<T>& translation);
		[[nodiscard]] static constexpr Matrix4x4<T> CreateScaleMatrix(const Vector3<T>& scale);

		[[nodiscard]] static constexpr Matrix4x4<T> CreateRTMatrix(const RotationMatrix3<T>& rotationMatrix, const Point3<T>& translation);

		[[nodiscard]] static constexpr Matrix4x4<T> GetTransposed(const Matrix4x4<T>& matrix);
		[[nodiscard]] static constexpr Matrix4x4<T> GetInverse(Matrix4x4<T> matrix);

		// GetFastInverse should only be use on Matrices with scale of 1
		[[nodiscard]] static constexpr Matrix4x4<T> GetFastInverse(const Matrix4x4<T>& matrix);
		[[nodiscard]] constexpr Matrix4x4<T> GetFastInverse() const;

		[[nodiscard]] static constexpr unsigned int GetIndex(unsigned int row, unsigned int column) noexcept;

	private:

		static constexpr void SwapRows(Matrix4x4<T>& matrix, const int row1, const int row2);
		static constexpr void ScaleRow(Matrix4x4<T>& matrix, const int row, const T factor);
		static constexpr void AddScaledRow(Matrix4x4<T>& matrix, const int row1, const int row2, const T factor);

	private:

		std::array<T, ValueCount> mValues{};
	};

	using Matrix4x4f = Matrix4x4<float>;
	using Matrix4x4d = Matrix4x4<double>;
	using Matrix4x4i = Matrix4x4<int>;

	template<typename T>
	constexpr Matrix4x4<T>::Matrix4x4()
	{
		*this = Identity();
	}

	template<typename T>
	constexpr Matrix4x4<T>::Matrix4x4(const std::array<T, ValueCount>& values) noexcept
		: mValues(values)
	{
	}

	template<typename T>
	constexpr T& Matrix4x4<T>::operator()(const unsigned int row, const unsigned int column)
	{
		return mValues[GetIndex(row, column)];
	}

	template<typename T>
	constexpr const T& Matrix4x4<T>::operator()(const unsigned int row, const unsigned int column) const
	{
		return mValues[GetIndex(row, column)];
	}

	template<typename T>
	constexpr T& Matrix4x4<T>::operator[](const unsigned int index) noexcept
	{
		return mValues[index];
	}

	template<typename T>
	constexpr const T& Matrix4x4<T>::operator[](const unsigned int index) const noexcept
	{
		return mValues[index];
	}

	template<typename T>
	constexpr void Matrix4x4<T>::SetTranslation(const Point3<T>& translation)
	{
		mValues[12] = translation.x;
		mValues[13] = translation.y;
		mValues[14] = translation.z;
	}

	template<typename T>
	constexpr void Matrix4x4<T>::SetScale(const Vector3<T>& scale)
	{
		constexpr float minScale = 0.001f;

		const Vector3<T> xAxis = GetRightScaled();
		const Vector3<T> yAxis = GetUpScaled();
		const Vector3<T> zAxis = GetForwardScaled();

		SetRight(xAxis * Max(scale.x, minScale));
		SetUp(yAxis * Max(scale.y, minScale));
		SetForward(zAxis * Max(scale.z, minScale));
	}

	template<typename T>
	constexpr void Matrix4x4<T>::SetRight(const UnitVector3<T>& right)
	{
		mValues[0] = right.X();
		mValues[1] = right.Y();
		mValues[2] = right.Z();
	}

	template<typename T>
	constexpr void Matrix4x4<T>::SetUp(const UnitVector3<T>& up)
	{
		mValues[4] = up.X();
		mValues[5] = up.Y();
		mValues[6] = up.Z();
	}

	template<typename T>
	constexpr void Matrix4x4<T>::SetForward(const UnitVector3<T>& forward)
	{
		mValues[8] = forward.X();
		mValues[9] = forward.Y();
		mValues[10] = forward.Z();
	}

	template<typename T>
	constexpr void Matrix4x4<T>::SetRight(const Vector3<T>& right)
	{
		mValues[0] = right.x;
		mValues[1] = right.y;
		mValues[2] = right.z;
	}

	template<typename T>
	constexpr void Matrix4x4<T>::SetUp(const Vector3<T>& up)
	{
		mValues[4] = up.x;
		mValues[5] = up.y;
		mValues[6] = up.z;
	}

	template<typename T>
	constexpr void Matrix4x4<T>::SetForward(const Vector3<T>& forward)
	{
		mValues[8] = forward.x;
		mValues[9] = forward.y;
		mValues[10] = forward.z;
	}

	template<typename T>
	constexpr void Matrix4x4<T>::SetRotationMatrix(const Matrix3x3<T>& rotationMatrix)
	{
		mValues[0] = rotationMatrix(0, 0);
		mValues[1] = rotationMatrix(0, 1);
		mValues[2] = rotationMatrix(0, 2);
		mValues[4] = rotationMatrix(1, 0);
		mValues[5] = rotationMatrix(1, 1);
		mValues[6] = rotationMatrix(1, 2);
		mValues[8] = rotationMatrix(2, 0);
		mValues[9] = rotationMatrix(2, 1);
		mValues[10] = rotationMatrix(2, 2);
	}

	template<typename T>
	constexpr void Matrix4x4<T>::SetRotationMatrix(const RotationMatrix3<T>& rotationMatrix)
	{
		mValues[0] = rotationMatrix(0, 0);
		mValues[1] = rotationMatrix(0, 1);
		mValues[2] = rotationMatrix(0, 2);
		mValues[4] = rotationMatrix(1, 0);
		mValues[5] = rotationMatrix(1, 1);
		mValues[6] = rotationMatrix(1, 2);
		mValues[8] = rotationMatrix(2, 0);
		mValues[9] = rotationMatrix(2, 1);
		mValues[10] = rotationMatrix(2, 2);
	}

	template<typename T>
	constexpr T* Matrix4x4<T>::GetDataPtr()
	{
		return &mValues[0];
	}

	template<typename T>
	constexpr const T* Matrix4x4<T>::GetDataPtr() const
	{
		return &mValues[0];
	}

	template<typename T>
	constexpr Point3<T> Matrix4x4<T>::GetTranslation() const
	{
		return Point3<T>(mValues[12], mValues[13], mValues[14]);
	}

	template<typename T>
	constexpr Vector3<T> Matrix4x4<T>::GetScale() const
	{
		Vector3<T> scale;

		scale.x = Length(GetRightScaled());
		scale.y = Length(GetUpScaled());
		scale.z = Length(GetForwardScaled());

		return scale;
	}

	template<typename T>
	constexpr UnitVector3<T> Matrix4x4<T>::GetRight() const
	{
		return UnitVector3<T>(GetRightScaled());
	}

	template<typename T>
	constexpr UnitVector3<T> Matrix4x4<T>::GetUp() const
	{
		return UnitVector3<T>(GetUpScaled());
	}

	template<typename T>
	constexpr UnitVector3<T> Matrix4x4<T>::GetForward() const
	{
		return UnitVector3<T>(GetForwardScaled());
	}

	template<typename T>
	constexpr Vector3<T> Matrix4x4<T>::GetRightScaled() const
	{
		return Vector3<T>(mValues[0], mValues[1], mValues[2]);
	}

	template<typename T>
	constexpr Vector3<T> Matrix4x4<T>::GetUpScaled() const
	{
		return Vector3<T>(mValues[4], mValues[5], mValues[6]);
	}

	template<typename T>
	constexpr Vector3<T> Matrix4x4<T>::GetForwardScaled() const
	{
		return Vector3<T>(mValues[8], mValues[9], mValues[10]);
	}

	template<typename T>
	constexpr RotationMatrix3<T> Matrix4x4<T>::GetRotationMatrix() const
	{
		return RotationMatrix3<T>(
			{
				mValues[0], mValues[1], mValues[2],
				mValues[4], mValues[5], mValues[6],
				mValues[8], mValues[9], mValues[10]
			}
		);
	}

	template<typename T>
	constexpr Quaternion<T> Matrix4x4<T>::GetQuaternion() const
	{
		return Quaternion<T>(*this);
	}

	template<typename T>
	constexpr Matrix4x4<T> Matrix4x4<T>::Identity()
	{
		return Matrix4x4<T>
			(
				{
					1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1
				}
			);
	}

	template<typename T>
	constexpr Matrix4x4<T> Matrix4x4<T>::Zero()
	{
		return Matrix4x4<T>
			(
				{
					0, 0, 0, 0,
					0, 0, 0, 0,
					0, 0, 0, 0,
					0, 0, 0, 0
				}
			);
	}

	template<typename T>
	constexpr Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundX(const Radians<T> angle)
	{
		Matrix4x4<T> rotationMatrix;
		const T cosA = Cos(angle);
		const T sinA = Sin(angle);

		rotationMatrix(1, 1) = cosA;
		rotationMatrix(1, 2) = sinA;
		rotationMatrix(2, 1) = -sinA;
		rotationMatrix(2, 2) = cosA;

		return rotationMatrix;
	}

	template<typename T>
	constexpr Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundY(const Radians<T> angle)
	{
		Matrix4x4<T> rotationMatrix;
		const T cosA = Cos(angle);
		const T sinA = Sin(angle);

		rotationMatrix(0, 0) = cosA;
		rotationMatrix(0, 2) = -sinA;
		rotationMatrix(2, 0) = sinA;
		rotationMatrix(2, 2) = cosA;

		return rotationMatrix;
	}

	template<typename T>
	constexpr Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundZ(const Radians<T> angle)
	{
		Matrix4x4<T> rotationMatrix;
		const T cosA = Cos(angle);
		const T sinA = Sin(angle);

		rotationMatrix(0, 0) = cosA;
		rotationMatrix(0, 1) = sinA;
		rotationMatrix(1, 0) = -sinA;
		rotationMatrix(1, 1) = cosA;

		return rotationMatrix;
	}

	template<typename T>
	constexpr Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundX(const Degrees<T> angle)
	{
		return CreateRotationAroundX(ToRadians(angle));
	}

	template<typename T>
	constexpr Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundY(const Degrees<T> angle)
	{
		return CreateRotationAroundY(ToRadians(angle));
	}

	template<typename T>
	constexpr Matrix4x4<T> Matrix4x4<T>::CreateRotationAroundZ(const Degrees<T> angle)
	{
		return CreateRotationAroundZ(ToRadians(angle));
	}

	template<typename T>
	constexpr Matrix4x4<T> Matrix4x4<T>::CreateTranslationMatrix(const Point3<T>& translation)
	{
		Matrix4x4<T> result = Matrix4x4<T>::Identity();
		result.SetTranslation(translation);
		return result;
	}

	template<typename T>
	constexpr Matrix4x4<T> Matrix4x4<T>::CreateRotationMatrix(const Matrix4x4<T>& matrix)
	{
		const Vector3<T> scale = matrix.GetScale();

		Matrix4x4<T> rotationMatrix = Matrix4x4<T>::Identity();

		rotationMatrix(0, 0) = matrix(0, 0) / scale.x;
		rotationMatrix(1, 0) = matrix(1, 0) / scale.x;
		rotationMatrix(2, 0) = matrix(2, 0) / scale.x;

		rotationMatrix(0, 1) = matrix(0, 1) / scale.y;
		rotationMatrix(1, 2) = matrix(1, 1) / scale.y;
		rotationMatrix(2, 2) = matrix(1, 1) / scale.y;

		rotationMatrix(0, 2) = matrix(0, 2) / scale.z;
		rotationMatrix(1, 2) = matrix(1, 2) / scale.z;
		rotationMatrix(2, 2) = matrix(2, 2) / scale.z;

		return rotationMatrix;
	}

	template<typename T>
	constexpr Matrix4x4<T> Matrix4x4<T>::CreateScaleMatrix(const Vector3<T>& scale)
	{
		Matrix4x4<T> result = Matrix4x4<T>::Identity();

		result(0, 0) = scale.x;
		result(1, 1) = scale.y;
		result(2, 2) = scale.z;

		return result;
	}

	template<typename T>
	constexpr Matrix4x4<T> Matrix4x4<T>::CreateRTMatrix(const RotationMatrix3<T>& rotationMatrix, const Point3<T>& translation)
	{
		Matrix4x4<T> result = Matrix4x4<T>::Identity();
		result.SetRotationMatrix(rotationMatrix);
		result.SetTranslation(translation);
		return result;
	}

	template<typename T>
	constexpr Matrix4x4<T> Matrix4x4<T>::GetTransposed(const Matrix4x4<T>& m)
	{
		Matrix4x4<T> transposed;

		for (int row = 0; row < 4; row++)
		{
			for (int col = 0; col < 4; col++)
			{
				transposed(row, col) = m(col, row);
			}
		}

		return transposed;
	}

	template<typename T>
	constexpr Matrix4x4<T> Matrix4x4<T>::GetFastInverse(const Matrix4x4<T>& m)
	{
		Matrix4x4<T> inv = Matrix4x4<T>::Identity();

		// Transpose rotation part (since R^-1 = R^T)
		inv(0, 0) = m(0, 0); inv(0, 1) = m(1, 0); inv(0, 2) = m(2, 0);
		inv(1, 0) = m(0, 1); inv(1, 1) = m(1, 1); inv(1, 2) = m(2, 1);
		inv(2, 0) = m(0, 2); inv(2, 1) = m(1, 2); inv(2, 2) = m(2, 2);

		// Compute new translation
		inv(3, 0) = -(m(3, 0) * inv(0, 0) + m(3, 1) * inv(1, 0) + m(3, 2) * inv(2, 0));
		inv(3, 1) = -(m(3, 0) * inv(0, 1) + m(3, 1) * inv(1, 1) + m(3, 2) * inv(2, 1));
		inv(3, 2) = -(m(3, 0) * inv(0, 2) + m(3, 1) * inv(1, 2) + m(3, 2) * inv(2, 2));
		inv(3, 3) = 1.0f;

		return inv;
	}

	template<typename T>
	constexpr Matrix4x4<T> Matrix4x4<T>::GetInverse(Matrix4x4<T> matrixToInverse) //Dear Savior ChatGPT, value seem to be correct for most common cases so far (v9.19.1)
	{
		Matrix4x4<T> inverse = Matrix4x4<T>::Identity();

		for (int i = 0; i < 4; ++i)
		{
			int pivotRow = i;

			while (pivotRow < 4 && matrixToInverse(pivotRow, i) == 0)
			{
				++pivotRow;
			}

			if (pivotRow == 4)
				assert(false && "Matrix is singular, no unique inverse.");

			SwapRows(matrixToInverse, i, pivotRow);
			SwapRows(inverse, i, pivotRow);

			float pivotElement = matrixToInverse(i, i);
			ScaleRow(matrixToInverse, i, 1 / pivotElement);
			ScaleRow(inverse, i, 1 / pivotElement);

			for (int j = 0; j < 4; ++j)
			{
				if (j != i)
				{
					float factor = -matrixToInverse(j, i);
					AddScaledRow(matrixToInverse, j, i, factor);
					AddScaledRow(inverse, j, i, factor);
				}
			}
		}

		return inverse;
	}

	template<typename T>
	constexpr Matrix4x4<T> Matrix4x4<T>::GetFastInverse() const
	{
		return GetFastInverse(*this);
	}

	template<typename T>
	constexpr unsigned int Matrix4x4<T>::GetIndex(const unsigned int row, const unsigned int column) noexcept
	{
		return row * Dimensions + column;
	}

	template<typename T>
	constexpr void Matrix4x4<T>::SwapRows(Matrix4x4<T>& matrix, const int row1, const int row2)
	{
		for (int i = 0; i < 4; ++i)
		{
			std::swap(matrix(row1, i), matrix(row2, i));
		}
	}

	template<typename T>
	constexpr void Matrix4x4<T>::ScaleRow(Matrix4x4<T>& matrix, const int row, const T factor)
	{
		for (int i = 0; i < 4; ++i)
		{
			matrix(row, i) *= factor;
		}
	}

	template<typename T>
	constexpr void Matrix4x4<T>::AddScaledRow(Matrix4x4<T>& matrix, const int row1, const int row2, const T factor)
	{
		for (int i = 0; i < 4; ++i)
		{
			matrix(row1, i) += factor * matrix(row2, i);
		}
	}

	template<typename T>
	[[nodiscard]] constexpr Matrix4x4<T> operator+(const Matrix4x4<T>& a, const Matrix4x4<T>& b) noexcept
	{
		Matrix4x4<T> matrix;

		matrix(0, 0) = a(0, 0) + b(0, 0);
		matrix(0, 1) = a(0, 1) + b(0, 1);
		matrix(0, 2) = a(0, 2) + b(0, 2);
		matrix(0, 3) = a(0, 3) + b(0, 3);

		matrix(1, 0) = a(1, 0) + b(1, 0);
		matrix(1, 1) = a(1, 1) + b(1, 1);
		matrix(1, 2) = a(1, 2) + b(1, 2);
		matrix(1, 3) = a(1, 3) + b(1, 3);

		matrix(2, 0) = a(2, 0) + b(2, 0);
		matrix(2, 1) = a(2, 1) + b(2, 1);
		matrix(2, 2) = a(2, 2) + b(2, 2);
		matrix(2, 3) = a(2, 3) + b(2, 3);

		matrix(3, 0) = a(3, 0) + b(3, 0);
		matrix(3, 1) = a(3, 1) + b(3, 1);
		matrix(3, 2) = a(3, 2) + b(3, 2);
		matrix(3, 3) = a(3, 3) + b(3, 3);

		return matrix;
	}

	template<typename T>
	[[nodiscard]] constexpr Matrix4x4<T> operator-(const Matrix4x4<T>& a, const Matrix4x4<T>& b) noexcept
	{
		Matrix4x4<T> matrix;

		matrix(0, 0) = a(0, 0) - b(0, 0);
		matrix(0, 1) = a(0, 1) - b(0, 1);
		matrix(0, 2) = a(0, 2) - b(0, 2);
		matrix(0, 3) = a(0, 3) - b(0, 3);

		matrix(1, 0) = a(1, 0) - b(1, 0);
		matrix(1, 1) = a(1, 1) - b(1, 1);
		matrix(1, 2) = a(1, 2) - b(1, 2);
		matrix(1, 3) = a(1, 3) - b(1, 3);

		matrix(2, 0) = a(2, 0) - b(2, 0);
		matrix(2, 1) = a(2, 1) - b(2, 1);
		matrix(2, 2) = a(2, 2) - b(2, 2);
		matrix(2, 3) = a(2, 3) - b(2, 3);

		matrix(3, 0) = a(3, 0) - b(3, 0);
		matrix(3, 1) = a(3, 1) - b(3, 1);
		matrix(3, 2) = a(3, 2) - b(3, 2);
		matrix(3, 3) = a(3, 3) - b(3, 3);

		return matrix;
	}

	template<typename T>
	[[nodiscard]] constexpr Matrix4x4<T> operator*(const Matrix4x4<T>& a, const Matrix4x4<T>& b) noexcept
	{
		Matrix4x4<T> result = Matrix4x4<T>::Zero();
		for (int row = 0; row < 4; ++row)
		{
			for (int col = 0; col < 4; ++col)
			{
				T sum = 0;
				for (int k = 0; k < 4; ++k)
				{
					sum += a(row, k) * b(k, col);
				}
				result(row, col) = sum;
			}
		}
		return result;
	}

	template<typename T>
	constexpr Matrix4x4<T>& operator+=(Matrix4x4<T>& a, const Matrix4x4<T>& b) noexcept
	{
		a = a + b;
		return a;
	}

	template<typename T>
	constexpr Matrix4x4<T>& operator-=(Matrix4x4<T>& a, const Matrix4x4<T>& b) noexcept
	{
		a = a - b;
		return a;
	}

	template<typename T>
	constexpr Matrix4x4<T>& operator*=(Matrix4x4<T>& a, const Matrix4x4<T>& b) noexcept
	{
		a = a * b;
		return a;
	}

	/*template<typename T>
	[[nodiscard]] constexpr Vector4<T> operator*(const Matrix4x4<T>& matrix, const Vector4<T>& vector) noexcept
	{
		Vector4<T> result;

		result.x = vector.x * matrix[0] + vector.y * matrix[4] + vector.z * matrix[8] + vector.w * matrix[12];
		result.y = vector.x * matrix[1] + vector.y * matrix[5] + vector.z * matrix[9] + vector.w * matrix[13];
		result.z = vector.x * matrix[2] + vector.y * matrix[6] + vector.z * matrix[10] + vector.w * matrix[14];
		result.w = vector.x * matrix[3] + vector.y * matrix[7] + vector.z * matrix[11] + vector.w * matrix[15];

		return result;
	}*/

	//// COLUMN VECTOR VERSION
	//template<typename T>
	//[[nodiscard]] constexpr Vector4<T> operator*(const Matrix4x4<T>& m, const Vector4<T>& v) noexcept
	//{
	//	Vector4<T> result;

	//	result.x = m[0] * v.x + m[1] * v.y + m[2] * v.z + m[3] * v.w;
	//	result.y = m[4] * v.x + m[5] * v.y + m[6] * v.z + m[7] * v.w;
	//	result.z = m[8] * v.x + m[9] * v.y + m[10] * v.z + m[11] * v.w;
	//	result.w = m[12] * v.x + m[13] * v.y + m[14] * v.z + m[15] * v.w;

	//	return result;
	//}

	template<typename T>
	[[nodiscard]] constexpr Vector4<T> operator*(const Vector4<T>& vector, const Matrix4x4<T>& matrix) noexcept
	{
		Vector4<T> result;

		result.x = vector.x * matrix[0] + vector.y * matrix[4] + vector.z * matrix[8] + vector.w * matrix[12];
		result.y = vector.x * matrix[1] + vector.y * matrix[5] + vector.z * matrix[9] + vector.w * matrix[13];
		result.z = vector.x * matrix[2] + vector.y * matrix[6] + vector.z * matrix[10] + vector.w * matrix[14];
		result.w = vector.x * matrix[3] + vector.y * matrix[7] + vector.z * matrix[11] + vector.w * matrix[15];

		return result;
	}

	template<typename T>
	[[nodiscard]] constexpr Vector3<T> operator*(const Matrix4x4<T>& matrix, const Vector3<T>& vector) noexcept
	{
		const Vector4<T> v = Vector4<T>(vector.x, vector.y, vector.z, 1) * matrix;
		return Vector3<T>(v.x, v.y, v.z);
	}

	template<typename T>
	[[nodiscard]] constexpr Vector3<T> operator*(const Vector3<T>& vector, const Matrix4x4<T>& matrix) noexcept
	{
		return matrix * vector;
	}

	template<typename T>
	[[nodiscard]] constexpr Point3<T> operator*(const Matrix4x4<T>& matrix, const Point3<T>& point) noexcept
	{
		const Vector4<T> v = Vector4<T>(point.x, point.y, point.z, 1) * matrix;
		return Point3<T>(v.x, v.y, v.z);
	}

	template<typename T>
	[[nodiscard]] constexpr Point3<T> operator*(const Point3<T>& point, const Matrix4x4<T>& matrix) noexcept
	{
		return matrix * point;
	}

	template<typename T>
	[[nodiscard]] constexpr bool operator==(const Matrix4x4<T>& a, const Matrix4x4<T>& b) noexcept
	{
		for (unsigned int i = 0; i < 16; ++i)
		{
			if (a[i] != b[i])
			{
				return false;
			}
		}

		return true;
	}

	template <class T>
	std::ostream& operator<<(std::ostream& os, const Matrix4x4<T>& matrix)
	{
		float max = 1;

		for (int row = 0; row < 4; ++row)
		{
			for (int column = 0; column < 4; ++column)
			{
				max = Max(max, static_cast<float>(matrix(row, column)));
			}
		}

		const std::string maxString = std::to_string(max);
		const size_t characterLength = maxString.length() + 5;
		const size_t totalLength = characterLength * 4 + 2;

		return os
			<< std::endl
			<< " " << std::string(totalLength, '-') << std::endl
			<< "| " << std::setw(characterLength) << matrix(0, 0) << std::setw(characterLength) << matrix(0, 1) << std::setw(characterLength) << matrix(0, 2) << std::setw(characterLength) << matrix(0, 3) << " |" << std::endl
			<< "| " << std::setw(characterLength) << matrix(1, 0) << std::setw(characterLength) << matrix(1, 1) << std::setw(characterLength) << matrix(1, 2) << std::setw(characterLength) << matrix(1, 3) << " |" << std::endl
			<< "| " << std::setw(characterLength) << matrix(2, 0) << std::setw(characterLength) << matrix(2, 1) << std::setw(characterLength) << matrix(2, 2) << std::setw(characterLength) << matrix(2, 3) << " |" << std::endl
			<< "| " << std::setw(characterLength) << matrix(3, 0) << std::setw(characterLength) << matrix(3, 1) << std::setw(characterLength) << matrix(3, 2) << std::setw(characterLength) << matrix(3, 3) << " |" << std::endl
			<< " " << std::string(totalLength, '-') << std::endl;
	}
}