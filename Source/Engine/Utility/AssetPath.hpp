#pragma once
#include <filesystem>
#include "Engine/Utility/StrongType.hpp"
#include "Engine/Utility/FixedString.hpp"

namespace Simple
{
	template<typename Tag>
	using AssetPath = StrongType<FixedString<256>, Tag>;

	using AssetPath_OBJ = AssetPath<struct Tag_OBJ>;
	
}