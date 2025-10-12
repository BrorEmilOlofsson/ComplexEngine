#pragma once
#include <type_traits>
#include <string_view>

namespace Simple
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
}