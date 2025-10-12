#pragma once
#include "Graphics/Light/DirectionalLight.hpp"

namespace Simple
{
	struct DirectionalLightComponent final
	{
		DirectionalLight directionalLight;
	};

	void RegisterDirectionalLightComponent();
}