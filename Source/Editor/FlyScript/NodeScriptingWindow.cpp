#include "Editor/Precompiled/EditorPch.hpp"
#include "NodeScriptingWindow.hpp"
#include "NodeScript/Fly.hpp"
#include "NodeScript/Command/FlyCommandTracker.hpp"
#include "NodeScript/NodeTypes/ExecutionNodes.hpp"
#include "FlyScriptEditorUtilities.hpp"
#include "EditorNodeRegistration.hpp"
#include "Editor/ImGui/ImGuiBegin.hpp"

#include <External/imgui/imnodes/imnodes_internal.h>
#include <External/imgui/imgui-node-editor/imgui_node_editor.h>

namespace Simple
{

	NodeScriptingWindow::NodeScriptingWindow()
		: PopUp("Node Scripting")
		, mClassWindow(*this)
		, mCustomEventWindow(*this)
		, mFunctionSettingsWindow(*this)
		, mStructCreatorWindow(this)
	{
		Fly::SetEditorTextFunction([](const std::string& text) { ImGui::TextWrapped(text.c_str()); });



		auto editor = ax::NodeEditor::CreateEditor();
		ax::NodeEditor::SetCurrentEditor(editor);
	}

	NodeGraphContext& NodeScriptingWindow::GetNodeContext()
	{
		return *mNodeContextHistory.history[mNodeContextHistory.currentIndex];
	}

	const NodeGraphContext& NodeScriptingWindow::GetNodeContext() const
	{
		return *mNodeContextHistory.history[mNodeContextHistory.currentIndex];
	}

	void NodeScriptingWindow::SetNodeContext(const Fly::NodeGraphProxy nodeGraphProxy, Fly::ClassProxy classProxy)
	{
		assert(nodeGraphProxy);
		assert(classProxy);

		std::unique_ptr<NodeGraphContext> nodeContext = std::make_unique<NodeGraphContext>();

		nodeContext->mClass = classProxy;
		nodeContext->mNodeGraph = nodeGraphProxy;
		nodeContext->mClassInstance = classProxy.CreateClassInstance();

		ImNodes::SetCurrentContext(nodeContext->mImNodesContext);
		mNodeContextHistory.history.push_back(std::move(nodeContext));
		mNodeContextHistory.currentIndex++;
	}

	eGraphMode NodeScriptingWindow::GetCurrentMode() const
	{
		return GetNodeContext().mClass ? eGraphMode::Class : eGraphMode::Global;
	}

	void NodeScriptingWindow::SetSelectedFunctionProxy(Fly::FunctionProxy functionProxy)
	{
		mSelectedFunctionProxy = functionProxy;
	}

	bool NodeScriptingWindow::OpenClassByName(std::string_view name)
	{
		if (Fly::ClassProxy classProxy = Fly::FindClassByName(name))
		{
			SetNodeContext(classProxy.GetEventGraph(), classProxy);
			return true;
		}

		if (Fly::GenericDataTypeProxy structProxy = Fly::FindDataTypeByName(name))
		{
			mStructCreatorWindow.SetStructProxy(structProxy);
			return true;
		}

		return false;
	}

