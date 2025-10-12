#pragma once
#include <External/imgui/imgui.h>
#include "Utility/PointerResource.hpp"

#ifdef _WIN32

#include <External/imgui/imgui_impl_dx11.h>
#include <d3d11.h>

namespace Simple
{
	struct ImGuiContextCreator
	{
		ImGuiContext* operator()() const
		{
			return ImGui::CreateContext();
		}
	};

	struct ImGuiContextDeleter
	{
		void operator()(ImGuiContext* context) const
		{
			ImGui::DestroyContext(context);
		}
	};

	template<typename Initializer, typename Deinitializer>
	class FunctionResource final
	{
	public:

		template<typename... Args>
		FunctionResource(Initializer initializer = Initializer{}, Deinitializer deinitializer = Deinitializer{}, Args&&... initializerArgs)
			: mInitializer(initializer)
			, mDeinitializer(deinitializer)
		{
			mInitializer(std::forward<Args>(initializerArgs)...);
		}

		~FunctionResource()
		{
			if (!mHasBeenMoved)
			{
				mDeinitializer();
			}
		}

		FunctionResource(const FunctionResource&) = delete;
		FunctionResource& operator=(const FunctionResource&) = delete;

		FunctionResource(FunctionResource&& other) noexcept
			: mInitializer(std::move(other.mInitializer))
			, mDeinitializer(std::move(other.mDeinitializer))
			, mHasBeenMoved(std::exchange(other.mHasBeenMoved, true))
		{
		}

		FunctionResource& operator=(FunctionResource&& other) noexcept
		{
			mInitializer = std::move(mInitializer);
			mDeinitializer = std::move(mDeinitializer);
			mHasBeenMoved = std::exchange(other.mHasBeenMoved, true);

			return *this;
		}

	private:

		[[no_unique_address]] Initializer mInitializer = Initializer{};
		[[no_unique_address]] Deinitializer mDeinitializer = Deinitializer{};
		bool mHasBeenMoved = false;
	};

	struct ImGuiInitializer
	{
		void operator()(ID3D11Device* device, ID3D11DeviceContext* context) const
		{
			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
			io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
			io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
			io.IniFilename = nullptr;
			io.LogFilename = nullptr;

			if (!ImGui_ImplDX11_Init(device, context))
			{
				assert(false);
			}
		}
	};

	struct ImGuiShutdowner
	{
		void operator()() const
		{
			ImGui_ImplDX11_Shutdown();
		}
	};

	class DX11ImGuiFoundation final
	{
	public:

		DX11ImGuiFoundation(ID3D11Device* device, ID3D11DeviceContext* context);
		//~DX11ImGuiFoundation();

		void BeginFrame();
		void EndFrame();
		void Load();
		void Save();

	private:

		PointerResource<ImGuiContextCreator, ImGuiContextDeleter> mImGuiContext;
		FunctionResource<ImGuiInitializer, ImGuiShutdowner> mImGuiFoundation;

	};
}

#endif