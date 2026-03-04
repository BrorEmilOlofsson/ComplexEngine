#pragma once
#include "Engine/Graphics/Sprite/Sprite2D.hpp"

namespace Simple
{
	struct Sprite2DComponent final
	{
		Sprite2D sprite;
	};

	void RegisterSprite2DComponent();

}