#pragma once
#include "Graphics/Light/PointLight.hpp"

namespace Simple
{

	
	struct PointLightComponent final
	{
		PointLight pointLight;
	};

	void RegisterPointLightComponent();
}