#pragma once
#include "Engine/Asset/EntityCompositionAsset.hpp"

namespace Simple
{
	struct EntityCompositionComponent final
	{
		EntityCompositionAssetHandle asset;
	};

	void RegisterEntityCompositionComponent();
}