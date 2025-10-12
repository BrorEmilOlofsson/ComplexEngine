#pragma once
#include <filesystem>
#include "Utility/StrongType.hpp"
#include "Utility/FixedString.hpp"

namespace Simple
{
	template<typename Tag>
	using AssetPath = StrongType<FixedString<256>, Tag>;

	using AssetPath_OBJ = AssetPath<struct Tag_OBJ>;
	
}