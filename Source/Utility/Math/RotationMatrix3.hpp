#pragma once
#include "Utility/Math/Matrix3x3.hpp"
#include "Utility/Math/Vector3.hpp"
#include "Utility/Math/UnitVector3.hpp"

namespace Simple
{

	template<typename T>
	class RotationMatrix3 final
	{
	public:

		constexpr RotationMatrix3() noexcept = default;
		constexpr explicit RotationMatrix3(const std::array<T, 9>& values) noexcept;
		constexpr RotationMatrix3(const UnitVector3<T>& right, const UnitVector3<T>& up, const UnitVector3<T>& forward);

		[[nodiscard]] constexpr const T& operator()(const unsigned int row, const unsigned int column) const;

		[[nodiscard]] static constexpr RotationMatrix3<T> Identity() noexcept;
		[[nodiscard]] static constexpr RotationMatrix3<T> CreateFromXY(const UnitVector3<T>& xAxis, const UnitVector3<T>& yAxis);

	private:

		Matrix3x3<T> mMatrix;
	};

	using RotationMatrix3f = RotationMatrix3<float>;
	using RotationMatrix3d = RotationMatrix3<double>;

	template<typename T>
	constexpr RotationMatrix3<T>::RotationMatrix3(const std::array<T, 9>& values) noexcept
		: mMatrix(values)
	{
	}

	template<typename T>
	constexpr RotationMatrix3<T>::RotationMatrix3(const UnitVector3<T>& right, const UnitVector3<T>& up, const UnitVector3<T>& forward)
		: mMatrix({ right.X(), right.Y(), right.Z(), up.X(), up.Y(), up.Z(), forward.X(), forward.Y(), forward.Z() })
	{
	}

	template<typename T>
	constexpr const T& RotationMatrix3<T>::operator()(const unsigned int row, const unsigned int column) const
	{
		return mMatrix(row, column);
	}

	template<typename T>
	constexpr RotationMatrix3<T> RotationMatrix3<T>::Identity() noexcept
	{
		return RotationMatrix3({ 1, 0, 0, 0, 1, 0, 0, 0, 1 });
	}

	template<typename T>
	constexpr RotationMatrix3<T> RotationMatrix3<T>::CreateFromXY(const UnitVector3<T>& xAxis, const UnitVector3<T>& yAxis)
	{
		const UnitVector3<T> zAxis = Cross(xAxis, yAxis);
		const UnitVector3<T> correctedYAxis = Cross(zAxis, xAxis);
		return RotationMatrix3<T>(xAxis, correctedYAxis, zAxis);
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
	[[nodiscard]] constexpr UnitVector3<T> operator*(const UnitVector3<T>& vector, const RotationMatrix3<T>& matrix) noexcept
	{
		return UnitVector3<T>
			(
				vector.X() * matrix(0, 0) + vector.Y() * matrix(1, 0) + vector.Z() * matrix(2, 0),
				vector.X() * matrix(0, 1) + vector.Y() * matrix(1, 1) + vector.Z() * matrix(2, 1),
				vector.X() * matrix(0, 2) + vector.Y() * matrix(1, 2) + vector.Z() * matrix(2, 2)
			);
	}
}