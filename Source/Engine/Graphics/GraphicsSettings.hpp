#pragma once
#include "Engine/Utility/Color.hpp"

namespace Simple
{
	class GraphicsSettings final
	{
	public:
		Color clearColor = Colors::DukeBlue;
		unsigned int fPSCap = 0;
		bool vSync = false;

		bool mIsUsingPBR = true;
		bool mShouldRenderMesh = true;
		bool mShouldRenderDebugLines = true;
		bool mShouldRenderBoundingBox = true;
		bool mShouldRenderSkeletonLines = false;

	};
}