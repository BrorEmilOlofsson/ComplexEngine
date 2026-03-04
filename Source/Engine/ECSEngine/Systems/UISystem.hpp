#pragma once
#include "Engine/Utility/Blackboard.hpp"

namespace Simple
{
	class UISystem final
	{
	public:

		UISystem() = default;


		static void Update(class ECS& ecs, const float deltaTime, const Blackboard& blackboard);
	};
}