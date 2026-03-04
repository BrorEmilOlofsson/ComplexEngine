#pragma once
#include "Engine/Utility/Blackboard.hpp"

namespace CLX
{

	class ECS;
	
	class RenderSystem final
	{

	public:
		RenderSystem() = delete;

		static void Update(ECS& ecs, const float deltaTime, const Blackboard& blackboard);
		static void Render(const ECS& ecs, const Blackboard&);
		static void EditorUpdate(ECS& ecs, const Blackboard&);
	};
}