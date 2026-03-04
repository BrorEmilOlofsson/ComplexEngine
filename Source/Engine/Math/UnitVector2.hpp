#pragma once
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Math.hpp"
#include "Engine/Utility/ValueType.hpp"
#include <iostream>

namespace Simple
{

	/// <summary>
	/// A 2D unit vector class that always maintains a length of 1.
	/// </summary>
	template<typename T>
	class UnitVector2 final
	{
	public:
		
		using value_type = T;

		constexpr UnitVector2() = default;
		constexpr UnitVector2(const T& x, const T& y);
		constexpr explicit UnitVector2(const Vector2<T>& vector);

		[[nodiscard]] constexpr const T& X() const noexcept;
		[[nodiscard]] constexpr const T& Y() const noexcept;

		constexpr void SetX(const T& x);
		constexpr void SetY(const T& y);

		[[nodiscard]] static constexpr UnitVector2<T> Right();
		[[nodiscard]] static constexpr UnitVector2<T> Up();
		[[nodiscard]] static constexpr UnitVector2<T> Left();
		[[nodiscard]] static constexpr UnitVector2<T> Down();

		[[nodiscard]] friend constexpr UnitVector2<T> operator-(const UnitVector2<T>& vector) noexcept
		{
			return UnitVector2<T>(UnsafeTag{}, -vector.mX, -vector.mY);
		}

	private:

		struct UnsafeTag {};

		constexpr UnitVector2(UnsafeTag, const T& x, const T& y);

		constexpr void Assert() const;

	private:

		T mX = T{ 0 };
		T mY = T{ 1 };
	};

	using UnitVector2f = UnitVector2<float>;
	using UnitVector2d = UnitVector2<double>;

	template<typename T>
	struct ValueType<UnitVector2<T>>
	{
		using type = T;
    };

	template<typename T>
	constexpr UnitVector2<T>::UnitVector2(const T& x, const T& y)
		: mX(x)
		, mY(y)
	{
		Normalize(mX, mY);
	}

	template<typename T>
	constexpr UnitVector2<T>::UnitVector2(const Vector2<T>& vector)
		: mX(vector.x)
		, mY(vector.y)
	{
		Normalize(mX, mY);
	}

	template<typename T>
	constexpr UnitVector2<T>::UnitVector2(UnsafeTag, const T& x, const T& y) 
		: mX(x)
		, mY(y)
	{
	}

	template<typename T>
	constexpr const T& UnitVector2<T>::X() const noexcept
	{
		return mX;
	}

	template<typename T>
	constexpr const T& UnitVector2<T>::Y() const noexcept
	{
		return mY;
	}

	template<typename T>
	constexpr void UnitVector2<T>::SetX(const T& x)
	{
		mX = x;
		Normalize(mX, mY);
	}

	template<typename T>
	constexpr void UnitVector2<T>::SetY(const T& y)
	{
		mY = y;
		Normalize(mX, mY);
	}

	template<typename T>
	constexpr void UnitVector2<T>::Assert() const
	{
		AssertNormalized(mX, mY);
	}

	template<typename T>
	constexpr UnitVector2<T> UnitVector2<T>::Right()
	{
		return UnitVector2<T>(1, 0);
	}

	template<typename T>
	constexpr UnitVector2<T> UnitVector2<T>::Up()
	{
		return UnitVector2<T>(0, 1);
	}

	template<typename T>
	constexpr UnitVector2<T> UnitVector2<T>::Left()
	{
		return -Right();
	}

	template<typename T>
	constexpr UnitVector2<T> UnitVector2<T>::Down()
	{
		return -Up();
	}

	template<typename T>
	[[nodiscard]] constexpr bool operator==(const UnitVector2<T>& a, const UnitVector2<T>& b) noexcept
	{
		return a.X() == b.X() && a.Y() == b.Y();
	}

	template<typename T>
	[[nodiscard]] constexpr Vector2<T> operator+(const UnitVector2<T>& a, const UnitVector2<T>& b) noexcept
	{
		return Vector2<T>(a.X() + b.X(), a.Y() + b.Y());
	}

	template<typename T>
	[[nodiscard]] constexpr Vector2<T> operator+(const Vector2<T>& a, const UnitVector2<T>& b) noexcept
	{
		return Vector2<T>(a.x + b.X(), a.y + b.Y());
	}

	template<typename T>
	[[nodiscard]] constexpr Vector2<T> operator+(const UnitVector2<T>& a, const Vector2<T>& b) noexcept
	{
		return Vector2<T>(a.X() + b.x, a.Y() + b.y);
	}

	template<typename T>
	[[nodiscard]] constexpr Vector2<T> operator-(const UnitVector2<T>& a, const UnitVector2<T>& b) noexcept
	{
		return Vector2<T>(a.X() - b.X(), a.Y() - b.Y());
	}

	template<typename T>
	[[nodiscard]] constexpr Vector2<T> operator-(const Vector2<T>& a, const UnitVector2<T>& b) noexcept
	{
		return Vector2<T>(a.x - b.X(), a.y - b.Y());
	}

	template<typename T>
	[[nodiscard]] constexpr Vector2<T> operator-(const UnitVector2<T>& a, const Vector2<T>& b) noexcept
	{
		return Vector2<T>(a.X() - b.x, a.Y() - b.y);
	}

	template<typename T>
	[[nodiscard]] constexpr Vector2<T> operator*(const UnitVector2<T>& vector, const T& scalar) noexcept
	{
		return Vector2<T>(vector.X() * scalar, vector.Y() * scalar);
	}

	template<typename T>
	[[nodiscard]] constexpr Vector2<T> operator*(const T& scalar, const UnitVector2<T>& vector) noexcept
	{
		return vector * scalar;
	}

	template<typename T>
	[[nodiscard]] constexpr Vector2<T> operator/(const UnitVector2<T>& vector, const T& scalar)
	{
		return Vector2<T>(vector.X() / scalar, vector.Y() / scalar);
	}

	template<typename T>
	std::ostream& operator<<(std::ostream& os, const UnitVector2<T>& vector)
	{
		return os << "UnitVector2(" << vector.X() << ", " << vector.Y() << ")";
	}

	template<typename T>
	[[nodiscard]] constexpr bool NearlyEqual(const UnitVector2<T>& a, const UnitVector2<T>& b, const Tolerance<T>& tolerance = Tolerance<T>(static_cast<T>(1e-5))) noexcept
	{
        return NearlyEqual(a.X(), b.X(), tolerance) 
			&& NearlyEqual(a.Y(), b.Y(), tolerance);
	}
}