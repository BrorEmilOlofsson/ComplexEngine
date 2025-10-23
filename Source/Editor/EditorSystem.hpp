#pragma once
#include "Utility/Blackboard.hpp"

namespace Simple
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