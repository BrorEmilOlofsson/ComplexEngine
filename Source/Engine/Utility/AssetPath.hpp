#pragma once
#include "Engine/Utility/StrongType.hpp"
#include "Engine/Utility/FixedString.hpp"
#include "Engine/Asset/AssetTypes/AssetTypes.hpp"
#include "Engine/Asset/AssetExtensions.hpp"

namespace CLX
{
	template<typename Tag>
	using AssetPath = StrongType<FixedString<256>, Tag>;

	using AssetPath_OBJ = AssetPath<struct Tag_OBJ>;
    using AssetPath_EntityComposition = AssetPath<struct Tag_EntityComposition>;

	template<typename T>
	struct AssetPathMapper;

    template<>
    struct AssetPathMapper<AssetPath_OBJ>
    {
        static constexpr std::wstring_view extension = L".obj";
        //using AssetType = ;
    };

    template<>
    struct AssetPathMapper<AssetPath_EntityComposition>
    {
        static constexpr std::wstring_view extension = AssetExtensions::EntityComposition;
        using AssetType = EntityCompositionAsset;
        using AssetHandleType = EntityCompositionAssetHandle;
    };

	
}