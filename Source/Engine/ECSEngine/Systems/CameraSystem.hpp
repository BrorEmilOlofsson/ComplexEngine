#pragma once
#include "Engine/ECS/System.hpp"

namespace Simple
{
	class CameraSystem final
	{
	public:

		CameraSystem() = delete;

		static void Update(ECS& ecs, const float, const Blackboard& blackboard);
		static void Render(const ECS& ecs, const Blackboard& blackboard);
	};
}
