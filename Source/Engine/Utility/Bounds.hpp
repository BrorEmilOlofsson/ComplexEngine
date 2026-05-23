#pragma once
#include <type_traits>
#include <concepts>
#include <format>
#include <string>
#include "Engine/Utility/Assert.hpp"
#include "Engine/Utility/Rebind.hpp"

namespace CLX
{

	template<typename BoundsChecker, typename T>
	constexpr void AssertBounds([[maybe_unused]] const T& min, [[maybe_unused]] const T& max)
	{
		ASSERT(BoundsChecker{}(min, max));
	}

	template<typename T>
	concept HasValueType = requires
	{
		typename T::value_type;
	};

	template<typename T>
	struct make_extent_scalar
	{
		using type = T;
	};

	template<std::signed_integral T>
	struct make_extent_scalar<T>
	{
		using type = std::make_unsigned_t<T>;
	};

	template<typename T>
	using make_extent_scalar_t = typename make_extent_scalar<T>::type;

	template<typename T>
	struct make_extent
	{
		using type = make_extent_scalar_t<T>;
	};

	template<typename T>
		requires HasValueType<T>
	struct make_extent<T>
	{
		using type = rebind_t<T, make_extent_scalar_t<typename T::value_type>>;
	};

	template<typename T>
	using make_extent_t = typename make_extent<T>::type;

	template<typename T, typename BoundsChecker = std::less_equal<T>>
	class Bounds final
	{
		template<typename T>
		struct scalar_resolve;

		template<typename T> requires std::is_arithmetic_v<T>
		struct scalar_resolve<T>
		{
			using type = T;
		};

		template<typename T> requires (!std::is_arithmetic_v<T>)
		struct scalar_resolve<T>
		{
			using type = typename T::value_type;
		};

		template<typename U>
		struct extent_resolve
		{
			using difference_type = decltype(U{} - U{});
			using type = make_extent_t<difference_type>;
		};

		constexpr Bounds(const T& min, const T& max);

	public:

		using extent_t = extent_resolve<T>::type;
		using scalar_t = scalar_resolve<T>::type;

		constexpr Bounds() = default;

		constexpr void SetCenterAndExtent(const T& center, const extent_t& extent);
		constexpr void SetCenter(const T& center);
		constexpr void SetExtent(const extent_t& extent);
		constexpr void SetMin(const T& min);
		constexpr void SetMax(const T& max);
		constexpr void SetMinAndMax(const T& min, const T& max);
		constexpr void SetMinAndExtent(const T& min, const extent_t& extent);

		[[nodiscard]] constexpr const T& GetMin() const noexcept;
		[[nodiscard]] constexpr const T& GetMax() const noexcept;
		[[nodiscard]] constexpr T GetCenter() const noexcept;
		[[nodiscard]] constexpr extent_t GetExtent() const noexcept;

	private:

		constexpr void Assert();

	public:

		[[nodiscard]] static constexpr Bounds FromMinAndExtent(const T& min, const extent_t& extent);
		[[nodiscard]] static constexpr Bounds FromDefaultAndExtent(const extent_t& extent);
		[[nodiscard]] static constexpr Bounds FromCenterAndExtent(const T& center, const extent_t& extent);
		[[nodiscard]] static constexpr Bounds FromMinAndMax(const T& min, const T& max);

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
	constexpr void Bounds<T, BoundsChecker>::SetCenterAndExtent(const T& center, const extent_t& extent)
	{
		mMin = center - extent / static_cast<scalar_t>(2);
		mMax = center + extent / static_cast<scalar_t>(2);

		Assert();
	}

	template<typename T, typename BoundsChecker>
	constexpr void Bounds<T, BoundsChecker>::SetCenter(const T& center)
	{
		SetCenterAndExtent(center, GetExtent());
	}

	template<typename T, typename BoundsChecker>
	constexpr void Bounds<T, BoundsChecker>::SetExtent(const extent_t& extent)
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
	constexpr void Bounds<T, BoundsChecker>::SetMinAndMax(const T& min, const T& max)
	{
		mMin = min;
		mMax = max;
		Assert();
	}

	template<typename T, typename BoundsChecker>
	constexpr void Bounds<T, BoundsChecker>::SetMinAndExtent(const T& min, const extent_t& extent)
	{
		mMin = min;
		mMax = min + extent;
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
		return mMin + GetExtent() / static_cast<scalar_t>(2);
	}

	template<typename T, typename BoundsChecker>
	constexpr typename Bounds<T, BoundsChecker>::extent_t Bounds<T, BoundsChecker>::GetExtent() const noexcept
	{
		return static_cast<extent_t>(mMax - mMin);
	}

	template<typename T, typename BoundsChecker>
	constexpr void Bounds<T, BoundsChecker>::Assert()
	{
		AssertBounds<BoundsChecker>(mMin, mMax);
	}

	template<typename T, typename BoundsChecker>
	[[nodiscard]] constexpr Bounds<T, BoundsChecker> Bounds<T, BoundsChecker>::FromMinAndExtent(const T& min, const extent_t& extent)
	{
		return Bounds<T, BoundsChecker>(min, min + extent);
	}

	template<typename T, typename BoundsChecker>
	[[nodiscard]] constexpr Bounds<T, BoundsChecker> Bounds<T, BoundsChecker>::FromDefaultAndExtent(const extent_t& extent)
	{
		constexpr T d = T{};
		return Bounds<T, BoundsChecker>(d, d + extent);
	}

	template<typename T, typename BoundsChecker>
	[[nodiscard]] constexpr Bounds<T, BoundsChecker> Bounds<T, BoundsChecker>::FromCenterAndExtent(const T& center, const extent_t& extent)
	{
		const auto halfExtent = extent / static_cast<scalar_t>(2);
		return Bounds<T, BoundsChecker>(center - halfExtent, center + halfExtent);
	}

	template<typename T, typename BoundsChecker>
	[[nodiscard]] constexpr Bounds<T, BoundsChecker> Bounds<T, BoundsChecker>::FromMinAndMax(const T& min, const T& max)
	{
		return Bounds<T, BoundsChecker>(min, max);
	}

	template<typename T, typename BoundsChecker> requires std::equality_comparable<T>
	[[nodiscard]] constexpr bool operator==(const Bounds<T, BoundsChecker>& a, const Bounds<T, BoundsChecker>& b)
	{
		return a.GetMin() == b.GetMin() && a.GetMax() == b.GetMax();
	}
}

template<typename T, typename BoundsChecker>
struct std::formatter<CLX::Bounds<T, BoundsChecker>> : std::formatter<std::string>
{
	auto format(const CLX::Bounds<T, BoundsChecker>& bounds, auto& ctx) const
	{
		return std::format_to(
			ctx.out(),
			"AABB2(Min: {}, Max: {})",
			bounds.GetMin(), bounds.GetMax()
		);
	}
};