#include "Graphics/Precompiled/GraphicsPch.hpp"
#include "Graphics/Sprite/Sprite2D.hpp"

namespace Simple
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