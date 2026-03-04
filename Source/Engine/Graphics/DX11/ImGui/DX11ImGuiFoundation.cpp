#include "Engine/Precompiled/EnginePch.hpp"
#include "DX11ImGuiFoundation.hpp"
#include <External/imgui/imgui_impl_win32.h>
#include <External/imgui/imguizmo/ImGuizmo.h>
#include <filesystem>

namespace CLX
{

	DX11ImGuiFoundation::DX11ImGuiFoundation(ID3D11Device* device, ID3D11DeviceContext* context)
		: mImGuiFoundation(ImGuiInitializer{}, ImGuiShutdowner{}, device, context)
	{
	}

	void DX11ImGuiFoundation::BeginFrame()
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();

		ImGui::NewFrame();
		ImGuizmo::BeginFrame();

		ImGui::DockSpaceOverViewport(nullptr, ImGuiDockNodeFlags_PassthruCentralNode);
	}

	void DX11ImGuiFoundation::EndFrame()
	{
		PROFILER_FUNCTION(profiler::colors::Red300);
		ImGui::Render();

		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	void DX11ImGuiFoundation::Load()
	{
		IMGUI_CHECKVERSION();

		ImGui::LoadIniSettingsFromDisk(std::filesystem::absolute(SIMPLE_SETTINGS_IMGUI).string().c_str());
	}

	void DX11ImGuiFoundation::Save()
	{
		ImGui::SaveIniSettingsToDisk(std::filesystem::absolute(SIMPLE_SETTINGS_IMGUI).string().c_str());
	}
}