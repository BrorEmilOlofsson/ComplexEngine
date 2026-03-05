#pragma once
#include "Engine/Asset/AssetTypes/EntityCompositionAsset.hpp"

namespace CLX
{
	struct EntityCompositionComponent final
	{
		EntityCompositionAssetHandle asset;
	};

	void RegisterEntityCompositionComponent();
}