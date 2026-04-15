#include "Editor/Precompiled/EditorPch.hpp"
#include "GraphicsSettingsWindow.hpp"
#include "Engine/Utility/BlackboardKeys.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Graphics/GraphicsSettings.hpp"
#include "Editor/ImGui/ImGuiStyleManager.hpp"
#include "Engine/OperatingSystem/WindowView.hpp"
#include <array>
#include <unordered_map>
#include <stdlib.h>

namespace CLX
{

	static void UpdateFPSCapSettings(GraphicsSettings& graphicsSettings, const unsigned int /*aMonitorUpdateFrequency*/)
	{
		ImGui::SetNextItemWidth(200);

		static unsigned int fpsCap = 0;
		ImGui::InputScalar("FPS Cap##SettingWindow2", ImGuiDataType_U32, &fpsCap);

		if (ImGui::IsItemDeactivated())
		{
			graphicsSettings.fPSCap = fpsCap;
		}
	}

	/*static void UpdateConsoleSettings(bool& isConsoleOpen, const std::string& windowTag)
	{
		static const std::string console = std::string("Show Console").append(windowTag).c_str();
		if (ImGui::Checkbox(console.c_str(), &isConsoleOpen))
		{
			HWND consoleWindow = GetConsoleWindow();

			if (isConsoleOpen)
			{
				ShowWindow(consoleWindow, SW_SHOW);
			}
			else
			{
				ShowWindow(consoleWindow, SW_HIDE);
			}
		}

		static const std::string clearConsole = std::string("Clear Console").append(windowTag).c_str();
		if (ImGui::Button(clearConsole.c_str()))
		{
			system("CLS");
		}
	}*/

	void UpdateCursorSettings()
	{
		//ImGui::SetNextItemWidth(200);

		/*static const std::string cursors = std::string("Cursors").append(myImGuiTag).c_str();
		if (ImGui::Combo(cursors.c_str(), &mySelectedCursor, myLongCursorStringName.c_str()))
		{
			SetCustomCursor(myCursorNames[mySelectedCursor]);
		}*/
	}

	static std::string WindowSizeToString(Dimension2u windowSize)
	{
		return std::to_string(windowSize.GetWidth()) + "x" + std::to_string(windowSize.GetHeight());
	}

	void UpdateWindowsSizeSettings(Dimension2u currentWindowSize, std::span<const Dimension2u> windowSizes, std::optional<int>& selectedWindowSizeIndex, WindowView window)
	{
		ImGui::SetNextItemWidth(200);

		if (ImGui::BeginCombo("Window Size##SettingsWindow", WindowSizeToString(currentWindowSize).c_str()))
		{
			const int selectedIndex = selectedWindowSizeIndex.value_or(-1);
			for (int i = 0; i < windowSizes.size(); i++)
			{
				if (ImGui::Selectable(WindowSizeToString(windowSizes[i]).c_str(), selectedIndex == i))
				{
					selectedWindowSizeIndex = i;
					window.SetSize(windowSizes[i], false);
				}

			}

			ImGui::EndCombo();
		}
	}

	void UpdateAndSetEditorStyle(ImGuiStyleManager& imguiStyleManager)
	{
		constexpr static std::array<const char*, 3> editorStyles = { "Simple", "Dark", "Light" };

		static int selectedStyle = 0;

		ImGui::SetNextItemWidth(200);

		if (ImGui::Combo("Editor Style##GraphicsSettings", &selectedStyle, editorStyles.data(), static_cast<int>(editorStyles.size())))
		{
			switch (selectedStyle)
			{
			case 0:
				imguiStyleManager.SetEditorStyle(eImGuiEditorStyle::Simple);
				break;
			case 1:
				imguiStyleManager.SetEditorStyle(eImGuiEditorStyle::Dark);
				break;
			case 2:
				imguiStyleManager.SetEditorStyle(eImGuiEditorStyle::Light);
				break;
			default:
				imguiStyleManager.SetEditorStyle(eImGuiEditorStyle::Simple);
				break;
			}
		}
	}

