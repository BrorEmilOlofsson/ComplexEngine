#include "Editor/Precompiled/EditorPch.hpp"
#include "GraphicsSettingsPopUp.hpp"
#include "Engine/Utility/BlackboardKeys.hpp"
#include "Utility/Math/Vector2.hpp"
#include "Graphics/GraphicsSettings.hpp"
#include "Editor/ImGui/ImGuiStyleManager.hpp"
#include "Engine/OperatingSystem/WindowView.hpp"
#include <array>
#include <unordered_map>
#include <stdlib.h>

namespace Simple
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

	static std::string WindowSizeToString(Vector2ui aWindowSize)
	{
		return std::to_string(aWindowSize.x) + "x" + std::to_string(aWindowSize.y);
	}

	void UpdateWindowsSizeSettings(Vector2ui currentWindowSize, std::span<const Vector2ui> windowSizes, std::optional<int>& selectedWindowSizeIndex, WindowView window)
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

	void UpdateAndSetEditorStyle(const std::string& tag, ImGuiStyleManager& imguiStyleManager)
	{
		constexpr static std::array<const char*, 3> editorStyles = { "Simple", "Dark", "Light" };

		static int selectedStyle = 0;

		ImGui::SetNextItemWidth(200);

		const std::string editorStyle = std::string("Editor Style").append(tag).c_str();
		if (ImGui::Combo(editorStyle.c_str(), &selectedStyle, editorStyles.data(), static_cast<int>(editorStyles.size())))
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

	static void RenderPopup(bool& isActive, const std::string& windowName, const std::string& windowTag,
		const float deltaTime, [[maybe_unused]] bool& isConsoleOpen, const unsigned int monitorUpdateFrequency, const ManualTimerf& updateFpsTimer, float& fps,
		const std::span<const Vector2ui> windowSizes, std::optional<int>& selectedWindowSizeIndex,
		Vector2ui currentWindowSize, GraphicsSettings& graphicsSettings, ImGuiStyleManager& imguiStyleManager, WindowView osWindow)
	{
		if (ImGui::Begin(windowName.c_str(), &isActive))
		{
			constexpr unsigned int heightPadding = 2;

			if (updateFpsTimer.HasBeenTriggered())
			{
				fps = std::round(1.f / deltaTime);
			}
			ImGui::Text("FPS: %d", static_cast<int>(fps));

			ImGuiUtility::SeparatorTextDummy("Render");

			const std::string vSync = std::string("VSync").append(windowTag).c_str();
			ImGui::Checkbox(vSync.c_str(), &graphicsSettings.vSync);

			ImGuiUtility::SameLineDummy(3, 0);

			const std::string debugLines = std::string("DebugLines").append(windowTag).c_str();
			ImGui::Checkbox(debugLines.c_str(), &graphicsSettings.mShouldRenderDebugLines);

			ImGuiUtility::SameLineDummy(3, 0);

			ImGui::Checkbox(std::string("BoundingBox").append(windowTag).c_str(), &graphicsSettings.mShouldRenderBoundingBox);

			ImGui::Checkbox(std::string("Mesh").append(windowTag).c_str(), &graphicsSettings.mShouldRenderMesh);

			ImGuiUtility::SameLineDummy(10, 0);

			ImGui::Checkbox(std::string("PBR").append(windowTag).c_str(), &graphicsSettings.mIsUsingPBR);

			ImGuiUtility::SameLineDummy(52, 0);

			ImGui::Checkbox(std::string("Skeleton").append(windowTag).c_str(), &graphicsSettings.mShouldRenderSkeletonLines);

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

			UpdateAndSetEditorStyle(windowTag, imguiStyleManager);

			ImGuiUtility::SeparatorDummy(0, heightPadding);
			UpdateCursorSettings();
			ImGuiUtility::SeparatorDummy(0, heightPadding);
		}

		ImGui::End();
	}

	GraphicsSettingsPopUp::GraphicsSettingsPopUp(const std::string& name)
		: PopUp(name)
	{
	}

	void GraphicsSettingsPopUp::Init()
	{
		InitRasterizerStatesStrings();

		//UpdateAndFetchFPSCapStrings();
		//UpdateAndFetchCurrentMonitorResolution();
		UpdateAndFetchCurrentCursorSettings();
		mShowFpsTimer.SetDuration(std::chrono::duration<float>(0.5f));
	}

	void GraphicsSettingsPopUp::UpdateInternal(const Blackboard& blackboard)
	{
		mShowFpsTimer.Update(std::chrono::duration<float>(blackboard.Get<Key_DeltaTime>()));
	}

	void GraphicsSettingsPopUp::Render(const Blackboard& blackboard)
	{
		const float deltaTime = blackboard.Get<Key_DeltaTime>();
		const WindowView window = blackboard.Get<Key_WindowView>();
		Vector2ui currentWindowSize = window.GetClientSize();
		ImGuiStyleManager& imguiStyleManager = blackboard.Get<Key_ImGuiStyleManager>();
		GraphicsSettings& graphicsSettings = blackboard.Get<Key_GraphicsSettings>();
		RenderPopup(
			mIsActive, mImGuiName, mImGuiTag, deltaTime, mConsoleIsOpen,
			mMonitorUpdateFrequency, mShowFpsTimer, mPreviousFPS, mWindowSizes, mSelectedWindowSizeIndex,
			currentWindowSize, graphicsSettings, imguiStyleManager, window
		);
	}

	void GraphicsSettingsPopUp::InitRasterizerStatesStrings()
	{
		ForEachEnum<eRasterizerState>([this](EnumIterator<eRasterizerState> iterator)
			{
				mRasterizerStatesConstChar[static_cast<unsigned int>(iterator.value)] = iterator.name;
			});
	}


	/*void GraphicsSettingsPopUp::UpdateAndFetchCurrentMonitorResolution()
	{
		HMONITOR hMonitor = MonitorFromWindow(GetDesktopWindow(), MONITOR_DEFAULTTOPRIMARY);

		MONITORINFOEX monitorInfo = { sizeof(MONITORINFOEX) };
		GetMonitorInfo(hMonitor, &monitorInfo);

		mMonitorResolution.x = static_cast<unsigned int>(monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left);
		mMonitorResolution.y = static_cast<unsigned int>(monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top);

		mWindowSizes.clear();
		mWindowSizes.push_back(Vector2ui(1280, 720));
		mWindowSizes.push_back(Vector2ui(1600, 900));
		mWindowSizes.push_back(Vector2ui(1920, 1080));

		if (mWindowSizes.back().x < mMonitorResolution.x &&
			mWindowSizes.back().y < mMonitorResolution.y)
		{
			mWindowSizes.push_back(mMonitorResolution);
		}
	}*/

	/*void GraphicsSettingsPopUp::UpdateAndFetchFPSCapStrings()
	{
		HDC hdc = GetDC(0);
		DEVMODE devMode{};
		devMode.dmSize = sizeof(DEVMODE);
		EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &devMode);

		mMonitorUpdateFrequency = static_cast<unsigned int>(devMode.dmDisplayFrequency);
		ReleaseDC(0, hdc);
	}*/

	void GraphicsSettingsPopUp::UpdateAndFetchCurrentCursorSettings()
	{
		mCursorNames.clear();
		mLongCursorStringName.clear();

		//const std::unordered_map<std::filesystem::path, const HCURSOR>& loadedCursors = GetLoadedCustomCursors();
		//const HCURSOR currentCursor = GetCurrentCustomCursor();

		/*for (const auto& [name, cursor] : loadedCursors)
		{
			myCursorNames.push_back(name.string());
			myLongCursorStringName += name.string();
			myLongCursorStringName += '\0';
		}

		myLongCursorStringName += '\0';

		unsigned int index = 0;

		for (const auto& [name, cursor] : loadedCursors)
		{
			if (currentCursor == cursor)
			{
				mySelectedCursor = index;
				break;
			}

			index++;
		}*/
	}
}