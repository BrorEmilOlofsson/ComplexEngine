#include "Engine/Precompiled/EnginePch.hpp"
#include "InputManager.hpp"

namespace Simple
{

	static bool IsKeyTriggered(const InputState& inputState, const InputTrigger& inputTrigger)
	{
		switch (inputTrigger.triggerType)
		{
		case eInputTriggerType::Pressed:
			return inputState.IsKeyPressed(inputTrigger.key);
			break;
		case eInputTriggerType::Released:
			return inputState.IsKeyReleased(inputTrigger.key);
			break;
		case eInputTriggerType::Held:
			return inputState.IsKeyHeld(inputTrigger.key);
			break;
		default:
			throw std::invalid_argument("Invalid trigger type");
			break;
		}
	}

	void InputManager::Update(const InputState& inputState)
	{
		for (auto& [inputTrigger, responses] : mInputResponses)
		{
			for (auto& response : responses)
			{
				response->SetIsTriggered(false);
			}

			if (IsKeyTriggered(inputState, inputTrigger))
			{
				responses.front()->SetIsTriggered(true);
			}
		}
	}

	InputResponseHandle InputManager::CreateInputResponse(eInputKey key, eInputTriggerType triggerType)
	{
		InputTrigger trigger
		{
			.key = key,
			.triggerType = triggerType
		};

		auto& v = mInputResponses[trigger];
		v.push_back(std::make_shared<InputResponse>());
		return InputResponseHandle(v.back());
	}
}