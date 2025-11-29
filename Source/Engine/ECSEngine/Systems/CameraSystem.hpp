#pragma once
#include "Utility/Blackboard.hpp"

namespace Simple
{

	class ECS;

	class CameraSystem final
	{
	public:

		CameraSystem() = delete;

		static void Update(ECS& ecs, const float, const Blackboard& blackboard);
		static void Render(const ECS& ecs, const Blackboard& blackboard);
	};
}
