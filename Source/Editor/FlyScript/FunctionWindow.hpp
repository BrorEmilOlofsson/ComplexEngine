#pragma once

namespace Simple
{

	class NodeScriptingWindow;

	class FunctionWindow
	{
	public:

		FunctionWindow(NodeScriptingWindow& aParentWindow);

		void Update();


	private:

		NodeScriptingWindow& myParentWindow;
	};
}