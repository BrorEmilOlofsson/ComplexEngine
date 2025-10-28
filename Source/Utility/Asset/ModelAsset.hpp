#pragma once
#include "Utility/Asset/Asset.hpp"
#include "Utility/Asset/AssetHandle.hpp"

namespace Simple
{
	class Model;

	using ModelAsset = Asset<Model>;
	using ModelAssetHandle = AssetHandle<Model>;
}