#pragma once
#include <cstdint>

namespace CLX
{
	constexpr std::size_t GlobalMaxBones = 64;
	constexpr std::size_t Global_Max_PointLights = 100;

	namespace ConstantBufferSlots
	{
		constexpr uint32_t Camera = 0;
		constexpr uint32_t Transform = 1;
		constexpr uint32_t Time = 2;
		constexpr uint32_t Light = 3;
		constexpr uint32_t Bone = 4;
		constexpr uint32_t PostProcess = 5;
		constexpr uint32_t Pointlight = 6;
		constexpr uint32_t SkyBox = 7;
		constexpr uint32_t ObjectID = 8;
		constexpr uint32_t Color = 9;
	}

	constexpr std::size_t Global_StartSlot_GBuffer = 5;
	constexpr std::size_t Global_GBuffer_Count = 5;

	namespace TextureSlots
	{
		constexpr uint32_t Albedo = 0;
		constexpr uint32_t Normal = 1;
		constexpr uint32_t Material = 2;
		constexpr uint32_t CubeMap = 4;
		constexpr uint32_t GBufferStart = 5;
	}
}