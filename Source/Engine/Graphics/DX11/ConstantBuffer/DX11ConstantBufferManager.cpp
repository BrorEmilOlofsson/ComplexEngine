#include "Engine/Precompiled/EnginePch.hpp"
#include "DX11ConstantBufferManager.hpp"
#include "Engine/Utility/Camera.hpp"
#include "Engine/Graphics/ConstantBufferData.hpp"
#include "Engine/Graphics/GraphicsConstants.hpp"
#include <cassert>

namespace CLX
{

	DX11ConstantBufferManager::DX11ConstantBufferManager(ID3D11Device& device)
	{
		mCameraBuffer.Init(device, ConstantBufferSlots::Camera);
		mTransformBuffer.Init(device, ConstantBufferSlots::Transform);
		mTimeBuffer.Init(device, ConstantBufferSlots::Time);
		mLightBuffer.Init(device, ConstantBufferSlots::Light);
		mPointLightBuffer.Init(device, ConstantBufferSlots::Pointlight);
		mPostProcessBuffer.Init(device, ConstantBufferSlots::PostProcess);
		mBoneBuffer.Init(device, ConstantBufferSlots::Bone);
		mSkyBoxBuffer.Init(device, ConstantBufferSlots::SkyBox);
		mColorBuffer.Init(device, ConstantBufferSlots::Color);
		mObjectIDBuffer.Init(device, ConstantBufferSlots::ObjectID);
	}

	void DX11ConstantBufferManager::UpdateCameraBuffer(Camera camera, const Dimension2u& resolution, ID3D11DeviceContext& context)
	{
		camera.SetResolution(resolution);
		CameraBufferData cameraBufferData
		{
			.viewMatrix = camera.GetViewMatrix(),
			.projectionMatrix = camera.GetProjectionMatrix(),
			.cameraPosition = camera.GetPosition(),
			.resolution = resolution
		};

		mCameraBuffer.Bind(context);
		mCameraBuffer.Update(cameraBufferData, context);
	}

	void DX11ConstantBufferManager::UpdateTimeBuffer(const float totalTime, const float deltaTime, ID3D11DeviceContext& context)
	{
		TimeBufferData timeBuffer = {};
		timeBuffer.totalTime = totalTime;
		timeBuffer.deltaTime = deltaTime;
		mTimeBuffer.Bind(context);
		mTimeBuffer.Update(timeBuffer, context);
	}

	void DX11ConstantBufferManager::UpdateLightBuffer(const LightBufferData& lightBufferData, ID3D11DeviceContext& context)
	{
		mLightBuffer.Bind(context);
		mLightBuffer.Update(lightBufferData, context);
	}

	void DX11ConstantBufferManager::UpdatePostProcessBuffer(const PostProcessBufferData& postProcessData, ID3D11DeviceContext& context)
	{
		mPostProcessBuffer.Bind(context);
		mPostProcessBuffer.Update(postProcessData, context);
	}

	void DX11ConstantBufferManager::UpdatePointLights(const PointLightBufferData& data, ID3D11DeviceContext& context)
	{
		mPointLightBuffer.Bind(context);
		mPointLightBuffer.Update(data, context);
	}

	void DX11ConstantBufferManager::UpdateSkyBoxBuffer(const bool shouldUseFlooring, ID3D11DeviceContext& context)
	{
		SkyBoxBufferData skyBoxBufferData;
		skyBoxBufferData.skyBoxUseFlooring = shouldUseFlooring;

		mSkyBoxBuffer.Bind(context);
		mSkyBoxBuffer.Update(skyBoxBufferData, context);
	}

	void DX11ConstantBufferManager::UpdateObjectIDBuffer(unsigned int objectID, ID3D11DeviceContext& context)
	{
		mObjectIDBuffer.Bind(context);
		mObjectIDBuffer.Update(ObjectIDBufferData{ objectID }, context);
	}
}
