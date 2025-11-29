#pragma once
#include "Utility/Blackboard.hpp"

namespace Simple
{
	class ECS;
}

class PlayerMovementSystem
{
public:


	PlayerMovementSystem() = default;


	static void BeginPlay(Simple::ECS& ecs, const Simple::Blackboard& blackboard);
	static void Update(Simple::ECS& ecs, const float deltaTime, const Simple::Blackboard& blackboard);
	static void EditorUpdate(Simple::ECS& ecs, const Simple::Blackboard& blackboard);

};