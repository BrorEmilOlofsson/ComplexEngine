#pragma once
#include "Engine/Utility/Blackboard.hpp"

namespace CLX
{

	class ECS;
	
	class EditorSystem final
	{
	public:

		EditorSystem() = delete;

		static void Update(ECS&, const float, const Blackboard&) {}
		static void EditorUpdate(ECS& ecs, const Blackboard& blackboard);
	};
}