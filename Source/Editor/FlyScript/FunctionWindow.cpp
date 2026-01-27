#include "Editor/Precompiled/EditorPch.hpp"
#include "FunctionWindow.hpp"
#include "NodeScriptingWindow.hpp"
#include "NodeScript/Fly.hpp"
#include "FlyScriptEditorUtilities.hpp"

namespace Simple
{

    FunctionWindow::FunctionWindow(NodeScriptingWindow& parentWindow)
        : myParentWindow(parentWindow)
    {
    }

    void FunctionWindow::Update()
    {

        if (ImGui::Begin("Free Functions"))
        {

            static std::vector<Fly::FunctionID> functions;

            if (ImGui::Button("Create Free Function"))
            {
                auto functionProxy = Fly::CreateFreeFunction("Function");
                functions.push_back(functionProxy.GetID());
            }

            ImGui::Separator();


            for (Fly::FunctionID functionID : functions)
            {
                Fly::FunctionProxy function(functionID);
                char functionNameBuffer[32]{};
                strcpy_s(functionNameBuffer, function.GetName().c_str());
                if (ImGui::InputText("Name", functionNameBuffer, 32))
                {
                    function.SetName(functionNameBuffer, nullptr);
                }
                const std::string functionlabel = function.GetName() + "##function" + std::to_string(function.GetID());
                if (ImGui::Selectable(functionlabel.c_str()))
                {
                    myParentWindow.SetNodeContext(function.GetNodeGraph(), {});
                    myParentWindow.SetSelectedFunction(function);
                }
            }

        }
        ImGui::End();

    }
}
