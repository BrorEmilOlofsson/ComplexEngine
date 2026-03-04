#pragma once
#include "Engine/OperatingSystem/WindowView.hpp"
#include "Engine/OperatingSystem/WindowHandle.hpp"
#include "Engine/Utility/GraphicsBufferData.hpp"
#include "Engine/Graphics/GraphicsFoundation.hpp"
#include "Engine/Graphics/RenderState.hpp"

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
		void EndFrame(RenderContext* renderContext);

		void Init();
		void Shutdown();

		[[nodiscard]] WindowView GetWindow(WindowHandle windowHandle);
		[[nodiscard]] CWindowView GetWindow(WindowHandle windowHandle) const;
        [[nodiscard]] GraphicsFoundation& GetGraphicsFoundation();
        [[nodiscard]] const GraphicsFoundation& GetGraphicsFoundation() const;


		WindowHandle MakeWindow(Vector2ui size, std::wstring title);


		void LoadCursors(const std::filesystem::path& path);
        [[nodiscard]] void* GetForegroundWindow() const;
		[[nodiscard]] bool IsCursorVisible() const;
		Point2i GetCursorScreenPosition() const;
        void ShowCursor();
		void HideCursor();

	private:

		class OperatingSystemConcept
		{
		public:

			OperatingSystemConcept() = default;
			virtual ~OperatingSystemConcept() = default;

			virtual void BeginFrame(const GraphicsBufferData& data) = 0;
			virtual void EndFrame(RenderContext* renderContext) = 0;
			virtual void Init() = 0;
			virtual void Shutdown() = 0;
			virtual WindowView GetWindow(WindowHandle windowHandle) = 0;
			virtual CWindowView GetCWindow(WindowHandle windowHandle) const = 0;
			virtual WindowHandle MakeWindow(Vector2ui size, std::wstring title) = 0;
            virtual GraphicsFoundation& GetGraphicsFoundation() = 0;
            virtual const GraphicsFoundation& GetGraphicsFoundation() const = 0;
			virtual void LoadCursors(const std::filesystem::path& path) = 0;
            virtual void* GetForegroundWindow() const = 0;
			virtual bool IsCursorVisible() const = 0;
			virtual Point2i GetCursorScreenPosition() const = 0;
            virtual void ShowCursor() = 0;
            virtual void HideCursor() = 0;

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

			void EndFrame(RenderContext* renderContext) override
			{
				OSEndFrame(mObject, renderContext);
			}

			void Init() override
			{
				OSInit(mObject);
			}

			void Shutdown() override
			{
				OSShutdown(mObject);
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

			WindowHandle MakeWindow(Vector2ui size, std::wstring title) override
			{
				return OSCreateWindow(mObject, size, title);
			}

			GraphicsFoundation& GetGraphicsFoundation() override
			{
				return OSGetGraphicsFoundation(mObject);
			}

			const GraphicsFoundation& GetGraphicsFoundation() const override
			{
				return OSGetGraphicsFoundation(mObject);
            }

			void LoadCursors(const std::filesystem::path& path) override
			{
                OSLoadCursors(mObject, path);
			}

			void* GetForegroundWindow() const override
			{
				return OSGetForegroundWindow(mObject);
            }

			bool IsCursorVisible() const override
			{
				return OSIsCursorVisible(mObject);
            }

			Point2i GetCursorScreenPosition() const override
			{
                return OSGetCursorPos(mObject);
			}

			void ShowCursor() override
			{
                OSShowCursor(mObject);
			}

			void HideCursor() override
			{
				OSHideCursor(mObject);
            }

		private:

			T mObject;
		};


		std::unique_ptr<OperatingSystemConcept> mConcept;

	};

}