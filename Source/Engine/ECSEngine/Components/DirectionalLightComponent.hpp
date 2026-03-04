#pragma once
#include "Engine/Graphics/Light/DirectionalLight.hpp"

namespace CLX
{
	struct DirectionalLightComponent final
	{
		DirectionalLight directionalLight;
	};

	void RegisterDirectionalLightComponent();
}