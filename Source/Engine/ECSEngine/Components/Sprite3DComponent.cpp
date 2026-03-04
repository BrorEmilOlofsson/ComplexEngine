#include "Engine/Precompiled/EnginePch.hpp"
#include "Sprite3DComponent.hpp"
#include "Engine/Reflection/Reflection.hpp"

namespace CLX
{

	void RegisterSprite3DComponent()
	{
	}

	REGISTER_COMPONENT(Sprite3DComponent);
	REGISTER_MEMBER(Sprite3DComponent::sprite);
	REGISTER_MEMBER(Sprite3D::mColor);
	REGISTER_MEMBER(Sprite3D::mRotation);
	REGISTER_MEMBER(Sprite3D::mTexture);
	REGISTER_MEMBER(Sprite3D::mUVAABB);
	REGISTER_MEMBER(Sprite3D::mPivot);
	REGISTER_MEMBER(Sprite3D::mScale);
}
