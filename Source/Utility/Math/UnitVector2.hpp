#pragma once
#include "Utility/Math/Vector2.hpp"
#include "Utility/Math/Math.hpp"

namespace Simple
{

	template<typename T>
	class UnitVector2 final
	{
	public:
		
		using value_type = T;

		constexpr UnitVector2() = default;
		constexpr UnitVector2(const T& aX, const T& aY);
		constexpr explicit UnitVector2(const Vector2<T>& aVector);

		[[nodiscard]] constexpr const T& X() const noexcept;
		[[nodiscard]] constexpr const T& Y() const noexcept;

		constexpr void SetX(const T& aX);
		constexpr void SetY(const T& aY);

		[[nodiscard]] static constexpr UnitVector2<T> Right();
		[[nodiscard]] static constexpr UnitVector2<T> Up();
		[[nodiscard]] static constexpr UnitVector2<T> Left();
		[[nodiscard]] static constexpr UnitVector2<T> Down();

	private:

		constexpr void Assert() const;

	private:

		T mX = T{ 0 };
		T mY = T{ 1 };
	};

	using UnitVector2f = UnitVector2<float>;
	using UnitVector2d = UnitVector2<double>;

	template<typename T>
	constexpr UnitVector2<T>::UnitVector2(const T& aX, const T& aY)
		: mX(aX)
		, mY(aY)
	{
		Normalize(mX, mY);
	}

	template<typename T>
	constexpr UnitVector2<T>::UnitVector2(const Vector2<T>& aVector)
		: mX(aVector.x)
		, mY(aVector.y)
	{
		Normalize(mX, mY);
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
	constexpr void UnitVector2<T>::SetX(const T& aX)
	{
		mX = aX;
		Assert();
	}

	template<typename T>
	constexpr void UnitVector2<T>::SetY(const T& aY)
	{
		mY = aY;
		Assert();
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
	[[nodiscard]] constexpr bool operator==(const UnitVector2<T>& aA, const UnitVector2<T>& aB) noexcept
	{
		return aA.X() == aB.X() && aA.Y() == aB.Y();
	}

	template<typename T>
	[[nodiscard]] constexpr Vector2<T> operator+(const UnitVector2<T>& aA, const UnitVector2<T>& aB) noexcept
	{
		return Vector2<T>(aA.X() + aB.X(), aA.Y() + aB.Y());
	}

	template<typename T>
	[[nodiscard]] constexpr Vector2<T> operator+(const Vector2<T>& aA, const UnitVector2<T>& aB) noexcept
	{
		return Vector2<T>(aA.z + aB.X(), aA.y + aB.Y());
	}

	template<typename T>
	[[nodiscard]] constexpr Vector2<T> operator+(const UnitVector2<T>& aA, const Vector2<T>& aB) noexcept
	{
		return Vector2<T>(aA.X() + aB.x, aA.Y() + aB.y);
	}

	template<typename T>
	[[nodiscard]] constexpr Vector2<T> operator-(const UnitVector2<T>& aA, const UnitVector2<T>& aB) noexcept
	{
		return Vector2<T>(aA.X() - aB.X(), aA.Y() - aB.Y());
	}

	template<typename T>
	[[nodiscard]] constexpr Vector2<T> operator-(const Vector2<T>& aA, const UnitVector2<T>& aB) noexcept
	{
		return Vector2<T>(aA.x - aB.X(), aA.y - aB.Y());
	}

	template<typename T>
	[[nodiscard]] constexpr Vector2<T> operator-(const UnitVector2<T>& aA, const Vector2<T>& aB) noexcept
	{
		return Vector2<T>(aA.X() - aB.x, aA.Y() - aB.y);
	}

	template<typename T>
	[[nodiscard]] constexpr UnitVector2<T> operator-(const UnitVector2<T>& aVector)
	{
		return UnitVector2<T>(-aVector.X(), -aVector.Y());
	}

	template<typename T>
	[[nodiscard]] constexpr Vector2<T> operator*(const UnitVector2<T>& aVector, const T& aScalar) noexcept
	{
		return Vector2<T>(aVector.X() * aScalar, aVector.Y() * aScalar);
	}

	template<typename T>
	[[nodiscard]] constexpr Vector2<T> operator*(const T& aScalar, const UnitVector2<T>& aVector) noexcept
	{
		return aVector * aScalar;
	}

	template<typename T>
	[[nodiscard]] constexpr Vector2<T> operator/(const T& aScalar, const UnitVector2<T>& aVector)
	{
		return Vector2<T>(aScalar / aVector.X(), aScalar / aVector.Y());
	}

	template<typename T>
	std::ostream& operator<<(std::ostream& aOS, const UnitVector2<T>& aVector)
	{
		aOS << "UnitVector2(" << aVector.X() << ", " << aVector.Y() << ")";
		return aOS;
	}

	template<typename T>
	[[nodiscard]] constexpr bool NearlyEqual(const UnitVector2<T>& aA, const UnitVector2<T>& aB, const T& aTolerance = static_cast<T>(0.0001)) noexcept
	{
		return (Abs(aA.X() - aB.X()) < aTolerance) && (Abs(aA.Y() - aB.Y()) < aTolerance);
	}
}