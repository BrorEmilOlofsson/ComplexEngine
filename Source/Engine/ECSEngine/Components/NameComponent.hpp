#pragma once
#include <string>

namespace CLX
{
	
	struct NameComponent final
	{
		std::string name = "Entity";
	};

	void RegisterNameComponent();
}