#pragma once
#include "Editor/Core/PopUp.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Graphics/GraphicsDeclarations.hpp"
#include "Engine/Utility/ManualTimer.hpp"
#include <vector>
#include <string>
#include <optional>

namespace CLX
{

	inline auto GetRasterizerStateNames()
	{
		std::array<std::string_view, static_cast<std::size_t>(eRasterizerState::Count)> names;
		ForEachEnum<eRasterizerState>([&names](EnumIterator<eRasterizerState> iterator)
			{
				names[static_cast<unsigned int>(iterator.value)] = iterator.name;
			});

		return names;
	}

	struct GraphicsSettingsData
	{
		std::array<std::string_view, static_cast<std::size_t>(eRasterizerState::Count)> mRasterizerStateNames = GetRasterizerStateNames();

		std::vector<std::string> mCursorNames;
		std::vector<Vector2ui> mWindowSizes;

		std::string mLongCursorStringName;

		Vector2ui mMonitorResolution;

		std::optional<int> mSelectedWindowSizeIndex;
		unsigned int mMonitorUpdateFrequency = 60;
		int mSelectedRasterizerState = 0;
		int mSelectedCursor = 0;
		bool mConsoleIsOpen = true;

		ManualTimerf mShowFpsTimer = ManualTimerf(std::chrono::duration<float>(0.5));
		float mPreviousFPS = 0.f;

	};

    void ShowGraphicsSettingsWindow(GraphicsSettingsData& settings, bool& isWindowActive, const Blackboard& blackboard);

}