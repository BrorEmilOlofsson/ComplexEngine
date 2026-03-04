#pragma once
#include <External/imgui/imgui.h>

namespace CLX
{

	class ImGuiBegin final
	{
	public:

		ImGuiBegin(const char* name, bool* open = nullptr, ImGuiWindowFlags flags = 0)
		{
			mIsOpen = ImGui::Begin(name, open, flags);
		}

		~ImGuiBegin()
		{
			ImGui::End();
		}

		[[nodiscard]] bool IsOpen() const
		{
			return mIsOpen;
		}

		ImGuiBegin(const ImGuiBegin&) = delete;
		ImGuiBegin(ImGuiBegin&&) = delete;
		ImGuiBegin& operator=(const ImGuiBegin&) = delete;
		ImGuiBegin& operator=(ImGuiBegin&&) = delete;

	private:

		bool mIsOpen;
	};
}