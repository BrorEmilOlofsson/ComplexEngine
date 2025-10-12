#pragma once
#include "Engine/UI/UICanvas.hpp"

namespace Simple
{
	struct UICanvasComponent final
	{
		UICanvas mCanvas;
	};

	void RegisterUICanvasComponent();
}