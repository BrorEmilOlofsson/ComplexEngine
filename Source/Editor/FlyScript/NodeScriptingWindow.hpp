#pragma once
#include "Editor/Core/PopUp.hpp"
#include "CustomEventWindow.hpp"
#include "FunctionWindow.hpp"
#include "FunctionSettingsWindow.hpp"
#include "FlyClassWindow.hpp"
#include "StructCreatorWindow.hpp"
#include "FlyReflectionMemoryWindow.hpp"
#include "FlyTraitWindow.hpp"
#include "FlyScriptEditorUtilities.hpp"
#include "NodeScript/Fly.hpp"
#include <External/imgui/imgui.h>
#include <vector>
#include <string>
#include <functional>

struct ImNodesContext;

namespace Fly
{
	class CommandTracker;
}

namespace Simple
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

		NodeScriptingWindow(const std::string& name);

		NodeGraphContext& GetNodeContext();
		const NodeGraphContext& GetNodeContext() const;
		void SetNodeContext(Fly::NodeGraphProxy nodeGraphProxy, Fly::ClassProxy classProxy);
		eGraphMode GetCurrentMode() const;
		void SetSelectedFunctionProxy(Fly::FunctionProxy functionProxy);

		bool OpenClassByName(std::string_view aName);

		void UpdateContext();

		void Render(const Blackboard& aEditorMultiton) override;

		void ShowSelectionMenu();
		void ShowLoadingMenu();

		ImVec2 GetMiddlePos() const;

		Fly::FunctionProxy GetCurrentFunctionProxy();

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
		CustomEventWindow mCustomEventWindow;
		FunctionSettingsWindow mFunctionSettingsWindow;
		StructCreatorWindow mStructCreatorWindow;
		FlyReflectionMemoryWindow mReflectionMemoryWindow;
		FlyTraitWindow mTraitWindow;

		NodeContextHistory mNodeContextHistory;

		bool mIsContextSensitive = false;
		bool mIsNodeEditorHovered = false;

		Fly::FunctionProxy mSelectedFunctionProxy;

		static constexpr const char* ASSET_FILE_PATH = "Assets/FlyClasses";

	};
}