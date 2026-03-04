#pragma once
#include "Engine/Math/Angle.hpp"
#include "Engine/Utility/Color.hpp"
#include "Engine/Math/Shapes/AABB2.hpp"
#include "Engine/Asset/TextureAsset.hpp"
//#include <External/DirectXTK/SpriteBatch.h>

namespace Simple
{

	class Sprite2D final
	{
	public:
		
		Sprite2D() = default;

		void SetPosition(const Point2f& position) { mPosition = position; }
		void SetScale(const Vector2f& scale) { mScale = scale; }
		void SetUVAABB(const AABB2i& uvAABB) { mUVAABB = uvAABB; }
		void SetColor(const Color& color) { mColor = color; }
		void SetPivot(const Vector2f& pivot) { mPivot = pivot; }
		void SetTexture(TextureAssetHandle textureAsset);
		void SetRotation(const Radiansf& rotation) { mRotationDegrees = rotation; }
		//void SetEffect(DirectX::SpriteEffects effect) { mEffect = effect; }
		void SetLayerDepth(float layerDepth) { mLayerDepth = layerDepth; }

		[[nodiscard]] const Point2f& GetPosition() const { return mPosition; }
		[[nodiscard]] const Vector2f& GetScale() const { return mScale; }
		[[nodiscard]] const AABB2i& GetUVAABB() const { return mUVAABB; }
		[[nodiscard]] const Color& GetColor() const { return mColor; }
		[[nodiscard]] const Vector2f& GetPivot() const { return mPivot; }
		[[nodiscard]] TextureAssetHandle GetTexture() const;
		[[nodiscard]] const Radiansf& GetRotation() const { return mRotationDegrees; }
		//[[nodiscard]] DirectX::SpriteEffects GetEffect() const { return mEffect; }
		[[nodiscard]] float GetLayerDepth() const { return mLayerDepth; }

	public:

		Point2f mPosition;
		Vector2f mScale = Vector2f::One();
		AABB2i mUVAABB = AABB2i::FromDefaultAndExtent(Vector2i(512, 512));
		Color mColor = Colors::White;
		Vector2f mPivot;
		TextureAssetHandle mTexture;
		Radiansf mRotationDegrees;
		//DirectX::SpriteEffects mEffect = DirectX::SpriteEffects_None;
		float mLayerDepth = 0.0f;
	};
}