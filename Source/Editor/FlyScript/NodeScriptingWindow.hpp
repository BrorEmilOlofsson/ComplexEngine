#pragma once
#include <vector>
#include <string>
#include <functional>

#include "Editor/Core/PopUp.hpp"
#include "FunctionWindow.hpp"
#include "FunctionSettingsWindow.hpp"
#include "FlyClassWindow.hpp"
#include "StructCreatorWindow.hpp"
#include "FlyReflectionMemoryWindow.hpp"
#include "FlyTraitWindow.hpp"
#include "FlyScriptEditorUtilities.hpp"
#include "NodeScript/Fly.hpp"
#include <External/imgui/imgui.h>

struct ImNodesContext;

namespace Fly
{
	class CommandTracker;
}

namespace CLX
{
	struct NodeContextHistory
	{
		int currentIndex = -1;
		std::vector<std::unique_ptr<NodeGraphContext>> history;
	};

	enum class eGraphMode
	{
		Class,
		Global
	};

	class NodeScriptingWindow : public PopUp
	{
	public:

		NodeScriptingWindow();

		NodeGraphContext& GetNodeContext();
		const NodeGraphContext& GetNodeContext() const;
		void SetNodeContext(Fly::NodeGraphProxy nodeGraph, Fly::ClassProxy flyClass);
		eGraphMode GetCurrentMode() const;
		void SetSelectedFunction(Fly::FunctionProxy function);

		bool OpenClassByName(std::string_view aName);

		void UpdateContext();

		void Render(const Blackboard& blackboard) override;

		void ShowSelectionMenu();
		void ShowLoadingMenu();

		ImVec2 GetMiddlePos() const;

		Fly::FunctionProxy GetCurrentFunction();

		Fly::CommandTracker& GetCommandTracker()
		{
			return *GetNodeContext().mCommandTracker;
		}

	private:

		void UpdateClickPos();
		ImVec2 GetMousePos() const;

	private:

		char mNewClassNameText[NodeGraphContext::TEXT_MAX_LENGTH] = "";
		Fly::GenericDataTypeProxy mSelectedTargetDataType;
		char mCreateCopyNameText[NodeGraphContext::TEXT_MAX_LENGTH]{};

		FlyClassWindow mClassWindow;
		FunctionSettingsWindow mFunctionSettingsWindow;
		StructCreatorWindow mStructCreatorWindow;
		FlyReflectionMemoryWindow mReflectionMemoryWindow;
		FlyTraitWindow mTraitWindow;
        FunctionWindow mFunctionWindow;

		NodeContextHistory mNodeContextHistory;

		bool mIsContextSensitive = false;
		bool mIsNodeEditorHovered = false;

		Fly::FunctionProxy mSelectedFunction;

		static constexpr const char* ASSET_FILE_PATH = "Assets/FlyClasses";

	};
}