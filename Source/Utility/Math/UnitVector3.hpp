#pragma once
#include "Utility/Math/Vector3.hpp"
#include "Utility/Math/Math.hpp"
#include <iostream>

namespace Simple
{

	template<typename T>
	class UnitVector3 final
	{
	public:

		constexpr UnitVector3() = default;
		constexpr UnitVector3(const T& x, const T& y, const T& z);
		constexpr explicit UnitVector3(const Vector3<T>& vector);

		[[nodiscard]] constexpr const T& X() const noexcept;
		[[nodiscard]] constexpr const T& Y() const noexcept;
		[[nodiscard]] constexpr const T& Z() const noexcept;

		constexpr void SetX(const T& x);
		constexpr void SetY(const T& y);
		constexpr void SetZ(const T& z);

		[[nodiscard]] static constexpr UnitVector3<T> Right() noexcept;
		[[nodiscard]] static constexpr UnitVector3<T> Up() noexcept;
		[[nodiscard]] static constexpr UnitVector3<T> Forward() noexcept;
		[[nodiscard]] static constexpr UnitVector3<T> Left() noexcept;
		[[nodiscard]] static constexpr UnitVector3<T> Down() noexcept;
		[[nodiscard]] static constexpr UnitVector3<T> Backward() noexcept;

		[[nodiscard]] friend constexpr UnitVector3<T> operator-(const UnitVector3<T>& vector) noexcept
		{
			return UnitVector3<T>(UnsafeTag{}, -vector.mX, -vector.mY, -vector.mZ);
		}

	private:

		struct UnsafeTag { constexpr UnsafeTag() = default; };

		constexpr UnitVector3(UnsafeTag, const T& x, const T& y, const T& z) noexcept;

		constexpr void Assert() const;

	private:

		T mX = T{ 0 };
		T mY = T{ 0 };
		T mZ = T{ 1 };
	};

	using UnitVector3f = UnitVector3<float>;
	using UnitVector3d = UnitVector3<double>;

	template<typename T>
	constexpr UnitVector3<T>::UnitVector3(const T& x, const T& y, const T& z)
		: mX(x)
		, mY(y)
		, mZ(z)
	{
		Normalize(mX, mY, mZ);
	}

	template<typename T>
	constexpr UnitVector3<T>::UnitVector3(const Vector3<T>& vector)
		: mX(vector.x)
		, mY(vector.y)
		, mZ(vector.z)
	{
		Normalize(mX, mY, mZ);
	}

	template<typename T>
	constexpr UnitVector3<T>::UnitVector3(UnsafeTag, const T& x, const T& y, const T& z) noexcept
		: mX(x)
		, mY(y)
		, mZ(z)
	{
	}

	template<typename T>
	constexpr const T& UnitVector3<T>::X() const noexcept
	{
		return mX;
	}

	template<typename T>
	constexpr const T& UnitVector3<T>::Y() const noexcept
	{
		return mY;
	}

	template<typename T>
	constexpr const T& UnitVector3<T>::Z() const noexcept
	{
		return mZ;
	}

	template<typename T>
	constexpr void UnitVector3<T>::SetX(const T& x)
	{
		mX = x;
		Assert();
	}

	template<typename T>
	constexpr void UnitVector3<T>::SetY(const T& y)
	{
		mY = y;
		Assert();
	}

	template<typename T>
	constexpr void UnitVector3<T>::SetZ(const T& z)
	{
		mZ = z;
		Assert();
	}

	template<typename T>
	constexpr void UnitVector3<T>::Assert() const
	{
		AssertNormalized(mX, mY, mZ);
	}

	template<typename T>
	constexpr UnitVector3<T> UnitVector3<T>::Right() noexcept
	{
		return UnitVector3<T>(UnsafeTag{}, 1, 0, 0);
	}

	template<typename T>
	constexpr UnitVector3<T> UnitVector3<T>::Up() noexcept
	{
		return UnitVector3<T>(UnsafeTag{}, 0, 1, 0);
	}

