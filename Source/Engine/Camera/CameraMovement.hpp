#pragma once
#include "Engine/Utility/Camera.hpp"
#include "Engine/Input/InputKey.hpp"
#include "Engine/Input/InputState.hpp"

namespace Simple
{

	struct CameraControls
	{
		eInputKey forwardKey = eInputKey::W;
		eInputKey leftKey = eInputKey::A;
		eInputKey rightKey = eInputKey::D;
		eInputKey backKey = eInputKey::S;
		eInputKey upKey = eInputKey::Space;
		eInputKey upKeyInverter = eInputKey::Ctrl;
	};

	struct FreeFlyCameraSettings final
	{
		float moveSpeed = 10.f;
		float rotationSpeed = 20.f;
		bool freeFly = false;
		bool mouseIsTapped = false;
		float mouseTapTimer = 0.f;
		CameraControls controls;
	};

	void FreeFlyCameraUpdate(Camera& camera, float deltaTime, float moveSpeed, float rotationSpeed, 
		bool forwardMovement, bool backwardMovement, bool rightMovement, bool leftMovement, bool upMovement, bool invertUp, Vector2f mouseDelta);


	void FreeFlyCameraUpdate(Camera& camera, float deltaTime, float moveSpeed, float rotationSpeed, const InputState& input, const CameraControls& controls);
}