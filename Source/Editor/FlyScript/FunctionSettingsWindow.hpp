#pragma once
#include "NodeScript/Pin/FlyPinType.hpp"

namespace Simple
{

	class NodeScriptingWindow;

	class FunctionSettingsWindow final
	{
	public:

		FunctionSettingsWindow(NodeScriptingWindow& aParent);
		~FunctionSettingsWindow();

		void Update();

	private:

		void ShowInputOutput(Fly::eIODirection aIODirection);

	private:

		NodeScriptingWindow& myParent;
	};
}