	template<typename T>
	constexpr UnitVector3<T> UnitVector3<T>::Forward() noexcept
	{
		return UnitVector3<T>(UnsafeTag{}, 0, 0, 1);
	}

	template<typename T>
	constexpr UnitVector3<T> UnitVector3<T>::Left() noexcept
	{
		return -Right();
	}

	template<typename T>
	constexpr UnitVector3<T> UnitVector3<T>::Down() noexcept
	{
		return -Up();
	}

	template<typename T>
	constexpr UnitVector3<T> UnitVector3<T>::Backward() noexcept
	{
		return -Forward();
	}

	template<typename T>
	[[nodiscard]] constexpr bool operator==(const UnitVector3<T>& a, const UnitVector3<T>& b) noexcept
	{
		return a.X() == b.X() && a.Y() == b.Y() && a.Z() == b.Z();
	}

	template<typename T>
	[[nodiscard]] constexpr Vector3<T> operator+(const UnitVector3<T>& a, const UnitVector3<T>& b) noexcept
	{
		return Vector3<T>(a.X() + b.X(), a.Y() + b.Y(), a.Z() + b.Z());
	}

	template<typename T>
	[[nodiscard]] constexpr Vector3<T> operator+(const Vector3<T>& a, const UnitVector3<T>& b) noexcept
	{
		return Vector3<T>(a.x + b.X(), a.y + b.Y(), a.z + b.Z());
	}

	template<typename T>
	[[nodiscard]] constexpr Vector3<T> operator+(const UnitVector3<T>& a, const Vector3<T>& b) noexcept
	{
		return Vector3<T>(a.X() + b.x, a.Y() + b.y, a.Z() + b.z);
	}

	template<typename T>
	constexpr Vector3<T>& operator+=(Vector3<T>& vectorA, const UnitVector3<T>& vectorB) noexcept
	{
		vectorA = vectorA + vectorB;
		return vectorA;
	}

	template<typename T>
	[[nodiscard]] constexpr Vector3<T> operator-(const UnitVector3<T>& a, const UnitVector3<T>& b) noexcept
	{
		return Vector3<T>(a.X() - b.X(), a.Y() - b.Y(), a.Z() - b.Z());
	}

	template<typename T>
	[[nodiscard]] constexpr Vector3<T> operator-(const Vector3<T>& a, const UnitVector3<T>& b) noexcept
	{
		return Vector3<T>(a.x - b.X(), a.y - b.Y(), a.z - b.Z());
	}

	template<typename T>
	[[nodiscard]] constexpr Vector3<T> operator-(const UnitVector3<T>& a, const Vector3<T>& b) noexcept
	{
		return Vector3<T>(a.X() - b.x, a.Y() - b.y, a.Z() - b.z);
	}

	template<typename T>
	[[nodiscard]] constexpr Vector3<T> operator*(const UnitVector3<T>& vector, const T& scalar) noexcept
	{
		return Vector3<T>(vector.X() * scalar, vector.Y() * scalar, vector.Z() * scalar);
	}

	template<typename T>
	[[nodiscard]] constexpr Vector3<T> operator*(const T& scalar, const UnitVector3<T>& vector) noexcept
	{
		return vector * scalar;
	}

	template<typename T>
	[[nodiscard]] constexpr Vector3<T> operator/(const T& scalar, const UnitVector3<T>& vector)
	{
		return Vector3<T>(scalar / vector.X(), scalar / vector.Y(), scalar / vector.Z());
	}

	template<typename T>
	std::ostream& operator<<(std::ostream& os, const UnitVector3<T>& vector)
	{
		os << "UnitVector3(" << vector.X() << ", " << vector.Y() << ", " << vector.Z() << ")";
		return os;
	}

	template<typename T>
	[[nodiscard]] constexpr bool NearlyEqual(const UnitVector3<T>& a, const UnitVector3<T>& b, const T& tolerance = static_cast<T>(0.0001)) noexcept
	{
		return (Abs(a.X() - b.X()) < tolerance) && (Abs(a.Y() - b.Y()) < tolerance) && (Abs(a.Z() - b.Z()) < tolerance);
	}
}