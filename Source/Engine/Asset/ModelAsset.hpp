#pragma once
#include "Engine/Asset/Asset.hpp"
#include "Engine/Asset/AssetHandle.hpp"

namespace Simple
{
	class Model;

	using ModelAsset = Asset<Model>;
	using ModelAssetHandle = AssetHandle<Model>;
}