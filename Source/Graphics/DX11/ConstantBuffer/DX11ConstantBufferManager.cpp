#include "Graphics/Precompiled/GraphicsPch.hpp"
#include "DX11ConstantBufferManager.hpp"
#include "Utility/Camera.hpp"
#include "Graphics/ConstantBufferData.hpp"
#include "Graphics/GraphicsConstants.hpp"
#include <cassert>

namespace Simple
{

	DX11ConstantBufferManager::DX11ConstantBufferManager(ID3D11Device& device)
	{
		mCameraBuffer.Init(device, ConstantBufferSlots::Camera);
		mTransformBuffer.Init(device, ConstantBufferSlots::Transform);
		mTimeBuffer.Init(device, ConstantBufferSlots::Time);
		mLightBuffer.Init(device, ConstantBufferSlots::Light);
		mPointLightBuffer.Init(device, ConstantBufferSlots::Pointlight);
		mPostProcessBuffer.Init(device, ConstantBufferSlots::PostProcess);
		mJointsBuffer.Init(device, ConstantBufferSlots::Joints);
		mSkyBoxBuffer.Init(device, ConstantBufferSlots::SkyBox);
		mColorBuffer.Init(device, ConstantBufferSlots::Color);
		mObjectIDBuffer.Init(device, ConstantBufferSlots::ObjectID);
	}

	void DX11ConstantBufferManager::UpdateCameraBuffer(Camera camera, const Vector2ui& resolution, ID3D11DeviceContext& context)
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
