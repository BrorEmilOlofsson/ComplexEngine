#include "Editor/Precompiled/EditorPch.hpp"
#include "ImGuiStyleManager.hpp"
#include <IconFontDefines.h>

namespace CLX
{

	static void LoadImGuiIniFile()
	{
		std::filesystem::path iniPath = std::filesystem::absolute("Settings/imgui.ini");
		if (iniPath.empty())
		{
			std::filesystem::path savedSource = std::filesystem::absolute(std::filesystem::path(SIMPLE_DIR_SETTINGS) / "imgui.ini");
			std::filesystem::copy(savedSource, iniPath);
		}
		const std::string iniPathStr = iniPath.string();
		ImGui::LoadIniSettingsFromDisk(iniPathStr.c_str());
	}

	static void LoadFonts()
	{
		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->AddFontDefault();

		constexpr ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };

		ImFontConfig icons_config;
		icons_config.MergeMode = true;
		icons_config.PixelSnapH = true;

		io.Fonts->AddFontFromFileTTF(std::filesystem::absolute(std::filesystem::path(Directory::Assets) / "Fonts/fa-solid-900.ttf").string().c_str(), 11.0f, &icons_config, icons_ranges);
		io.Fonts->Build();


		
	}

	[[nodiscard]] static std::array<ImVec4, static_cast<std::size_t>(ImGuiCol_COUNT)> LoadColors(ImVec4 darkGrayColor, ImVec4 softBlueColor, ImVec4& interactiveColor, ImVec4& playModeBackgroundColor)
	{
		playModeBackgroundColor = ImVec4(0.2350f, 0.05f, 0.05f, 1.0f);
		interactiveColor = softBlueColor;

		const ImVec4 darkGrey = darkGrayColor;
		const ImVec4 lightGreyAlphaDown = ImVec4(0.485f, 0.485f, 0.485f, 0.90f);
		const ImVec4 darkGreyScaledDown = ImVec4(darkGrey.x - 0.04f, darkGrey.y - 0.04f, darkGrey.z - 0.04f, 1.00f);
		const ImVec4 darkGreySlightlyScaledUp = ImVec4(darkGrey.x + 0.05f, darkGrey.y + 0.05f, darkGrey.z + 0.05f, 1.0f);
		const ImVec4 darkGreyModerateScaledUp = ImVec4(darkGrey.x + 0.13f, darkGrey.y + 0.13f, darkGrey.z + 0.13f, 1.00f);
		const ImVec4 darkGreyMuchScaledUpAlphaDown = ImVec4(darkGrey.x + 0.20f, darkGrey.y + 0.20f, darkGrey.z + 0.20f, 0.90f);
		const ImVec4 darkGreyAlphaSlightlyScaledDown = ImVec4(darkGrey.x - 0.02f, darkGrey.x - 0.02f, darkGrey.x - 0.02f, 0.90f);
		const ImVec4 darkGreyAlphaModerateScaledDown = ImVec4(darkGreyAlphaSlightlyScaledDown.x, darkGreyAlphaSlightlyScaledDown.y, darkGreyAlphaSlightlyScaledDown.z, 0.80f);

		const ImVec4 activeColor = interactiveColor;
		const ImVec4 activeLessAlpha = ImVec4(activeColor.x, activeColor.y, activeColor.z, 0.90f);
		const ImVec4 activeHalfAlpha = ImVec4(activeColor.x, activeColor.y, activeColor.z, 0.50f);
		const ImVec4 activeScaledDown = ImVec4(activeColor.x * 0.70f, activeColor.y * 0.70f, activeColor.z * 0.70f, 0.70f);
		const ImVec4 activeScaledDownEqually = ImVec4(activeColor.x * 0.60f, activeColor.y * 0.60f, activeColor.z * 0.60f, 0.50f);
		const ImVec4 activeLessAlphaSlightlyGreen = ImVec4(activeColor.x * 0.90f, activeColor.y > 0 ? activeColor.y * 1.05f : 0.25f, activeColor.z, 0.40f);

		auto& colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_MenuBarBg] = darkGrey;
		colors[ImGuiCol_WindowBg] = darkGrey;
		colors[ImGuiCol_PopupBg] = darkGreyScaledDown;
		colors[ImGuiCol_TitleBgActive] = activeColor;
		colors[ImGuiCol_TitleBgCollapsed] = activeColor;
		colors[ImGuiCol_ButtonHovered] = activeScaledDown;
		colors[ImGuiCol_HeaderHovered] = activeColor;
		colors[ImGuiCol_TabUnfocusedActive] = darkGreyMuchScaledUpAlphaDown;
		colors[ImGuiCol_Border] = darkGreyScaledDown;
		colors[ImGuiCol_FrameBg] = darkGreySlightlyScaledUp;
		colors[ImGuiCol_FrameBgHovered] = activeLessAlphaSlightlyGreen;
		colors[ImGuiCol_TitleBg] = darkGreyAlphaModerateScaledDown;
		colors[ImGuiCol_ScrollbarBg] = darkGrey;
		colors[ImGuiCol_CheckMark] = activeColor;
		colors[ImGuiCol_SliderGrab] = activeColor;
		colors[ImGuiCol_SliderGrabActive] = activeColor;
		colors[ImGuiCol_Button] = activeColor;
		colors[ImGuiCol_Header] = activeHalfAlpha;
		colors[ImGuiCol_Tab] = darkGreyModerateScaledUp;
		colors[ImGuiCol_TabHovered] = activeLessAlpha;
		colors[ImGuiCol_TabActive] = activeColor;
		colors[ImGuiCol_TabUnfocused] = darkGreyAlphaSlightlyScaledDown;
		colors[ImGuiCol_TabUnfocusedActive] = lightGreyAlphaDown;
		colors[ImGuiCol_DockingPreview] = activeColor;
		colors[ImGuiCol_ResizeGripHovered] = activeColor;
		colors[ImGuiCol_ResizeGripActive] = activeColor;
		colors[ImGuiCol_ButtonActive] = activeScaledDownEqually;
		colors[ImGuiCol_ChildBg] = darkGreyAlphaModerateScaledDown;

		std::array<ImVec4, static_cast<size_t>(ImGuiCol_COUNT)> c;
		for (size_t i = 0; i < static_cast<size_t>(ImGuiCol_COUNT); ++i)
		{
			c[i] = colors[i];
		}
		return c;
	}

	[[nodiscard]] static ImGuiStyle LoadStyle()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowBorderSize = 0;
		style.TabBarBorderSize = 0;
		style.TabBorderSize = 0;
		style.WindowMenuButtonPosition = ImGuiDir_None;
		style.DockingSeparatorSize = 1;
		style.DisplaySafeAreaPadding.x = 10;
		style.PopupBorderSize = 0;
		style.FrameBorderSize = 0.0f;
		style.ChildRounding = 5.0f;
		style.TabRounding = 0;

		return style;
	}

	void ImGuiStyleManager::Init()
	{
		ImGui::StyleColorsDark();

		LoadImGuiIniFile();
		mColors = LoadColors(DarkGrayColor, SoftBlueColor, mInteractiveColor, mPlayModeBackgroundColor);
		mStyle = LoadStyle();
		LoadFonts();
	}

	void ImGuiStyleManager::SetEditorStyle(const eImGuiEditorStyle styleType)
	{
		mCurrentColorStyle = styleType;

		switch (styleType)
		{
		case eImGuiEditorStyle::Simple:
		{
			ImGuiStyle& style = ImGui::GetStyle();
			ImVec4* colors = ImGui::GetStyle().Colors;
			style = mStyle;
			std::memcpy(colors, mColors.data(), sizeof(mColors));
			break;
		}
		case eImGuiEditorStyle::Dark:
		{
			ImGui::StyleColorsDark();
			break;
		}
		case eImGuiEditorStyle::Light:
		{
			ImGui::StyleColorsLight();
			break;
		}
		default:
			break;
		}
	}

	void ImGuiStyleManager::SetEditorMode(const eImGuiEditorMode mode)
	{
		switch (mode)
		{
		case eImGuiEditorMode::Playing:
		{
			ImVec4* colors = ImGui::GetStyle().Colors;
			colors[ImGuiCol_WindowBg] = mPlayModeBackgroundColor;
			break;
		}
		case eImGuiEditorMode::Default:
			SetEditorStyle(mCurrentColorStyle);
			break;
		default:
			break;
		}
	}
}