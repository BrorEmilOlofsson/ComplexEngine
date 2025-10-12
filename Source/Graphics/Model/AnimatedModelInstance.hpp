#pragma once
#include "Utility/Math/Transform3.hpp"
#include "Graphics/Model/Model.hpp"
#include "Graphics/GraphicsDefines.hpp"

namespace Simple
{

	struct AnimatedModelInstance
	{
		Transform transform;
		Model model;
		std::array<Matrix4x4f, Global_Max_Joints> jointMatrices;
		PixelShaderAssetHandle pixelShader;
		VertexShaderAssetHandle vertexShader;
	};
}