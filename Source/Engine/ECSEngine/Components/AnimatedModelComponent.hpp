#pragma once
#include "Utility/Asset/AnimatedModelAsset.hpp"

namespace Simple
{

	struct AnimatedModelComponent
	{
		AnimatedModelAssetHandle modelHandle;
	};

	void RegisterAnimatedModelComponent();
}