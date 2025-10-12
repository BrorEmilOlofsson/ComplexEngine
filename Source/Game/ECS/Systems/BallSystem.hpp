#pragma once

namespace Simple
{
	class ECS;
}

class BallSystem final
{
public:

	BallSystem() = default;

	static void BeginPlay(Simple::ECS& ecs, const Simple::Blackboard& blackboard);

	static void Update(Simple::ECS& ecs, const float deltaTime, const Simple::Blackboard& blackboard);
};