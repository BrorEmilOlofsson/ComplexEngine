#pragma once
#include "Engine/Win/WinDefines.hpp"

#include "Engine/Math/Vector2.hpp"
#include "Engine/Graphics/DX11/ConstantBuffer/DX11ConstantBuffer.hpp"
#include "Engine/Graphics/ConstantBufferData.hpp"
#include "Engine/Utility/Camera.hpp"
#include <memory>

#ifdef _WIN32

namespace CLX
{
	class DX11ConstantBufferManager final
	{
	public:

		explicit DX11ConstantBufferManager(ID3D11Device& device);

		void UpdateCameraBuffer(Camera camera, const Vector2ui& resolution, ID3D11DeviceContext& context);
		void UpdateTimeBuffer(const float totalTime, const float deltaTime, ID3D11DeviceContext& context);
		void UpdateLightBuffer(const LightBufferData& lightBufferData, ID3D11DeviceContext& context);
		void UpdatePostProcessBuffer(const PostProcessBufferData& postProcessData, ID3D11DeviceContext& context);
		void UpdatePointLights(const PointLightBufferData& data, ID3D11DeviceContext& context);
		void UpdateSkyBoxBuffer(const bool shouldUseFlooring, ID3D11DeviceContext& context);
		//void UpdateColorBuffer(Color color, ID3D11DeviceContext& context);
		void UpdateObjectIDBuffer(unsigned int objectID, ID3D11DeviceContext& context);

	public:

		DX11ConstantBuffer<CameraBufferData> mCameraBuffer;
		DX11ConstantBuffer<TransformBufferData> mTransformBuffer;
		DX11ConstantBuffer<TimeBufferData> mTimeBuffer;
		DX11ConstantBuffer<BoneBufferData> mBoneBuffer;
		DX11ConstantBuffer<LightBufferData> mLightBuffer;
		DX11ConstantBuffer<PointLightBufferData> mPointLightBuffer;
		DX11ConstantBuffer<PostProcessBufferData> mPostProcessBuffer;
		DX11ConstantBuffer<SkyBoxBufferData> mSkyBoxBuffer;
		DX11ConstantBuffer<ColorBufferData> mColorBuffer;
		DX11ConstantBuffer<ObjectIDBufferData> mObjectIDBuffer;
	};
}

#endif