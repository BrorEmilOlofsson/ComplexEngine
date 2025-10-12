#pragma once

namespace Simple
{

	class ECS;

	class AnimationSystem final
	{
	public:

		AnimationSystem() = delete;

		static void Update(ECS& ecs, const float deltaTime, const Blackboard& blackboard);

	};
}