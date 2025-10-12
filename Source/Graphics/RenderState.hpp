#pragma once
#include <vector>
#include <optional>
#include "Graphics/RenderList.hpp"
#include "Utility/Camera.hpp"
#include "Graphics/SkyBox/Skybox.hpp"
#include "Graphics/Light/DirectionalLight.hpp"
#include "Graphics/Light/PointLight.hpp"
#include "Graphics/Light/AmbientLight.hpp"
#include "Graphics/RenderTarget/RenderTargetView.hpp"
#include "Graphics/DepthBuffer/DepthStencilViewHandle.hpp"

namespace Simple
{

	class RenderState final
	{
	public:

		constexpr RenderState() = default;

		[[nodiscard]] constexpr RenderList& GetRenderList() noexcept;
		[[nodiscard]] constexpr const RenderList& GetRenderList() const noexcept;
		[[nodiscard]] constexpr const std::optional<Camera>& GetCamera() const noexcept;
		[[nodiscard]] constexpr const std::optional<SkyBox>& GetSkyBox() const noexcept;
		[[nodiscard]] constexpr const std::optional<DirectionalLight>& GetDirectionalLight() const noexcept;
		[[nodiscard]] constexpr const std::optional<AmbientLight>& GetAmbientLight() const noexcept;
		[[nodiscard]] constexpr const std::optional<AABB2i>& GetRenderRect() const noexcept;
		[[nodiscard]] constexpr const std::optional<RenderTargetView>& GetRenderTargetView() const noexcept;
		[[nodiscard]] constexpr const std::optional<DepthStencilViewHandle>& GetDepthStencilViewHandle() const noexcept;
		
		constexpr void SetSkyBox(const SkyBox& skyBox) noexcept;
		constexpr void SetDirectionalLight(const DirectionalLight& directionalLight) noexcept;
		constexpr void SetAmbientLight(const AmbientLight& ambientLight) noexcept;
		constexpr void SetCamera(const Camera& camera) noexcept;
		constexpr void SetRenderRect(const AABB2i& renderRect) noexcept;
		constexpr void SetRenderTargetView(const RenderTargetView& renderTargetView) noexcept;
		constexpr void SetDepthStencilViewHandle(const DepthStencilViewHandle& handle);

		constexpr void Reset();

	private:

		RenderList mRenderList;
		std::optional<Camera> mCamera;
		std::optional<SkyBox> mSkyBox;
		std::optional<DirectionalLight> mDirectionalLight;
		std::optional<AmbientLight> mAmbientLight;
		std::optional<AABB2i> mRenderRect;
		std::optional<RenderTargetView> mRenderTargetView;
		std::optional<DepthStencilViewHandle> mDepthStencilViewHandle;
	};

	constexpr RenderList& RenderState::GetRenderList() noexcept
	{
		return mRenderList;
	}

	constexpr const RenderList& RenderState::GetRenderList() const noexcept
	{
		return mRenderList;
	}

	constexpr const std::optional<Camera>& RenderState::GetCamera() const noexcept
	{
		return mCamera;
	}

	constexpr const std::optional<SkyBox>& RenderState::GetSkyBox() const noexcept
	{
		return mSkyBox;
	}

	constexpr const std::optional<DirectionalLight>& RenderState::GetDirectionalLight() const noexcept
	{
		return mDirectionalLight;
	}

	constexpr const std::optional<AmbientLight>& RenderState::GetAmbientLight() const noexcept
	{
		return mAmbientLight;
	}

	constexpr const std::optional<AABB2i>& RenderState::GetRenderRect() const noexcept
	{
		return mRenderRect;
	}

	constexpr const std::optional<RenderTargetView>& RenderState::GetRenderTargetView() const noexcept
	{
		return mRenderTargetView;
	}

	constexpr const std::optional<DepthStencilViewHandle>& RenderState::GetDepthStencilViewHandle() const noexcept
	{
		return mDepthStencilViewHandle;
	}

	constexpr void RenderState::SetSkyBox(const SkyBox& skyBox) noexcept
	{
		mSkyBox = skyBox;
	}

	constexpr void RenderState::SetDirectionalLight(const DirectionalLight& directionalLight) noexcept
	{
		mDirectionalLight = directionalLight;
	}

	constexpr void RenderState::SetAmbientLight(const AmbientLight& ambientLight) noexcept
	{
		mAmbientLight = ambientLight;
	}

	constexpr void RenderState::SetCamera(const Camera& camera) noexcept
	{
		mCamera = camera;
	}

	constexpr void RenderState::SetRenderRect(const AABB2i& renderRect) noexcept
	{
		mRenderRect = renderRect;
	}

	constexpr void RenderState::SetRenderTargetView(const RenderTargetView& renderTargetView) noexcept
	{
		mRenderTargetView = renderTargetView;
	}

	constexpr void RenderState::SetDepthStencilViewHandle(const DepthStencilViewHandle& handle)
	{
		mDepthStencilViewHandle = handle;
	}

	constexpr void RenderState::Reset()
	{
		mRenderList.Clear();
		mSkyBox.reset();
		mDirectionalLight.reset();
		mAmbientLight.reset();
		//mRenderRect.reset();
		mCamera.reset();
	}
}