#pragma once
#include "Utility/Blackboard.hpp"

namespace Simple
{

	class ECS;

	class AnimationSystem final
	{
	public:

		AnimationSystem() = delete;

		static void Update(ECS& ecs, const float deltaTime, const Blackboard& blackboard);
		static void EditorUpdate(ECS& ecs, const Blackboard& blackboard);

	};
}