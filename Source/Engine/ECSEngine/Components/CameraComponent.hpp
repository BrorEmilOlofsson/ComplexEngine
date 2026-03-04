#pragma once
#include "Engine/Utility/Camera.hpp"

namespace Simple
{

	struct CameraComponent final
	{
		Camera camera;
		bool freeFly = false;
	};

	void RegisterCameraComponent();
}