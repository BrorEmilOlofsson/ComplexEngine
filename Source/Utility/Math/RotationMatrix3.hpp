#pragma once
#include "Utility/Math/Matrix3x3.hpp"
#include "Utility/Math/Vector3.hpp"
#include "Utility/Math/UnitVector3.hpp"
#include "Utility/Math/VectorMath.hpp"

namespace Simple
{

	template<typename T>
	class RotationMatrix3 final
	{
	public:

		constexpr RotationMatrix3() noexcept = default;
		constexpr explicit RotationMatrix3(const std::array<T, 9>& values);

		[[nodiscard]] constexpr const T& operator()(const unsigned int row, const unsigned int column) const;

		[[nodiscard]] constexpr UnitVector3<T> GetRight() const;
		[[nodiscard]] constexpr UnitVector3<T> GetUp() const;
		[[nodiscard]] constexpr UnitVector3<T> GetForward() const;
		[[nodiscard]] constexpr T GetDeterminant() const noexcept;

		[[nodiscard]] static constexpr RotationMatrix3<T> Identity() noexcept;
		[[nodiscard]] static constexpr RotationMatrix3<T> FromAxes(const UnitVector3<T>& right, const UnitVector3<T>& up, const UnitVector3<T>& forward);
		[[nodiscard]] static constexpr RotationMatrix3<T> FromXY(const UnitVector3<T>& xAxis, const UnitVector3<T>& yAxis);

		[[nodiscard]] friend constexpr bool operator==(const RotationMatrix3<T>& a, const RotationMatrix3<T>& b) noexcept
		{
			return a.mMatrix == b.mMatrix;
		}

	private:

		[[nodiscard]] constexpr bool IsPure() const noexcept;

	private:

		Matrix3x3<T> mMatrix;
	};

	using RotationMatrix3f = RotationMatrix3<float>;
	using RotationMatrix3d = RotationMatrix3<double>;

	template<typename T>
	constexpr RotationMatrix3<T>::RotationMatrix3(const std::array<T, 9>& values)
		: mMatrix(values)
	{
		if (!IsPure())
		{
			throw std::invalid_argument("The provided matrix is not a pure rotation matrix.");
		}
	}

	template<typename T>
	constexpr const T& RotationMatrix3<T>::operator()(const unsigned int row, const unsigned int column) const
	{
		return mMatrix(row, column);
	}

	template<typename T>
	constexpr UnitVector3<T> RotationMatrix3<T>::GetRight() const
	{
		return UnitVector3<T>(mMatrix(0, 0), mMatrix(0, 1), mMatrix(0, 2));
	}

	template<typename T>
	constexpr UnitVector3<T> RotationMatrix3<T>::GetUp() const
	{
		return UnitVector3<T>(mMatrix(1, 0), mMatrix(1, 1), mMatrix(1, 2));
	}

	template<typename T>
	constexpr UnitVector3<T> RotationMatrix3<T>::GetForward() const
	{
		return UnitVector3<T>(mMatrix(2, 0), mMatrix(2, 1), mMatrix(2, 2));
	}

	template<typename T>
	constexpr T RotationMatrix3<T>::GetDeterminant() const noexcept
	{
		return mMatrix(0, 0) * (mMatrix(1, 1) * mMatrix(2, 2) - mMatrix(1, 2) * mMatrix(2, 1)) -
			mMatrix(0, 1) * (mMatrix(1, 0) * mMatrix(2, 2) - mMatrix(1, 2) * mMatrix(2, 0)) +
			mMatrix(0, 2) * (mMatrix(1, 0) * mMatrix(2, 1) - mMatrix(1, 1) * mMatrix(2, 0));
	}

	template<typename T>
	constexpr bool RotationMatrix3<T>::IsPure() const noexcept
	{
		const T det = GetDeterminant();
		const T epsilon = static_cast<T>(1e-6);
		return Abs(det - static_cast<T>(1)) < epsilon;
	}

	template<typename T>
	constexpr RotationMatrix3<T> RotationMatrix3<T>::Identity() noexcept
	{
		return RotationMatrix3({ 1, 0, 0, 0, 1, 0, 0, 0, 1 });
	}

	template<typename T>
	constexpr RotationMatrix3<T> RotationMatrix3<T>::FromAxes(const UnitVector3<T>& right, const UnitVector3<T>& up, const UnitVector3<T>& forward)
	{
		return RotationMatrix3<T>({ right.X(), right.Y(), right.Z(), up.X(), up.Y(), up.Z(), forward.X(), forward.Y(), forward.Z() });
	}

	template<typename T>
	constexpr RotationMatrix3<T> RotationMatrix3<T>::FromXY(const UnitVector3<T>& xAxis, const UnitVector3<T>& yAxis)
	{
		const UnitVector3<T> zAxis = Cross(xAxis, yAxis);
		const UnitVector3<T> correctedYAxis = Cross(zAxis, xAxis);
		return FromAxes(xAxis, correctedYAxis, zAxis);
	}

	template<typename T>
	[[nodiscard]] constexpr Vector3<T> operator*(const Vector3<T>& vector, const RotationMatrix3<T>& matrix) noexcept
	{
		return Vector3<T>
			(
				vector.x * matrix(0, 0) + vector.y * matrix(1, 0) + vector.z * matrix(2, 0),
				vector.x * matrix(0, 1) + vector.y * matrix(1, 1) + vector.z * matrix(2, 1),
				vector.x * matrix(0, 2) + vector.y * matrix(1, 2) + vector.z * matrix(2, 2)
			);
	}

	template<typename T>
	[[nodiscard]] constexpr UnitVector3<T> operator*(const UnitVector3<T>& vector, const RotationMatrix3<T>& matrix)
	{
		return UnitVector3<T>
			(
				vector.X() * matrix(0, 0) + vector.Y() * matrix(1, 0) + vector.Z() * matrix(2, 0),
				vector.X() * matrix(0, 1) + vector.Y() * matrix(1, 1) + vector.Z() * matrix(2, 1),
				vector.X() * matrix(0, 2) + vector.Y() * matrix(1, 2) + vector.Z() * matrix(2, 2)
			);
	}

}