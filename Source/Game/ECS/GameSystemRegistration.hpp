#pragma once
#include "Systems/PlayerMovementSystem.hpp"
#include "Utility/TypeList.hpp"
#include "Game/ECS/GameComponents.hpp"

namespace Simple
{
	constexpr TypeList<PlayerMovementSystem> GameSystemTypeList{};

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