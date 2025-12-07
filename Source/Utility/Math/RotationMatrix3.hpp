#pragma once
#include "Utility/Math/Matrix3x3.hpp"
#include "Utility/Math/Vector3.hpp"
#include "Utility/Math/UnitVector3.hpp"
#include "Utility/Math/VectorMath.hpp"
#include "Utility/Assert.hpp"
#include <array>
#include <limits>

namespace Simple
{

	template<typename T>
	[[nodiscard]] constexpr T GetDeterminant(const Matrix3x3<T>& m) noexcept
	{
		return m(0, 0) * (m(1, 1) * m(2, 2) - m(1, 2) * m(2, 1)) -
			m(0, 1) * (m(1, 0) * m(2, 2) - m(1, 2) * m(2, 0)) +
			m(0, 2) * (m(1, 0) * m(2, 1) - m(1, 1) * m(2, 0));
	}

	template<typename T>
	[[nodiscard]] constexpr bool IsPure(const Matrix3x3<T>& m) noexcept
	{
		const T det = GetDeterminant(m);
		if constexpr (std::same_as<T, float>)
		{
			return Abs(det - T{ 1 }) < T{ 1e-4 };
		}
		else
		{
			return Abs(det - T{ 1 }) < T{ 1e-8 };
		}
	}

	template<typename T>
	class RotationMatrix3 final
	{
	public:

		constexpr RotationMatrix3() noexcept = default;
		constexpr explicit RotationMatrix3(const std::array<T, 9>& values);

		[[nodiscard]] constexpr const T& operator()(const unsigned int row, const unsigned int column) const noexcept;
		[[nodiscard]] constexpr const T& operator[](const unsigned int index) const noexcept;

		[[nodiscard]] constexpr const UnitVector3<T>& GetRight() const noexcept;
		[[nodiscard]] constexpr const UnitVector3<T>& GetUp() const noexcept;
		[[nodiscard]] constexpr const UnitVector3<T>& GetForward() const noexcept;

		[[nodiscard]] constexpr Matrix3x3<T> ToMatrix() const noexcept
		{
			return Matrix3x3<T>
				(
					{
						mRight.X(), mRight.Y(), mRight.Z(),
						mUp.X(),	mUp.Y(),    mUp.Z(),
						mForward.X(), mForward.Y(), mForward.Z()
					}
				);
		}

		[[nodiscard]] static constexpr RotationMatrix3<T> Identity() noexcept;
		[[nodiscard]] static constexpr RotationMatrix3<T> FromAxes(const UnitVector3<T>& right, const UnitVector3<T>& up, const UnitVector3<T>& forward);
		[[nodiscard]] static constexpr RotationMatrix3<T> FromXY(const UnitVector3<T>& xAxis, const UnitVector3<T>& yAxis);
		[[nodiscard]] static constexpr RotationMatrix3<T> FromXZ(const UnitVector3<T>& xAxis, const UnitVector3<T>& zAxis);
		[[nodiscard]] static constexpr RotationMatrix3<T> FromYX(const UnitVector3<T>& yAxis, const UnitVector3<T>& xAxis);
		[[nodiscard]] static constexpr RotationMatrix3<T> FromYZ(const UnitVector3<T>& yAxis, const UnitVector3<T>& zAxis);
		[[nodiscard]] static constexpr RotationMatrix3<T> FromZX(const UnitVector3<T>& zAxis, const UnitVector3<T>& xAxis);
		[[nodiscard]] static constexpr RotationMatrix3<T> FromZY(const UnitVector3<T>& zAxis, const UnitVector3<T>& yAxis);
        [[nodiscard]] static constexpr RotationMatrix3<T> FromX(const UnitVector3<T>& xAxis);
        [[nodiscard]] static constexpr RotationMatrix3<T> FromY(const UnitVector3<T>& yAxis);
		[[nodiscard]] static constexpr RotationMatrix3<T> FromZ(const UnitVector3<T>& zAxis);

		[[nodiscard]] friend constexpr bool operator==(const RotationMatrix3<T>& a, const RotationMatrix3<T>& b) noexcept
		{
			return a.mRight == b.mRight && a.mUp == b.mUp && a.mForward == b.mForward;
		}

