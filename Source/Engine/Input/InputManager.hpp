#pragma once
#include <unordered_map>
#include <queue>
#include "Engine/Input/InputState.hpp"

namespace Simple
{
	enum class eInputTriggerType
	{
		Pressed,
		Released,
		Held
	};

	template<>
	struct EnumCount<eInputTriggerType> : std::integral_constant<std::size_t, 3> {};

	class InputTrigger
	{
	public:
		eInputKey key = eInputKey::A;
		eInputTriggerType triggerType = eInputTriggerType::Pressed;
	};

	[[nodiscard]] constexpr bool operator==(const InputTrigger& a, const InputTrigger& b) noexcept
	{
		return a.key == b.key && a.triggerType == b.triggerType;
	}
}

template<>
struct std::hash<Simple::InputTrigger>
{
	std::size_t operator()(const Simple::InputTrigger& inputTrigger) const
	{
		return static_cast<std::underlying_type_t<Simple::eInputTriggerType>>(inputTrigger.triggerType) + 
			static_cast<std::underlying_type_t<Simple::eInputKey>>(inputTrigger.key) * Simple::EnumCount<Simple::eInputTriggerType>::value;
	}
};

namespace Simple
{

	class InputResponse
	{
	public:

		InputResponse() = default;

		bool IsTriggered() const
		{
			return mIsTriggered;
		}

		void SetIsTriggered(bool isTriggered)
		{
			mIsTriggered = isTriggered;
		}

	private:

		bool mIsTriggered = false;
	};


	class InputResponseHandle
	{
	public:

		InputResponseHandle() = default;

		explicit InputResponseHandle(std::shared_ptr<InputResponse> response)
			: mInputResponse(response)
		{
		}


		[[nodiscard]] operator bool() const
		{
			return !mInputResponse.expired() && mInputResponse.lock()->IsTriggered();
		}

	private:

		std::weak_ptr<InputResponse> mInputResponse;
	};



	class InputManager final
	{
	public:

		constexpr InputManager() = default;



		void Update(const InputState& inputState);

		InputResponseHandle CreateInputResponse(eInputKey key, eInputTriggerType triggerType);

	private:

		std::unordered_map<InputTrigger, std::vector<std::shared_ptr<InputResponse>>> mInputResponses;
	};
}