#pragma once
#include <limits>

namespace CLX
{

	template<typename T>
	[[nodiscard]] constexpr T GetInvalidIndex()
	{
		return std::numeric_limits<T>::max();
	}
}
