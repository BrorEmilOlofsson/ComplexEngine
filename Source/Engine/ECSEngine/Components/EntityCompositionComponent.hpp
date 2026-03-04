#pragma once
#include "Engine/Asset/EntityCompositionAsset.hpp"

namespace CLX
{
	struct EntityCompositionComponent final
	{
		EntityCompositionAssetHandle asset;
	};

	void RegisterEntityCompositionComponent();
}