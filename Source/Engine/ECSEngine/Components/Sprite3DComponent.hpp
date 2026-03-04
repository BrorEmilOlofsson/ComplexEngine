#pragma once
#include "Engine/Graphics/Sprite/Sprite3D.hpp"

namespace CLX
{

	struct Sprite3DComponent final
	{
		Sprite3D sprite;
	};

	void RegisterSprite3DComponent();
}