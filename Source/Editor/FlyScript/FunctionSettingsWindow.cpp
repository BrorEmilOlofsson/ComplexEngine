#include "Editor/Precompiled/EditorPch.hpp"
#include "FunctionSettingsWindow.hpp"
#include "NodeScriptingWindow.hpp"
#include "NodeScript/Fly.hpp"

namespace Simple
{

    static void ShowInputsOrOutputs(Fly::FunctionProxy function, const Fly::eIODirection ioDirection, Fly::CommandTracker& commandTracker)
    {
        const std::string inputOutputLabel = Fly::SelectByIODirection(ioDirection, std::string("Input"), std::string("Output"));

        constexpr static const char* dataTypeStrID = "Data Type##FunctionSettings_";
        constexpr static const char* pinTypeNameStrID = "Name##FunctionSettings_";

        const Fly::NodeTypeProxy callerNodeTypeProxy = function.GetCallerNodeType();

        const std::vector<Fly::PinTypeProxy> pinTypes = Fly::SelectByIODirection(ioDirection, callerNodeTypeProxy.GetInputPinTypes(), callerNodeTypeProxy.GetOutputPinTypes());
        for (size_t i = 1; i < pinTypes.size(); ++i)
        {
            const Fly::PinTypeProxy& pinType = pinTypes.at(i);

            const std::string& pinTypeName = pinType.GetName();

            char newName[32]{};
            strcpy_s(newName, pinTypeName.c_str());
            if (ImGui::InputText(std::string(pinTypeNameStrID + inputOutputLabel + std::to_string(i)).c_str(), newName, IM_ARRAYSIZE(newName)))
            {
                function.SetPinNameAtIndex(newName, i, ioDirection, &commandTracker);
            }

            const std::string comboLabel = dataTypeStrID + inputOutputLabel + std::to_string(i);

            const Fly::GenericDataTypeProxy pinTypeDataType(pinType.GetDataTypeID());

            if (ImGui::BeginCombo(comboLabel.c_str(), std::string(pinTypeDataType.GetName()).c_str()))
            {
                for (const Fly::GenericDataTypeProxy& dataTypeProxy : Fly::GetGenericDataTypes())
                {
                    if (ImGui::Selectable(std::string(dataTypeProxy.GetName()).c_str()))
                    {
                        function.SetPinDataTypeAtIndex(dataTypeProxy, i, ioDirection, &commandTracker);
                    }
                }
                ImGui::EndCombo();
            }

        }
    }

    static void ShowLocalVariables(Fly::FunctionProxy function, Fly::CommandTracker& commandTracker)
    {
        auto localVariables = function.GetLocalVariables();
        for (auto& localVariable : localVariables)
        {
            ImGui::Text("Local Variable: %s", localVariable.GetName().data());
            
            localVariable.ViewAndEditDefaultValue(&commandTracker);
        }
    }

    static void ShowFunction(Fly::FunctionProxy function, Fly::CommandTracker& commandTracker)
    {
        if (ImGui::Begin("Function Settings"))
        {
            const std::string& functionName = function.GetName();
            char nameBuffer[40]{};
            strcpy_s(nameBuffer, functionName.c_str());
            if (ImGui::InputText("Name", nameBuffer, IM_ARRAYSIZE(nameBuffer)))
            {
                function.SetName(nameBuffer, &commandTracker);
            }

            ImGui::Separator();

            ImGui::Text("Inputs");
            ImGui::SameLine();
            if (ImGui::Button("Add Input"))
            {
                function.AddPin(Fly::GenericDataTypeProxy(Fly::GetDataTypeID<bool>()), Fly::eIODirection::Input, "Pin", &commandTracker);
            }
            ImGui::Separator();

            ShowInputsOrOutputs(function, Fly::eIODirection::Input, commandTracker);

            ImGui::Separator();
            ImGui::Spacing();
            ImGui::Text("Outputs");
            ImGui::SameLine();
            if (ImGui::Button("Add Output"))
            {
                function.AddPin(Fly::GenericDataTypeProxy(Fly::GetDataTypeID<bool>()), Fly::eIODirection::Output, "Pin", &commandTracker);
            }
            ImGui::Separator();

            ShowInputsOrOutputs(function, Fly::eIODirection::Output, commandTracker);

            ShowLocalVariables(function, commandTracker);
        }
        ImGui::End();
    }


    FunctionSettingsWindow::FunctionSettingsWindow(NodeScriptingWindow& parent)
        : mParent(parent)
    {
    }

    void FunctionSettingsWindow::Update(Fly::CommandTracker& commandTracker)
    {
        if (!mParent.GetCurrentFunction())
        {
            return;
        }

        ShowFunction(mParent.GetCurrentFunction(), commandTracker);
    }
}
