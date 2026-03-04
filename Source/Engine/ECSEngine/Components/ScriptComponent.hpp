#pragma once
#include "NodeScript/Proxy/FlyClassInstanceProxy.hpp"

namespace CLX
{

	struct ScriptComponent final
	{
		Fly::ClassInstanceProxy classInstance;
	};

	void RegisterScriptComponent();

}