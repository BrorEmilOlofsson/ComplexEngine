#pragma once

namespace Simple
{

	struct NavmeshRenderSettings final
	{
		bool renderLines = false;
		bool renderNodeIndices = false;
		bool renderVertexIndices = false;
		bool renderNodeGrid = false;
	};
}