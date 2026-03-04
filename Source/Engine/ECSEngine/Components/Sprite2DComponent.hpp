#pragma once
#include "Engine/Graphics/Sprite/Sprite2D.hpp"

namespace CLX
{
	struct Sprite2DComponent final
	{
		Sprite2D sprite;
	};

	void RegisterSprite2DComponent();

}