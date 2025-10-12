#pragma once
#include <limits>

namespace Simple
{

	template<typename T>
	[[nodiscard]] constexpr T GetInvalidIndex()
	{
		return std::numeric_limits<T>::max();
	}
}
