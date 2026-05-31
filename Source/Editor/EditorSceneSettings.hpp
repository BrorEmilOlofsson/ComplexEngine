#pragma once
#include "Engine/Math/TransformMode.hpp"
#include "Engine/Math/TransformOperation.hpp"

namespace CLX
{
	struct EditorSceneSettings final
	{
		bool useSnap = false;
		float snapValue = 1.0f;
		bool showGrid = false;
        bool showNavmeshEditor = false;
		eTransformMode transformMode = eTransformMode::Local;
        eTransformOperation transformOperation = eTransformOperation::Translate;
		bool showUnitVectorInScene = false;
	};
}