#pragma once
#include "Engine/Utility/Camera.hpp"

namespace CLX
{

	struct CameraComponent final
	{
		Camera camera;
		bool freeFly = false;
	};

	void RegisterCameraComponent();
}