#pragma once
#include "Utility/Blackboard.hpp"

namespace Simple
{

	class NavmeshSystem final
	{
	public:

		NavmeshSystem() = delete;

		static void Render(const class ECS& ecs, const Blackboard& blackboard);
	};
}