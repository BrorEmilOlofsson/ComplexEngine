#pragma once
#include "Engine/Utility/Blackboard.hpp"

namespace CLX
{

	class ECS;

	class AnimationSystem final
	{
	public:

		AnimationSystem() = delete;
		
		static void Update(ECS& ecs, const float deltaTime, const Blackboard& blackboard);
		static void EditorUpdate(ECS& ecs, const Blackboard& blackboard);
		static void Render(const ECS& ecs, const Blackboard& blackboard);
	};
}