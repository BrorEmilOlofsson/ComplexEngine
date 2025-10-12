#pragma once
#include <cassert>
#include <exception>

namespace Simple
{

	template<typename BoundsChecker, typename T>
	constexpr void AssertBounds(const T& min, const T& max)
	{
		if (!BoundsChecker{}(min, max))
		{
			throw std::runtime_error("Bounds assertion failed: Min must be less than or equal to Max.");
		}
	}

	template<typename T, typename BoundsChecker = std::less_equal<T>>
	class Bounds final
	{
		template<typename T>
		struct ScalarTypeResolve;

		template<typename T> requires std::is_arithmetic_v<T>
		struct ScalarTypeResolve<T>
		{
			using type = T;
		};

		template<typename T> requires (!std::is_arithmetic_v<T>)
		struct ScalarTypeResolve<T>
		{
			using type = typename T::value_type;
		};

		using ExtentType = decltype(T() - T());
		using ScalarType = ScalarTypeResolve<T>::type;
		
		constexpr Bounds(const T& min, const T& max);

	public:

		constexpr Bounds() = default;

		constexpr void SetCenterAndExtent(const T& center, const ExtentType& extent);
		constexpr void SetCenter(const T& center);
		constexpr void SetExtent(const ExtentType& extent);
		constexpr void SetMin(const T& min);
		constexpr void SetMax(const T& max);
		constexpr void SetMinMax(const T& min, const T& max);
		constexpr void SetMinAndExtent(const T& min, const ExtentType& extent);

		[[nodiscard]] constexpr const T& GetMin() const noexcept;
		[[nodiscard]] constexpr const T& GetMax() const noexcept;
		[[nodiscard]] constexpr T GetCenter() const noexcept;
		[[nodiscard]] constexpr ExtentType GetExtent() const noexcept;

		
	private:

		constexpr void Assert();

	public:

		[[nodiscard]] static constexpr Bounds CreateFromMinAndExtent(const T& min, const ExtentType& extent);
		[[nodiscard]] static constexpr Bounds CreateFromCenterAndExtent(const T& center, const ExtentType& extent);
		[[nodiscard]] static constexpr Bounds CreateFromMinAndMax(const T& min, const T& max);

	private:

		T mMin{};
		T mMax{};
	};

	template<typename T, typename BoundsChecker>
	constexpr Bounds<T, BoundsChecker>::Bounds(const T& min, const T& max)
		: mMin(min)
		, mMax(max)
	{
		Assert();
	}

	template<typename T, typename BoundsChecker>
	constexpr void Bounds<T, BoundsChecker>::SetCenterAndExtent(const T& aCenter, const ExtentType& extent)
	{
		mMin = aCenter - extent / static_cast<ScalarType>(2);
		mMax = aCenter + extent / static_cast<ScalarType>(2);

		Assert();
	}

	template<typename T, typename BoundsChecker>
	constexpr void Bounds<T, BoundsChecker>::SetCenter(const T& center)
	{
		SetCenterAndExtent(center, GetExtent());
	}

	template<typename T, typename BoundsChecker>
	constexpr void Bounds<T, BoundsChecker>::SetExtent(const ExtentType& extent)
	{
		SetCenterAndExtent(GetCenter(), extent);
	}

	template<typename T, typename BoundsChecker>
	constexpr void Bounds<T, BoundsChecker>::SetMin(const T& min)
	{
		mMin = min;
		Assert();
	}

	template<typename T, typename BoundsChecker>
	constexpr void Bounds<T, BoundsChecker>::SetMax(const T& max)
	{
		mMax = max;
		Assert();
	}

	template<typename T, typename BoundsChecker>
	constexpr void Bounds<T, BoundsChecker>::SetMinMax(const T& min, const T& max)
	{
		mMin = min;
		mMax = max;
		Assert();
	}

	template<typename T, typename BoundsChecker>
	constexpr void Bounds<T, BoundsChecker>::SetMinAndExtent(const T& min, const ExtentType& aExtent)
	{
		mMin = min;
		mMax = min + aExtent;
		Assert();
	}

	template<typename T, typename BoundsChecker>
	constexpr const T& Bounds<T, BoundsChecker>::GetMin() const noexcept
	{
		return mMin;
	}

	template<typename T, typename BoundsChecker>
	constexpr const T& Bounds<T, BoundsChecker>::GetMax() const noexcept
	{
		return mMax;
	}

	template<typename T, typename BoundsChecker>
	constexpr T Bounds<T, BoundsChecker>::GetCenter() const noexcept
	{
		return mMin + GetExtent() / static_cast<ScalarType>(2);
	}

	template<typename T, typename BoundsChecker>
	constexpr typename Bounds<T, BoundsChecker>::ExtentType Bounds<T, BoundsChecker>::GetExtent() const noexcept
	{
		return mMax - mMin;
	}

	template<typename T, typename BoundsChecker>
	constexpr void Bounds<T, BoundsChecker>::Assert()
	{
		AssertBounds<BoundsChecker>(mMin, mMax);
	}

	template<typename T, typename BoundsChecker>
	constexpr Bounds<T, BoundsChecker> Bounds<T, BoundsChecker>::CreateFromMinAndExtent(const T& min, const ExtentType& extent)
	{
		return Bounds<T, BoundsChecker>(min, min + extent);
	}

	template<typename T, typename BoundsChecker>
	constexpr Bounds<T, BoundsChecker> Bounds<T, BoundsChecker>::CreateFromCenterAndExtent(const T& center, const ExtentType& extent)
	{
		const auto halfExtent = extent / static_cast<ScalarType>(2);
		return Bounds<T, BoundsChecker>(center - halfExtent, center + halfExtent);
	}

	template<typename T, typename BoundsChecker>
	constexpr Bounds<T, BoundsChecker> Bounds<T, BoundsChecker>::CreateFromMinAndMax(const T& min, const T& max)
	{
		return Bounds<T, BoundsChecker>(min, max);
	}
}