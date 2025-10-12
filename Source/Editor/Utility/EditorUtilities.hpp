#pragma once
#include <concepts>
#include <limits>

namespace Simple
{
	template<std::integral T>
	[[nodiscard]] constexpr T GetInvalidIndex()
	{
		return std::numeric_limits<T>::max();
	}
}