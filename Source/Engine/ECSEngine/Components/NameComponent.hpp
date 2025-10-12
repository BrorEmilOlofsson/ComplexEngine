#pragma once
#include <string>

namespace Simple
{
	
	struct NameComponent final
	{
		std::string name = "Entity";
	};

	void RegisterNameComponent();
}