	void NodeScriptingWindow::UpdateContext()
	{
		if (mNodeContextHistory.currentIndex == -1)
		{
			auto classes = Fly::GetClasses();
			if (classes.empty())
			{
				Fly::CreateClassWithoutTarget("Default Class", ASSET_FILE_PATH);
			}

			classes = Fly::GetClasses();
			if (classes.begin()->second.empty())
			{
				Fly::CreateClass(Fly::GenericDataTypeProxy(Fly::GetClasses().begin()->first), "Default Class", ASSET_FILE_PATH);
			}

			Fly::ClassProxy flyClass = classes.begin()->second.front();
			SetNodeContext(flyClass.GetEventGraph(), flyClass);
		}


		ImNodes::SetCurrentContext((*mNodeContextHistory.history.back()).mImNodesContext);

		ImNodesStyle& style = ImNodes::GetStyle();

		style.Colors[ImNodesCol_TitleBar] = ToImGuiColor(Fly::Color{ 0.1f, 0.3f , 0.6f , 1.f });
		style.Colors[ImNodesCol_TitleBarHovered] = ToImGuiColor(Fly::Color{ 0.1f, 0.3f , 0.7f , 1.f });
		style.Colors[ImNodesCol_TitleBarSelected] = ToImGuiColor(Fly::Color{ 0.1f, 0.3f , 0.8f , 1.f });

		style.Colors[ImNodesCol_NodeBackground] = ToImGuiColor(Fly::Color{ 0.02f, 0.02f, 0.02f, 1.f });
		style.Colors[ImNodesCol_NodeBackgroundHovered] = ToImGuiColor(Fly::Color{ 0.015f, 0.015f, 0.015f, 1.f });
		style.Colors[ImNodesCol_NodeBackgroundSelected] = ToImGuiColor(Fly::Color{ 0.01f, 0.01f, 0.01f, 1.f });

		style.Colors[ImNodesCol_NodeOutline] = ToImGuiColor(Fly::Color{ 0.0f, 0.0f, 0.0f, 1.f });
		style.Colors[ImNodesCol_GridBackground] = ToImGuiColor(Fly::Color{ 0.03f,0.03f, 0.03f, 1.f });

		style.Flags = ImNodesStyleFlags_GridSnapping | ImNodesStyleFlags_GridLines | ImNodesStyleFlags_NodeOutline;
		style.PinCircleRadius = 5.f;

		style.LinkThickness = 3.f;
	}

	void NodeScriptingWindow::Render(const Blackboard&)
	{
		PROFILER_FUNCTION(profiler::colors::Amber600);

		if (ImGui::Begin("Test Editor Window"))
		{

			ax::NodeEditor::Begin("TestEditor");

			ax::NodeEditor::BeginNode(1);
			ImGui::Text("Node A");
			ax::NodeEditor::BeginPin(1, ax::NodeEditor::PinKind::Input);
			ImGui::Text("-> In");
			ax::NodeEditor::EndPin();
			ImGui::SameLine();
			ax::NodeEditor::BeginPin(2, ax::NodeEditor::PinKind::Output);
			ImGui::Text("Out ->");
			ax::NodeEditor::EndPin();
			ax::NodeEditor::EndNode();
			ax::NodeEditor::End();
		}
		ImGui::End();

		ImGuiBegin b("Node Scripting");

		if (!b.IsOpen())
		{
			return;
		}

		UpdateContext();

		if (ImGui::IsKeyDown(ImGuiMod_Ctrl) && ImGui::IsKeyPressed(ImGuiKey_Z))
		{
			GetNodeContext().mCommandTracker->UndoCommand();
		}
		else if (ImGui::IsKeyDown(ImGuiMod_Ctrl) && ImGui::IsKeyPressed(ImGuiKey_Y))
		{
			GetNodeContext().mCommandTracker->RedoCommand();
		}
		Fly::BeginFrame(GetNodeContext().mCommandTracker.get());

		if (ImGui::IsKeyDown(ImGuiKey_ModCtrl) && ImGui::IsKeyPressed(ImGuiKey_C))
		{
			const int numSelectedNodes = ImNodes::NumSelectedNodes();

			if (numSelectedNodes > 0)
			{
				std::vector<int> selectedNodeIDs(numSelectedNodes);

				ImNodes::GetSelectedNodes(selectedNodeIDs.data());

				std::vector<Fly::NodeID> nodeIDs;
				std::ranges::transform(selectedNodeIDs, std::back_inserter(nodeIDs), [](int aID) { return Fly::NodeID{ aID }; });

				Fly::CreateCopyBuffer(std::span(nodeIDs), GetNodeContext().mNodeGraph);
			}
		}
		else if (ImGui::IsKeyDown(ImGuiKey_ModCtrl) && ImGui::IsKeyPressed(ImGuiKey_V))
		{

			const Fly::Vec2 mousePos = Fly::Vec2{ GetMousePos().x, GetMousePos().y };

			Fly::PasteCopyBuffer(GetNodeContext().mNodeGraph, mousePos, GetNodeContext().mCommandTracker.get());
		}
		else if (ImGui::IsKeyDown(ImGuiKey_ModCtrl) && ImGui::IsKeyPressed(ImGuiKey_X))
		{
			const int numSelectedNodes = ImNodes::NumSelectedNodes();

			if (numSelectedNodes > 0)
			{
				std::vector<int> selectedNodeIDs(numSelectedNodes);

				ImNodes::GetSelectedNodes(selectedNodeIDs.data());

				std::vector<Fly::NodeID> nodeIDs;
				std::ranges::transform(nodeIDs, std::back_inserter(nodeIDs), [](int id) { return Fly::NodeID{ id }; });

				Fly::CreateCopyBuffer(std::span(nodeIDs), GetNodeContext().mNodeGraph);

				GetNodeContext().mNodeGraph.DestroySelection(nodeIDs, {}, nullptr);
			}
		}

		if (ImGui::IsKeyPressed(ImGuiKey_UpArrow))
		{
			GetNodeContext().mSearchNodeData.mCurrentIndex = std::max(0, GetNodeContext().mSearchNodeData.mCurrentIndex - 1);
		}
		else if (ImGui::IsKeyPressed(ImGuiKey_DownArrow))
		{
			GetNodeContext().mSearchNodeData.mCurrentIndex++;
		}

		bool& isDebugging = Fly::IsDebugging();

		ImGui::Checkbox("Debug", &isDebugging);

		if (isDebugging)
		{

			ImGui::Text((std::string("Undo Stack size: ") + std::to_string(GetNodeContext().mCommandTracker->GetUndoSize())).c_str());
			ImGui::SameLine();
			ImGui::Text((std::string("Redo Stack size: ") + std::to_string(GetNodeContext().mCommandTracker->GetRedoSize())).c_str());

		}

		ShowSelectionMenu();
		ShowLoadingMenu();
		UpdateContext();

		ShowNodeGraph(*mNodeContextHistory.history[mNodeContextHistory.currentIndex]);

		mClassWindow.Draw();
		mCustomEventWindow.Update();
		mStructCreatorWindow.Update();
		mReflectionMemoryWindow.Update();
		mTraitWindow.Update();


		if (GetNodeContext().mNodeGraph.GetType() == Fly::eNodeGraphType::Function)
		{
			mFunctionSettingsWindow.Update();
		}

	}

