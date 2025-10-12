#pragma once
#include "Graphics/Sprite/Sprite2D.hpp"

namespace Simple
{
	struct Sprite2DComponent final
	{
		Sprite2D sprite;
	};

	void RegisterSprite2DComponent();

}