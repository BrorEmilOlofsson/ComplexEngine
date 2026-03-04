#pragma once
#include "Engine/Utility/Blackboard.hpp"

namespace Simple
{

	class ECS;

	class RenderLightSystem final
	{
	public:
		RenderLightSystem() = default;

		static void Render(const ECS& ecs, const Blackboard& blackboard);
	};
}