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


	template<std::size_t Size>
	void CopyString(char(&buffer)[Size], const std::string& source)
	{
		strncpy_s(buffer, source.c_str(), sizeof(buffer));
		buffer[sizeof(buffer) - 1] = '\0';
	}
}