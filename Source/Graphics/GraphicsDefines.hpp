#pragma once

namespace Simple
{
	constexpr std::size_t Global_Max_Joints = 64;
	constexpr std::size_t Global_Max_PointLights = 100;

	namespace ConstantBufferSlots
	{
		constexpr unsigned int Camera = 0;
		constexpr unsigned int Transform = 1;
		constexpr unsigned int Time = 2;
		constexpr unsigned int Light = 3;
		constexpr unsigned int Joints = 4;
		constexpr unsigned int PostProcess = 5;
		constexpr unsigned int Pointlight = 6;
		constexpr unsigned int SkyBox = 7;
		constexpr unsigned int ObjectID = 8;
		constexpr unsigned int Color = 9;
	}

	constexpr std::size_t Global_StartSlot_GBuffer = 5;
	constexpr std::size_t Global_GBuffer_Count = 5;

	namespace TextureSlots
	{
		constexpr unsigned int Albedo = 0;
		constexpr unsigned int Normal = 1;
		constexpr unsigned int Material = 2;
		constexpr unsigned int CubeMap = 4;
		constexpr unsigned int ShadowMap = 5;
	}
}