#pragma once
#include "Engine/Asset/Asset.hpp"
#include "Engine/Asset/AssetHandle.hpp"

#include "Engine/Graphics/Model/Model.hpp"

namespace CLX
{
	using ModelAsset = Asset<Model>;
	using ModelAssetHandle = AssetHandle<Model>;
}