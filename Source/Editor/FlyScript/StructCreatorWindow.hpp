#pragma once
#include "NodeScript/Fly.hpp"

namespace CLX
{

	class NodeScriptingWindow;

	class StructCreatorWindow final
	{
	public:

		StructCreatorWindow(NodeScriptingWindow* aParentWindow);


		void Update();

		void SetStructProxy(Fly::GenericDataTypeProxy aDataType)
		{
			myGenericDataType = aDataType;
		}
	private:

		void ShowStructMember(Fly::VariableProxy aVariable);

	private:

		NodeScriptingWindow* myParentWindow = nullptr;
		Fly::GenericDataTypeProxy myGenericDataType;

	};
}