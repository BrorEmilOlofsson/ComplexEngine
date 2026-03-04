#pragma once
#include <External/imgui/imgui.h>
#include <array>

namespace CLX
{
	enum class eImGuiEditorStyle
	{
		Simple,
		Dark,
		Light,
		Count
	};

	enum class eImGuiEditorMode
	{
		Default,
		Playing,
		Count
	};

	class ImGuiStyleManager final
	{
	public:

		ImGuiStyleManager() = default;

		void Init();

		void SetEditorStyle(const eImGuiEditorStyle style);
		void SetEditorMode(const eImGuiEditorMode mode);

	private:

		ImGuiStyle mStyle = {};
		std::array<ImVec4, static_cast<std::size_t>(ImGuiCol_COUNT)> mColors;
		ImVec4 mPlayModeBackgroundColor = {};
		ImVec4 mInteractiveColor = {};
		eImGuiEditorStyle mCurrentColorStyle = eImGuiEditorStyle::Simple;
		static constexpr ImVec4 DarkGrayColor = ImVec4(0.20f, 0.20f, 0.20f, 1.0f);
		static constexpr ImVec4 LegacyPinkColor = ImVec4(1.00f, 0.00f, 1.00f, 1.0f);
		static constexpr ImVec4 SoftBlueColor = ImVec4(0.29f, 0.56f, 0.89f, 1.0f);
	};
}