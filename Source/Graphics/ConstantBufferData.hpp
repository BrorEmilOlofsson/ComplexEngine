#pragma once
#include "Utility/Math/Vector2.hpp"
#include "Utility/Math/Vector3.hpp"
#include "Utility/Math/Vector4.hpp"
#include "Utility/Math/Matrix4x4.hpp"
#include "Utility/Color.hpp"
#include "Graphics/GraphicsConstants.hpp"
#include <array>

namespace Simple
{
	struct alignas(16) PointLightData final
	{
		RGBColor color = Colors::White;
		float intensity = 1.f;
		Point3f position = Point3f::Zero();
		float range = 0.0f;
	};

	struct alignas(16) PostProcessBufferData final
	{
		Vector3f tint = Vector3f::One();
		float saturation = 1.070f;

		float exposure = 0.060f;
		float contrast = 0.970f;
		float blackpoint = 0.020f;
		float bloom = 1.280f;

		float bloomPixelFilterThreshold = 0.991f;
		unsigned int useToneMapping = 0;
		unsigned int useBloom = 0;

		std::byte padding[4];
	};

	struct alignas(16) LightBufferData final
	{
		RGBColor ambientLightColor = Colors::White;
		float ambientLightIntensity = 1.f;
		RGBColor directionalLightColor = Colors::White;
		float directionalLightIntensity = 1.f;
		Vector3f directionalLightDirection = Vector3f();

		float specularIntensity = 1.f;
		float specularPower = 10.f;

		std::byte padding[12];
	};

	struct alignas(16) PointLightBufferData final
	{
		std::array<PointLightData, Global_Max_PointLights> pointLights;
		unsigned int used = 0;
		std::byte padding[12];
	};

	struct alignas(16) CameraBufferData final
	{
		Matrix4x4f viewMatrix = Matrix4x4f::Identity();
		Matrix4x4f projectionMatrix = Matrix4x4f::Identity();

		Point3f cameraPosition = Point3f::Zero();
		std::byte paddingCameraPos[4]{};

		Vector2ui resolution = Vector2ui(1280, 720);

		std::byte padding[8];
	};

	struct alignas(16) TransformBufferData final
	{
		Matrix4x4f modelWorldMatrix = Matrix4x4f::Identity();
	};

	struct alignas(16) ColorBufferData final
	{
		Color color = Colors::White;
	};

	struct alignas(16) TimeBufferData final
	{
		float totalTime = 0.0f;
		float deltaTime = 0.0f;

		std::byte padding[8];
	};

	struct alignas(16) BoneBufferData final
	{
		std::array<Matrix4x4f, GlobalMaxBones> boneTransforms;
	};

	struct alignas(16) SkyBoxBufferData final
	{
		int skyBoxUseFlooring = 0;

		std::byte padding[12];
	};

	struct alignas(16) ObjectIDBufferData final
	{
		uint32_t objectID = std::numeric_limits<uint32_t>::max();

		std::byte padding[12];
	};
}