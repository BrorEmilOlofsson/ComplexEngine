#pragma once
#include <External/imgui/imgui.h>
#include "Engine/Utility/PointerResource.hpp"
#include "Engine/Utility/FunctionResource.hpp"

#ifdef _WIN32

#include <External/imgui/imgui_impl_dx11.h>
#include <d3d11.h>

namespace CLX
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

		DX11ImGuiFoundation(const DX11ImGuiFoundation&) = delete;
        DX11ImGuiFoundation& operator=(const DX11ImGuiFoundation&) = delete;
        DX11ImGuiFoundation(DX11ImGuiFoundation&&) = default;
        DX11ImGuiFoundation& operator=(DX11ImGuiFoundation&&) = default;

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