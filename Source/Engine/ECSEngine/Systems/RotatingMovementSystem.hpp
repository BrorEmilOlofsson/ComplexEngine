#pragma once
#include "Engine/Utility/Blackboard.hpp"

namespace CLX
{

	class RotatingMovementSystem final
	{
	public:

		RotatingMovementSystem() = delete;

		static void Update(class ECS& ecs, const float deltaTime, const Blackboard&);
	};
}