	private:

		constexpr RotationMatrix3(const UnitVector3<T>& right, const UnitVector3<T>& up, const UnitVector3<T>& forward) noexcept
			: mRight(right)
			, mUp(up)
			, mForward(forward)
		{
			const auto det = GetDeterminant(ToMatrix());
			det;
			const auto dot = Dot(up, forward);
			dot;
			ASSERT(IsPure(ToMatrix()) && "The provided axes do not form a pure rotation matrix.");
		}

	private:

		UnitVector3<T> mRight = UnitVector3<T>::Right();
		UnitVector3<T> mUp = UnitVector3<T>::Up();
		UnitVector3<T> mForward = UnitVector3<T>::Forward();
	};


	using RotationMatrix3f = RotationMatrix3<float>;
	using RotationMatrix3d = RotationMatrix3<double>;

	template<typename T>
	constexpr RotationMatrix3<T>::RotationMatrix3(const std::array<T, 9>& values)
		: mRight(values[0], values[1], values[2])
		, mUp(values[3], values[4], values[5])
		, mForward(values[6], values[7], values[8])
	{
		ASSERT(IsPure(ToMatrix()) && "The provided matrix is not a pure rotation matrix.");
	}

	template<typename T>
	constexpr const T& RotationMatrix3<T>::operator()(const unsigned int row, const unsigned int column) const noexcept
	{
		static_assert(sizeof(RotationMatrix3<T>) == sizeof(T) * 3 * 3);
		const unsigned int index = row * 3 + column;
		return reinterpret_cast<const T*>(this)[index];
	}

	template<typename T>
	constexpr const T& RotationMatrix3<T>::operator[](const unsigned int index) const noexcept
	{
		static_assert(sizeof(RotationMatrix3<T>) == sizeof(T) * 3 * 3);
		return reinterpret_cast<const T*>(this)[index];
	}

	template<typename T>
	constexpr const UnitVector3<T>& RotationMatrix3<T>::GetRight() const noexcept
	{
		return mRight;
	}

	template<typename T>
	constexpr const UnitVector3<T>& RotationMatrix3<T>::GetUp() const noexcept
	{
		return mUp;
	}

	template<typename T>
	constexpr const UnitVector3<T>& RotationMatrix3<T>::GetForward() const noexcept
	{
		return mForward;
	}

	template<typename T>
	constexpr RotationMatrix3<T> RotationMatrix3<T>::Identity() noexcept
	{
		return RotationMatrix3(UnitVector3<T>::Right(), UnitVector3<T>::Up(), UnitVector3<T>::Forward());
	}

	template<typename T>
	constexpr RotationMatrix3<T> RotationMatrix3<T>::FromAxes(const UnitVector3<T>& right, const UnitVector3<T>& up, const UnitVector3<T>& forward)
	{
		return RotationMatrix3<T>(right, up, forward);
	}

	template<typename T>
	constexpr RotationMatrix3<T> RotationMatrix3<T>::FromXY(const UnitVector3<T>& xAxis, const UnitVector3<T>& yAxis)
	{
		const UnitVector3<T> zAxis = Cross(xAxis, yAxis);
		const UnitVector3<T> yAxisCorrected = Cross(zAxis, xAxis);
		return FromAxes(xAxis, yAxisCorrected, zAxis);
	}

	template<typename T>
	constexpr RotationMatrix3<T> RotationMatrix3<T>::FromXZ(const UnitVector3<T>& xAxis, const UnitVector3<T>& zAxis)
	{
		const UnitVector3<T> yAxis = Cross(zAxis, xAxis);
		const UnitVector3<T> zAxisCorrected = Cross(xAxis, yAxis);
		return FromAxes(xAxis, yAxis, zAxisCorrected);
	}

	template<typename T>
	constexpr RotationMatrix3<T> RotationMatrix3<T>::FromYX(const UnitVector3<T>& yAxis, const UnitVector3<T>& xAxis)
	{
		const UnitVector3<T> zAxis = Cross(xAxis, yAxis);
		const UnitVector3<T> xAxisCorrected = Cross(yAxis, zAxis);
		return FromAxes(xAxisCorrected, yAxis, zAxis);
	}

