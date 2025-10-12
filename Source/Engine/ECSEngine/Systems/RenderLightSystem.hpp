#pragma once
#include "Engine/ECS/System.hpp"

namespace Simple
{

	class RenderLightSystem final
	{
	public:
		RenderLightSystem() = default;

		static void Render(const ECS& ecs, const Blackboard& blackboard);
	};
}