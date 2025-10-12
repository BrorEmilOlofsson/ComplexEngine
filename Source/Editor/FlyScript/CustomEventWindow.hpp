#pragma once
#include "NodeScript/FlyDefines.hpp"

namespace Fly
{
	class CustomEventProxy;
}

namespace Simple
{
	class NodeScriptingWindow;

	class CustomEventWindow final
	{
	public:

		CustomEventWindow(NodeScriptingWindow& parentWindow);

		void Update();

	private:

		void EditInputs(Fly::CustomEventProxy& customEventProxy);

	private:

		NodeScriptingWindow& mParentWindow;
	};
}