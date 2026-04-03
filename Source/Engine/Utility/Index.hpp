#pragma once
#include <cstddef>
#include <variant>

namespace CLX
{
	struct LastIndex {};
	struct FirstIndex {};
	struct Index { std::size_t mIndex = 0; };
	using ChildIndexSetting = std::variant<Index, FirstIndex, LastIndex>;
}