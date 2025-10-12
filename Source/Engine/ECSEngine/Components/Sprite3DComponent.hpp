#pragma once
#include "Graphics/Sprite/Sprite3D.hpp"

namespace Simple
{

	struct Sprite3DComponent final
	{
		Sprite3D sprite;
	};

	void RegisterSprite3DComponent();
}