	void NodeScriptingWindow::ShowSelectionMenu()
	{

		ImGui::BeginDisabled(mNodeContextHistory.currentIndex == 0);
		if (ImGui::ArrowButton("NodeContextHistoryLeft", ImGuiDir_Left))
		{
			mNodeContextHistory.currentIndex--;
			UpdateContext();
		}
		ImGui::EndDisabled();

		ImGui::SameLine();

		ImGui::BeginDisabled(mNodeContextHistory.currentIndex + 1 == mNodeContextHistory.history.size());
		if (ImGui::ArrowButton("NodeContextHistoryRight", ImGuiDir_Right))
		{
			mNodeContextHistory.currentIndex++;
			UpdateContext();
		}
		ImGui::EndDisabled();

		if (GetCurrentMode() == eGraphMode::Class)
		{
			ImGui::SameLine();

			std::string className = std::string(GetNodeContext().mClass.GetName());
			if (ImGui::InputString<32>("##", className))
			{
				GetNodeContext().mClass.SetName(className, nullptr);
			}
		}
	}

	void NodeScriptingWindow::ShowLoadingMenu()
	{
		Fly::ClassProxy currentClass = GetNodeContext().mClass;

		std::string currentClassName = currentClass ? std::string(currentClass.GetName()) : "None";

		if (ImGui::BeginCombo("Select Fly Class", currentClassName.c_str()))
		{
			const auto classes = Fly::GetClasses();

			for (auto& [dataTypeProxy, classesByDataTypeID] : classes)
			{
				if (ImGui::BeginMenu(dataTypeProxy.GetName().c_str()))
				{
					for (auto& flyClass : classesByDataTypeID)
					{
						if (ImGui::MenuItem(std::string(flyClass.GetName()).c_str()))
						{
							SetNodeContext(flyClass.GetEventGraph(), flyClass);
						}
					}

					ImGui::EndMenu();
				}
			}
			ImGui::EndCombo();
		}

		const bool canSave = GetNodeContext().mCommandTracker->GetUndoSize() == 0;

		ImGui::BeginDisabled(canSave);

		if (ImGui::Button("Save"))
		{
			currentClass.Save(ASSET_FILE_PATH);
			GetNodeContext().mCommandTracker->Clear();
		}

		ImGui::EndDisabled();
		ImGui::SameLine();

		if (ImGui::Button("Reload All"))
		{
			Fly::LoadAllFlyFiles(ASSET_FILE_PATH);
		}

		ImGui::SameLine();

		if (ImGui::Button("Create New Class"))
		{
			ImGui::OpenPopup("Create New Class");
		}

		if (ImGui::BeginPopupModal("Create New Class", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::InputText("##", mNewClassNameText, IM_ARRAYSIZE(mNewClassNameText), ImGuiInputTextFlags_AutoSelectAll);

			DataTypeComboTargetableFilter("Select Target##CreateNewClassTarget", mSelectedTargetDataType);

			ImGui::Separator();

			if (ImGui::Button("Create", ImVec2(120, 0)))
			{
				Fly::ClassProxy createdClassProxy = Fly::CreateClass(mSelectedTargetDataType, mNewClassNameText, ASSET_FILE_PATH);
				mNewClassNameText[0] = (char)0;


				ImGui::CloseCurrentPopup();
			}


			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Create Copy"))
		{
			ImGui::OpenPopup("Create Copy Class");
		}

		if (ImGui::BeginPopupModal("Create Copy Class", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::InputText("##", mCreateCopyNameText, IM_ARRAYSIZE(mCreateCopyNameText), ImGuiInputTextFlags_AutoSelectAll);
			ImGui::Separator();

			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();

			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				mCreateCopyNameText[0] = (char)0;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Save Custom Events"))
		{
			Fly::SaveCustomEvents(ASSET_FILE_PATH);
		}

		if (GetCurrentMode() == eGraphMode::Class)
		{

			constexpr const char* events[] = { "Begin Play", "Tick", "End Play", "Editor Update" };
			static int currentEventIndex = 0;

			if (ImGui::BeginCombo("Event Type", events[currentEventIndex]))
			{
				for (int i = 0; i < IM_ARRAYSIZE(events); ++i)
				{
					if (ImGui::Selectable(events[i], i == currentEventIndex))
					{
						currentEventIndex = i;
					}
				}

				ImGui::EndCombo();
			}

			ImGui::GetFocusID();

			ImGui::SameLine();

			if (ImGui::Button("Execute Event"))
			{
				Fly::ClassInstanceProxy classInstanceProxy = GetNodeContext().mClass.CreateClassInstance();
				Fly::ExecutionContextBase c
				{
					.mDeltaTime = 0.01f
				};

				Fly::None none;
				switch (currentEventIndex)
				{
				case 0:
					classInstanceProxy.ExecuteEvent(Fly::BeginPlay, &none, c);
					break;
				case 1:
					classInstanceProxy.ExecuteEvent(Fly::Tick, &none, c);
					break;
				case 2:
					classInstanceProxy.ExecuteEvent(Fly::EndPlay, &none, c);
					break;
				case 3:
					break;
				default:
					break;
				}

				classInstanceProxy.Destroy();
			}
		}

		if (GetNodeContext().mClass.GetTargetDataType().GetID() == Fly::GetDataTypeID<Fly::None*>())
		{
			GetNodeContext().mClassInstance.GetClassInstance().mEventGraphInstance.Mirror();
			Fly::ExecutionContextBase c
			{
				.mDeltaTime = 0.01f
			};
			Fly::None none;
			PROFILER_BEGIN("Execute Event");
			GetNodeContext().mClassInstance.ExecuteEvent(EditorUpdate, &none, c);
			PROFILER_END();

			GetNodeContext().mTraversedLinks = Fly::GetTraversedLinks();
		}
	}

	ImVec2 NodeScriptingWindow::GetMiddlePos() const
	{
		ImNodesContext* currentImNodesContext = GetNodeContext().mImNodesContext;
		return currentImNodesContext->CanvasOriginScreenSpace + ImNodes::EditorContextGetPanning() / 2.f;
	}

	Fly::FunctionProxy NodeScriptingWindow::GetCurrentFunctionProxy()
	{
		return mSelectedFunctionProxy;
	}


	void NodeScriptingWindow::UpdateClickPos()
	{
		GetNodeContext().mNodeCreationClickPos = GetMousePos();
	}

	ImVec2 NodeScriptingWindow::GetMousePos() const
	{
		return ImGui::GetMousePosOnOpeningCurrentPopup() - GetNodeContext().mImNodesContext->CanvasOriginScreenSpace - ImNodes::EditorContextGetPanning();
	}
}