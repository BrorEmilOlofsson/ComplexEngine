#pragma once
#include "Engine/Utility/Blackboard.hpp"

namespace CLX
{
	class ECS;
}

class PlayerMovementSystem
{
public:


	PlayerMovementSystem() = default;


	static void BeginPlay(CLX::ECS& ecs, const CLX::Blackboard& blackboard);
	static void Update(CLX::ECS& ecs, const float deltaTime, const CLX::Blackboard& blackboard);
	static void EditorUpdate(CLX::ECS& ecs, const CLX::Blackboard& blackboard);

};