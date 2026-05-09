#pragma once
#include <type_traits>
#include "Engine/Utility/EnumUtility.hpp"

namespace CLX
{
	enum class eTransformMode : uint8_t
	{
		Local,
		World
	};

	constexpr std::string_view ToString(eTransformMode transformMode) noexcept
	{
		switch (transformMode)
		{
		case eTransformMode::Local:
			return "Local";
			break;
		case eTransformMode::World:
			return "World";
			break;
		default:
			return "None";
			break;
		}
	}

	template<>
	struct EnumCount<eTransformMode> : std::integral_constant<std::underlying_type_t<eTransformMode>, 2> {};
}