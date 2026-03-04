#include "Editor/Precompiled/EditorPch.hpp"
#include "FlyClassWindow.hpp"
#include "NodeScript/Fly.hpp"
#include "NodeScriptingWindow.hpp"
#include "FlyScriptEditorUtilities.hpp"

namespace CLX
{
	FlyClassWindow::FlyClassWindow(NodeScriptingWindow& parentWindow)
		: mParentWindow(parentWindow)
	{
	}

	void FlyClassWindow::Draw()
	{
		if (ImGui::Begin("Class Window"))
		{
			DrawGraphSelection();
			ImGui::Separator();
			DrawFunctionSelection();
			ImGui::Separator();
			DrawVariables();

		}

		ImGui::End();
	}

	void FlyClassWindow::DrawGraphSelection()
	{
		Fly::ClassProxy currentClass = mParentWindow.GetNodeContext().mClass;
		if (!currentClass)
		{
			return;
		}
		if (ImGui::Selectable("Event Graph", currentClass.GetEventGraph() == mParentWindow.GetNodeContext().mNodeGraph))
		{
			mParentWindow.SetNodeContext(currentClass.GetEventGraph(), currentClass);
		}
	}

	void FlyClassWindow::DrawFunctionSelection()
	{
		Fly::ClassProxy currentClass = mParentWindow.GetNodeContext().mClass;

		/*if (ImGui::Button("Create Member Function"))
		{
			Fly::FunctionProxy createdFunctionProxy = currentClassProxy.CreateMemberFunction("Function1");
			mParentWindow.SetNodeContext(createdFunctionProxy.GetNodeGraph(), currentClassProxy);
			mParentWindow.SetSelectedFunctionProxy(createdFunctionProxy);
			ImGui::SetNextItemOpen(true);
		}


		if (ImGui::TreeNode("Member Functions"))
		{

			std::vector<Fly::FunctionProxy> memberFunctions = currentClassProxy.GetFunctions();

			for (Fly::FunctionProxy& memberFunctionProxy : memberFunctions)
			{

				if (ImGui::Selectable(memberFunctionProxy.GetName().c_str(), memberFunctionProxy.GetNodeGraph() == mParentWindow.GetNodeContext().mNodeGraph))
				{
					mParentWindow.SetNodeContext(memberFunctionProxy.GetNodeGraph(), mParentWindow.GetNodeContext().mClass);
					mParentWindow.SetSelectedFunctionProxy(memberFunctionProxy);
					break;
				}
			}

			ImGui::TreePop();
		}*/

	}



	void FlyClassWindow::DrawVariables()
	{
		Fly::ClassProxy currentClass = mParentWindow.GetNodeContext().mClass;

		if (ImGui::Button("Create Variable"))
		{
			mParentWindow.GetNodeContext().mClass.CreateVariable(Fly::GenericDataTypeProxy(Fly::GetDataTypeID<bool>()), "Var", nullptr);
			ImGui::SetNextItemOpen(true);
		}

		if (ImGui::TreeNode("Variables"))
		{

			ImGui::Separator();

			std::vector<Fly::VariableProxy> variableProxys = currentClass.GetVariables();
			for (Fly::VariableProxy& variableProxy : variableProxys)
			{
				const std::string treeNodeStrID = std::string("##ScriptVariable" + std::to_string(variableProxy.GetID()));
				if (ImGui::TreeNode(treeNodeStrID.c_str(), std::string(variableProxy.GetName()).c_str()))
				{
					ImGui::SameLine(ImGui::GetWindowWidth() - 30.f);

					ImGui::BeginDisabled();
					ImGui::ColorButton("  ##Color", ImGui::ColorConvertU32ToFloat4(ToImGuiColor(Fly::GenericDataTypeProxy(variableProxy.GetDataTypeID()).GetColor())));
					ImGui::EndDisabled();

					ModifyVariablePopup(variableProxy);

					ImGui::TreePop();
				}
				else
				{
					ImGui::SameLine(ImGui::GetWindowWidth() - 30.f);

					ImGui::BeginDisabled();
					ImGui::ColorButton("  ##Color", ImGui::ColorConvertU32ToFloat4(ToImGuiColor(Fly::GenericDataTypeProxy(variableProxy.GetDataTypeID()).GetColor())));
					ImGui::EndDisabled();
				}

				ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0, 0, 0, 1));
				ImGui::Separator();
				ImGui::PopStyleColor();
			}

			ImGui::TreePop();
		}

	}

	void FlyClassWindow::ModifyVariablePopup(Fly::VariableProxy& variableProxy)
	{
		std::string variableName(variableProxy.GetName());

		if (ImGui::InputString<35>("##VariableName", variableName))
		{
			variableProxy.SetName(variableName, nullptr);
		}

		Fly::GenericDataTypeProxy currentDataTypeProxy(variableProxy.GetDataTypeID());

		ImGui::Separator();
		if (DataTypeComboEditableFilter("##ChangeDataType", currentDataTypeProxy))
		{
			variableProxy.SetDataType(currentDataTypeProxy, nullptr);
		}

		ImGui::Text("Default Value:");
		ImGui::SameLine();
		variableProxy.ViewAndEditDefaultValue(nullptr);

		ImGui::Separator();

		if (ImGui::Button("Create Getter"))
		{
			//myParentWindow.GetNodeContext().myClassProxy.GetEventGraphProxy().CreateGetterNode(aVariableProxy, Fly::Vec2{}, &myParentWindow.GetCommandTracker());
			ImGui::CloseCurrentPopup();

		}

		ImGui::SameLine();

		if (ImGui::Button("Create Setter"))
		{
			//myParentWindow.GetNodeContext().myClassProxy.GetEventGraphProxy().CreateSetterNode(aVariableProxy, Fly::Vec2{}, &myParentWindow.GetCommandTracker());
		}

		if (ImGui::Button("Delete Variable"))
		{
			variableProxy.Destroy(nullptr);
		}
	}
}