	static void RenderPopup(bool& isActive, /*const std::string& windowName, const std::string& windowTag,*/
		const float deltaTime, [[maybe_unused]] bool& isConsoleOpen, const unsigned int monitorUpdateFrequency, const ManualTimerf& updateFpsTimer, float& fps,
		const std::span<const Dimension2u> windowSizes, std::optional<int>& selectedWindowSizeIndex,
		Dimension2u currentWindowSize, GraphicsSettings& graphicsSettings, ImGuiStyleManager& imguiStyleManager, WindowView osWindow)
	{
		if (!isActive)
		{
			return;
		}
		if (ImGui::Begin("Graphics Settings", &isActive))
		{
			constexpr unsigned int heightPadding = 2;

			if (updateFpsTimer.HasBeenTriggered())
			{
				fps = std::round(1.f / deltaTime);
			}
			ImGui::Text("FPS: %d", static_cast<int>(fps));

			ImGuiUtility::SeparatorTextDummy("Render");

			ImGui::Checkbox("VSync##GraphicsSettings", &graphicsSettings.vSync);

			ImGuiUtility::SameLineDummy(3, 0);

			ImGui::Checkbox("DebugLines##GraphicsSettings", &graphicsSettings.mShouldRenderDebugLines);

			ImGuiUtility::SameLineDummy(3, 0);

			ImGui::Checkbox("BoundingBox##GraphicsSettings", &graphicsSettings.mShouldRenderBoundingBox);

			ImGui::Checkbox("Mesh##GraphicsSettings", &graphicsSettings.mShouldRenderMesh);

			ImGuiUtility::SameLineDummy(10, 0);

			ImGui::Checkbox("PBR##GraphicsSettings", &graphicsSettings.mIsUsingPBR);

			ImGuiUtility::SameLineDummy(52, 0);

			ImGui::Checkbox("Skeleton##GraphicsSettings", &graphicsSettings.mShouldRenderSkeletonLines);

			ImGuiUtility::SeparatorDummy(0, heightPadding);

			UpdateFPSCapSettings(graphicsSettings, monitorUpdateFrequency);

			/*ImGui::SetNextItemWidth(200);

			const std::string rasterizerState = std::string("RasterizerState").append(myImGuiTag).c_str();
			if (ImGui::Combo(rasterizerState.c_str(), &mySelectedRasterizerState, myRasterizerStatesConstChar.data(), static_cast<int>(myRasterizerStatesConstChar.size())))
			{
				graphicsEngine.GetStateManager().SetRasterizerState(graphicsEngine.GetContext(), static_cast<eRasterizerState>(mySelectedRasterizerState));
			}*/

			UpdateWindowsSizeSettings(currentWindowSize, windowSizes, selectedWindowSizeIndex, osWindow);

			ImGuiUtility::SeparatorDummy(0, heightPadding);

			//UpdateConsoleSettings(isConsoleOpen, windowTag);

			ImGuiUtility::SeparatorDummy(0, heightPadding);

			UpdateAndSetEditorStyle(imguiStyleManager);

			ImGuiUtility::SeparatorDummy(0, heightPadding);
			UpdateCursorSettings();
			ImGuiUtility::SeparatorDummy(0, heightPadding);
		}

		ImGui::End();
	}

	void ShowGraphicsSettingsWindow(GraphicsSettingsData& settings, bool& isWindowActive, const Blackboard& blackboard)
	{
		const float deltaTime = blackboard.Get<Key_DeltaTime>();
		settings.mShowFpsTimer.Update(std::chrono::duration<float>(deltaTime));

		const WindowView window = blackboard.Get<Key_WindowView>();
		Dimension2u currentWindowSize = window.GetClientSize();
		ImGuiStyleManager& imguiStyleManager = blackboard.Get<Key_ImGuiStyleManager>();
		GraphicsSettings& graphicsSettings = blackboard.Get<Key_GraphicsSettings>();
		RenderPopup(
			isWindowActive, 
			deltaTime, 
			settings.mConsoleIsOpen,
			settings.mMonitorUpdateFrequency,
			settings.mShowFpsTimer,
			settings.mPreviousFPS,
			settings.mWindowSizes,
			settings.mSelectedWindowSizeIndex,
			currentWindowSize, 
			graphicsSettings,
			imguiStyleManager, 
			window
		);
    }
}