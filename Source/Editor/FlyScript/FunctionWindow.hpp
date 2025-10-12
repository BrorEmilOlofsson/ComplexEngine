#pragma once

namespace Simple
{

	class NodeScriptingWindow;

	class FunctionWindow
	{
	public:

		FunctionWindow(NodeScriptingWindow& aParentWindow);
		~FunctionWindow();

		void Update();


	private:

		NodeScriptingWindow& myParentWindow;
	};
}