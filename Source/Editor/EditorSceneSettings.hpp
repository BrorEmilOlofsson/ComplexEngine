#pragma once
#include "Engine/Math/TransformMode.hpp"

namespace Simple
{
	struct EditorSceneSettings final
	{
		bool useSnap = false;
		float snapValue = 1.0f;
		bool showGrid = false;
		eTransformMode transformMode = eTransformMode::Local;
		bool showUnitVectorInScene = false;
	};
}