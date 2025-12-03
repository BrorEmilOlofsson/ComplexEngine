#pragma once
#include "Utility/Math/Transform3.hpp"
#include "Utility/Math/Angle.hpp"
#include "Utility/Color.hpp"
#include "Utility/Shapes/AABB2.hpp"
#include "Utility/Asset/TextureAsset.hpp"
//#include <External/DirectXTK/SpriteBatch.h>

namespace Simple
{

	class Sprite3D final
	{
	public:

		Sprite3D() = default;

	public:

		Transform mTransform;
		Vector2f mScale = Vector2f::One();
		AABB2i mUVAABB = AABB2i::FromDefaultAndExtent(Vector2i(512, 512));
		Color mColor = Colors::White;
		Vector2f mPivot;
		TextureAssetHandle mTexture;
		Radiansf mRotation;
		//DirectX::SpriteEffects mEffect = DirectX::SpriteEffects_None;
		float mLayerDepth = 0.0f;
	};
}