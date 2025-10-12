#pragma once
#include "Editor/Core/PopUp.hpp"
#include "Utility/Math/Vector2.hpp"
#include "Graphics/GraphicsDeclarations.hpp"
#include "Utility/ManualTimer.hpp"
#include <vector>
#include <string>

namespace Simple
{

	class GraphicsSettingsPopUp final : public PopUp
	{
	public:

		GraphicsSettingsPopUp(const std::string& aName);

		void Init() override;
		void UpdateInternal(const Blackboard& blackboard) override;
		void Render(const Blackboard& blackboard) override;

	private:

		void InitRasterizerStatesStrings();
		void UpdateCursorSettings();
		//void UpdateAndFetchCurrentMonitorResolution();
		//void UpdateAndFetchFPSCapStrings();
		void UpdateAndFetchCurrentCursorSettings();

	private:

		std::array<std::string_view, static_cast<int>(eRasterizerState::Count)> mRasterizerStatesConstChar;

		std::vector<std::string> mCursorNames;
		std::vector<Vector2ui> mWindowSizes;

		std::string mLongCursorStringName;

		Vector2ui mMonitorResolution;

		std::optional<int> mSelectedWindowSizeIndex;
		unsigned int mMonitorUpdateFrequency = 60;
		int mSelectedRasterizerState = 0;
		int mSelectedCursor = 0;
		bool mConsoleIsOpen = true;
		
		ManualTimerf mShowFpsTimer;
		float mPreviousFPS = 0.f;
	};
}