#pragma once

namespace Fly
{
	class CommandTracker;
}

namespace Simple
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