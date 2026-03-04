#include "Engine/Precompiled/EnginePch.hpp"
#include "Sprite2DComponent.hpp"
#include "Engine/Reflection/Reflection.hpp"

namespace CLX
{

	void RegisterSprite2DComponent() {}

	REGISTER_COMPONENT(Sprite2DComponent);

	REGISTER_MEMBER(Sprite2D::mPosition);
	REGISTER_MEMBER(Sprite2D::mRotationDegrees);
	REGISTER_MEMBER(Sprite2D::mUVAABB);
	REGISTER_MEMBER(Sprite2D::mScale);
	REGISTER_MEMBER(Sprite2D::mPivot);
	REGISTER_MEMBER(Sprite2D::mTexture);
	REGISTER_MEMBER(Sprite2D::mColor);
	REGISTER_MEMBER(Sprite2DComponent::sprite);
}