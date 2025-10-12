#pragma once

namespace Fly
{
	class VariableProxy;
}

namespace Simple
{

	class NodeScriptingWindow;

	class FlyClassWindow final
	{
	public:

		FlyClassWindow(NodeScriptingWindow& parentWindow);

		void Draw();

	private:

		void DrawGraphSelection();
		void DrawFunctionSelection();



		void DrawVariables();
		void ModifyVariablePopup(Fly::VariableProxy& variableProxy);

	private:

		NodeScriptingWindow& mParentWindow;
	};
}