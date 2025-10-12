#pragma once
#include "Systems/PhysicsSystem.hpp"
#include "Systems/BallSystem.hpp"
#include "Systems/PlayerMovementSystem.hpp"
#include "Utility/TypeList.hpp"
#include "Engine/ECS/ECS.hpp"
#include "Game/ECS/GameComponents.hpp"

namespace Simple
{
	constexpr TypeList<PhysicsSystem, BallSystem, PlayerMovementSystem> GameSystemTypeList{};

	struct ECSGameSystemRegistration
	{
		ECSGameSystemRegistration()
		{
			/*[]<typename... Systems>(TypeList<Systems...>)
				{
					((ECSRegistry::Get().RegisterSystem<Systems>()), ...);
				}(GameSystemTypeList);*/

			RegisterGameComponents();
		}
	};

	inline ECSGameSystemRegistration gameSystemRegistration;
}