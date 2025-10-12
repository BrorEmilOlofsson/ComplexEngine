#pragma once

namespace Simple
{
	class ECS;
}

class PhysicsSystem final
{
public:

	PhysicsSystem() = default;

	static void Update(Simple::ECS& ecs, const float, const Simple::Blackboard& blackboard);

	static void EditorUpdate(Simple::ECS& ecs, const Simple::Blackboard& blackboard);
};