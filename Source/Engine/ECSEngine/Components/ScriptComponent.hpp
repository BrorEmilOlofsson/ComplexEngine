#pragma once
#include "NodeScript/Proxy/FlyClassInstanceProxy.hpp"

namespace Simple
{

	struct ScriptComponent final
	{
		Fly::ClassInstanceProxy classInstance;
	};

	void RegisterScriptComponent();

}