#pragma once

namespace Fly
{
	class CommandTracker;
}

namespace CLX
{

	class NodeScriptingWindow;

	class FunctionSettingsWindow final
	{
	public:

		FunctionSettingsWindow(NodeScriptingWindow& parent);

		void Update(Fly::CommandTracker& commandTracker);

	private:

	private:

		NodeScriptingWindow& mParent;
	};
}