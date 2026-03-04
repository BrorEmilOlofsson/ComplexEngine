#include "Engine/Precompiled/EnginePch.hpp"
#include "Engine/Graphics/Sprite/Sprite2D.hpp"

namespace CLX
{
	void Sprite2D::SetTexture(TextureAssetHandle texture)
	{
		mTexture = texture;
	}

	TextureAssetHandle Sprite2D::GetTexture() const
	{
		return mTexture;
	}
}