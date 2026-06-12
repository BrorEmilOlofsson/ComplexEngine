#pragma once
#include "Engine/Asset/AssetTypes/TextureAsset.hpp"

namespace CLX
{

    class Material final
    {
    public:

        TextureAssetHandle albedoTexture;
        TextureAssetHandle normalTexture;
        TextureAssetHandle metallicTexture;
    };
}