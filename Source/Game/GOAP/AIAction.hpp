#pragma once
#include <vector>
#include "AIMath.hpp"
#include "AICheckedValue.hpp"

namespace CLX
{
	enum class eAIUpdateResult
	{
		Ongoing,
		Finished,
	};

	template<typename T>
	using Saturate = decltype([](const T& aValue) -> T
		{
			return std::clamp(aValue, T{0}, T{1});
		});

	using SaturatedFloat = CheckedValue<float, Saturate<float>>;

	using DynamicPointOptf = DynamicPoint<std::optional<SaturatedFloat>>;

	struct AIActionParams
	{
		DynamicPointOptf mPoint;
		std::function<eAIUpdateResult(float deltaTime)> myUpdateFunction;
		std::function<void()> myEnterFunction;
		std::function<void()> myExitFunction;
		std::function<void()> myInterruptFunction;
	};

	class AIAction final
	{
	public:

		explicit AIAction(const AIActionParams& aParams);

		[[nodiscard]] constexpr const DynamicPointOptf& GetPoint() const;

		eAIUpdateResult Update(const float deltaTime);
		void Enter();
		void Exit();
		void Interrupt();

	private:

		DynamicPointOptf mPoint;
		std::function<void()> myEnterFunction;
		std::function<eAIUpdateResult(float)> myUpdateFunction;
		std::function<void()> myExitFunction;
		std::function<void()> myInterruptFunction;
	};

	constexpr const DynamicPointOptf& AIAction::GetPoint() const
	{
		return mPoint;
	}
}