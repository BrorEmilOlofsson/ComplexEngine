#pragma once
#include <type_traits>
#include <string_view>

namespace CLX
{

	template<typename T>
	struct EnumCount;

	template<typename T> requires std::is_enum_v<T>
	struct EnumIterator
	{
		std::string_view name;
		std::size_t index;
		T value;
	};

	template<typename T, typename Func> requires std::is_enum_v<T>
	void ForEachEnum(Func&& aFunc)
	{
		for (size_t i = 0; i < EnumCount<T>::value; i++)
		{
			const T enumValue = static_cast<T>(i);
			aFunc(EnumIterator{ EnumToString(enumValue), i, enumValue });
		}
	}

    template<typename T>
	struct IsEnumMask : std::false_type
	{
	};

	template<typename T>
    constexpr bool IsEnumMaskV = IsEnumMask<T>::value;

	template<typename T>
    concept EnumMaskable = std::is_enum_v<T> && std::is_unsigned_v<std::underlying_type_t<T>> && IsEnumMaskV<T>;


	template<EnumMaskable T>
	[[nodiscard]] constexpr T operator|(const T lhs, const T rhs) noexcept
	{
		using UnderlyingType = std::underlying_type_t<T>;
		return static_cast<T>(static_cast<UnderlyingType>(lhs) | static_cast<UnderlyingType>(rhs));
	}

	template<EnumMaskable T>
	[[nodiscard]] constexpr T operator&(const T lhs, const T rhs) noexcept
	{
		using UnderlyingType = std::underlying_type_t<T>;
		return static_cast<T>(static_cast<UnderlyingType>(lhs) & static_cast<UnderlyingType>(rhs));
    }

	template<EnumMaskable T>
	[[nodiscard]] constexpr T operator^(const T lhs, const T rhs) noexcept
	{
		using UnderlyingType = std::underlying_type_t<T>;
		return static_cast<T>(static_cast<UnderlyingType>(lhs) ^ static_cast<UnderlyingType>(rhs));
    }

	template<EnumMaskable T>
	[[nodiscard]] constexpr T operator~(const T value) noexcept
	{
		using UnderlyingType = std::underlying_type_t<T>;
		return static_cast<T>(~static_cast<UnderlyingType>(value));
    }

	template<EnumMaskable T>
	[[nodiscard]] constexpr T& operator|=(T& lhs, const T rhs) noexcept
	{
		lhs = lhs | rhs;
		return lhs;
    }

	template<EnumMaskable T>
	[[nodiscard]] constexpr T& operator&=(T& lhs, const T rhs) noexcept
	{
		lhs = lhs & rhs;
		return lhs;
	}

	template<EnumMaskable T>
	[[nodiscard]] constexpr bool HasAnyFlag(const T value, const T flag) noexcept
	{
		return (value & flag) != static_cast<T>(0);
    }

	template<EnumMaskable T>
	[[nodiscard]] constexpr bool HasFlag(const T value, const T flag) noexcept
	{
		return (value & flag) == flag;
	}
}