	template<typename T>
	constexpr RotationMatrix3<T> RotationMatrix3<T>::FromYZ(const UnitVector3<T>& yAxis, const UnitVector3<T>& zAxis)
	{
		const UnitVector3<T> xAxis = Cross(yAxis, zAxis);
		const UnitVector3<T> zAxisCorrected = Cross(xAxis, yAxis);
		return FromAxes(xAxis, yAxis, zAxisCorrected);
	}

	template<typename T>
	constexpr RotationMatrix3<T> RotationMatrix3<T>::FromZX(const UnitVector3<T>& zAxis, const UnitVector3<T>& xAxis)
	{
		const UnitVector3<T> yAxis = Cross(zAxis, xAxis);
		const UnitVector3<T> xAxisCorrected = Cross(yAxis, zAxis);
		return FromAxes(xAxisCorrected, yAxis, zAxis);
	}

	template<typename T>
	constexpr RotationMatrix3<T> RotationMatrix3<T>::FromZY(const UnitVector3<T>& zAxis, const UnitVector3<T>& yAxis)
	{
		const UnitVector3<T> xAxis = Cross(yAxis, zAxis);
		const UnitVector3<T> yAxisCorrected = Cross(zAxis, xAxis);
		return FromAxes(xAxis, yAxisCorrected, zAxis);
	}

	template<typename T>
	constexpr RotationMatrix3<T> RotationMatrix3<T>::FromX(const UnitVector3<T>& xAxis)
	{
		const UnitVector3<T> yAxis = GetPerpendicularVector(xAxis);
		return FromXY(xAxis, yAxis);
    }

	template<typename T>
	constexpr RotationMatrix3<T> RotationMatrix3<T>::FromY(const UnitVector3<T>& yAxis)
	{
		const UnitVector3<T> zAxis = GetPerpendicularVector(yAxis);
		return FromYZ(yAxis, zAxis);
    }

	template<typename T>
	constexpr RotationMatrix3<T> RotationMatrix3<T>::FromZ(const UnitVector3<T>& zAxis)
	{
		const UnitVector3<T> xAxis = GetPerpendicularVector(zAxis);
		return FromXZ(xAxis, zAxis);
    }

	template<typename T>
	[[nodiscard]] constexpr RotationMatrix3<T> operator*(const RotationMatrix3<T>& a, const RotationMatrix3<T>& b)
	{
        auto m = a.ToMatrix() * b.ToMatrix();
		return RotationMatrix3<T>(m.GetValues());
    }

	template<typename T>
	[[nodiscard]] constexpr Vector3<T> operator*(const Vector3<T>& vector, const RotationMatrix3<T>& matrix) noexcept
	{
		const UnitVector3<T> right = matrix.GetRight();
		const UnitVector3<T> up = matrix.GetUp();
		const UnitVector3<T> forward = matrix.GetForward();
		return Vector3<T>
			(
				vector.x * right.X() + vector.y * up.X() + vector.z * forward.X(),
				vector.x * right.Y() + vector.y * up.Y() + vector.z * forward.Y(),
				vector.x * right.Z() + vector.y * up.Z() + vector.z * forward.Z()
			);
	}

	template<typename T>
	[[nodiscard]] constexpr UnitVector3<T> operator*(const UnitVector3<T>& vector, const RotationMatrix3<T>& matrix)
	{
		const UnitVector3<T> right = matrix.GetRight();
		const UnitVector3<T> up = matrix.GetUp();
		const UnitVector3<T> forward = matrix.GetForward();
		return UnitVector3<T>
			(
				vector.X() * right.X() + vector.Y() * up.X() + vector.Z() * forward.X(),
				vector.X() * right.Y() + vector.Y() * up.Y() + vector.Z() * forward.Y(),
				vector.X() * right.Z() + vector.Y() * up.Z() + vector.Z() * forward.Z()
			);
	}

}