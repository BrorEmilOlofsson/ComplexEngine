#pragma once
#include "Utility/Asset/EntityCompositionAsset.hpp"

namespace Simple
{
	struct EntityCompositionComponent final
	{
		EntityCompositionAssetHandle asset;
	};

	void RegisterEntityCompositionComponent();
}