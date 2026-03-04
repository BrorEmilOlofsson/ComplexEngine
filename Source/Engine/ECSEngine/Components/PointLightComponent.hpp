#pragma once
#include "Engine/Graphics/Light/PointLight.hpp"

namespace CLX
{

	
	struct PointLightComponent final
	{
		PointLight pointLight;
	};

	void RegisterPointLightComponent();
}