#pragma once
#include "Engine/UI/UICanvas.hpp"

namespace CLX
{
	struct UICanvasComponent final
	{
		UICanvas mCanvas;
	};

	void RegisterUICanvasComponent();
}