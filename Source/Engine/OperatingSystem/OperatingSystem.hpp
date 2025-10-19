#pragma once
#include "Engine/OperatingSystem/WindowView.hpp"
#include "Engine/OperatingSystem/OSView.hpp"
#include "Engine/OperatingSystem/WindowHandle.hpp"
#include "Utility/Input/InputState.hpp"
#include "Utility/Asset/AssetManager.hpp"
#include "Graphics/GraphicsSettings.hpp"
#include "Utility/GraphicsBufferData.hpp"
#include "Graphics/RenderTarget/RenderTargetView.hpp"
#include "Graphics/DepthBuffer/DepthStencilViewHandle.hpp"
#include "Graphics/RenderState.hpp"
#include <exception>

namespace Simple
{

	class OperatingSystem final
	{
	public:

		template<typename T>
		OperatingSystem(T&& operatingSystem)
			: mConcept(std::make_unique<OperatingSystemModel<T>>(std::forward<T>(operatingSystem)))
		{
		}

		void BeginFrame(const GraphicsBufferData& bufferData);
		void EndFrame(std::optional<RenderTargetView> renderTarget);

		void Render();
		void Render(RenderState& renderState);

		void Init();
		void Shutdown();

		[[nodiscard]] WindowView GetWindow(WindowHandle windowHandle);
		[[nodiscard]] CWindowView GetWindow(WindowHandle windowHandle) const;
		[[nodiscard]] OSView GetOS();
		[[nodiscard]] RenderTargetView CreateRenderTarget(const Vector2ui& size);
		[[nodiscard]] DepthStencilViewHandle CreateDepthStencilView(const Vector2ui& size);

		[[nodiscard]] RenderContext CreateRenderContext(const Vector2ui size)
		{
			return mConcept->CreateRenderContext(size);
		}

		void LoadCursors(const std::filesystem::path& path);

		WindowHandle MakeWindow(Vector2ui size, std::wstring title)
		{
			return mConcept->MakeWindow(size, title);
		}

		void SetAssetManager(std::shared_ptr<AssetManager> assetManager)
		{
			mConcept->SetAssetManager(assetManager);
		}

		void SetGraphicsSettings(std::shared_ptr<GraphicsSettings> graphicsSettings)
		{
			mConcept->SetGraphicsSettings(graphicsSettings);
		}

	private:

		class OperatingSystemConcept
		{
		public:

			OperatingSystemConcept() = default;
			virtual ~OperatingSystemConcept() = default;

			virtual void BeginFrame(const GraphicsBufferData& data) = 0;
			virtual void EndFrame(std::optional<RenderTargetView> renderTarget) = 0;
			virtual void Init() = 0;
			virtual void Shutdown() = 0;
			virtual void Render() = 0;
			virtual void Render(RenderState& renderState) = 0;
			virtual WindowView GetWindow(WindowHandle windowHandle) = 0;
			virtual CWindowView GetCWindow(WindowHandle windowHandle) const = 0;
			virtual OSView GetOS() = 0;
			virtual WindowHandle MakeWindow(Vector2ui size, std::wstring title) = 0;
			virtual RenderTargetView CreateRenderTarget(const Vector2ui& size) = 0;
			virtual DepthStencilViewHandle CreateDepthStencilView(const Vector2ui& size) = 0;
			virtual RenderContext CreateRenderContext(const Vector2ui& size) = 0;
			virtual void SetAssetManager(std::shared_ptr<AssetManager> assetManager) = 0;
			virtual void SetGraphicsSettings(std::shared_ptr<GraphicsSettings> graphicsSettings) = 0;
		};

		template<typename T>
		class OperatingSystemModel final : public OperatingSystemConcept
		{
		public:

			OperatingSystemModel(T&& object)
				: mObject(std::move(object))
			{
			}

			void BeginFrame(const GraphicsBufferData& data) override
			{
				OSBeginFrame(mObject, data);
			}

			void EndFrame(std::optional<RenderTargetView> renderTarget) override
			{
				OSEndFrame(mObject, renderTarget);
			}

			void Init() override
			{
				OSInit(mObject);
			}

			void Shutdown() override
			{
				OSShutdown(mObject);
			}

			void Render() override
			{
				OSRender(mObject);
			}

			void Render(RenderState& renderState) override
			{
				OSRender(mObject, renderState);
			}

			WindowView GetWindow(WindowHandle windowHandle) override
			{
				auto& window = GetOSWindow(mObject, windowHandle);
				return WindowView(window);
			}

			CWindowView GetCWindow(WindowHandle windowHandle) const override
			{
				auto& window = GetOSWindow(mObject, windowHandle);
				return CWindowView(window);
			}

			OSView GetOS() override
			{
				return OSView(GetOSStyle(mObject));
			}

			WindowHandle MakeWindow(Vector2ui size, std::wstring title) override
			{
				return OSCreateWindow(mObject, size, title);
			}

			RenderTargetView CreateRenderTarget(const Vector2ui& size) override
			{
				return OSCreateRenderTarget(mObject, size);
			}

			DepthStencilViewHandle CreateDepthStencilView(const Vector2ui& size) override
			{
				return OSCreateDepthStencilView(mObject, size);
			}

			RenderContext CreateRenderContext(const Vector2ui& size) override
			{
				return OSCreateRenderContext(mObject, size);
			}

			void SetAssetManager(std::shared_ptr<AssetManager> assetManager) override
			{
				OSSetAssetManager(mObject, assetManager);
			}

			void SetGraphicsSettings(std::shared_ptr<GraphicsSettings> graphicsSettings) override
			{
				OSSetGraphicsSettings(mObject, graphicsSettings);
			}

		private:

			T mObject;
		};


		std::unique_ptr<OperatingSystemConcept> mConcept;
		InputState mInputState;

	};

}