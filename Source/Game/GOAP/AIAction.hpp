#pragma once
#include <vector>
#include "Engine/Utility/StrongType.hpp"
#include "Engine/Math/DynamicPoint.hpp"

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

	using SaturatedFloat = StrongCheckedType<float, Saturate<float>, struct SaturatedFloatTag>;

	using DynamicPointOptf = DynamicPoint<std::optional<SaturatedFloat>>;

	struct AIActionParams
	{
		DynamicPointOptf mPoint;
		std::function<eAIUpdateResult(float deltaTime)> mUpdateFunction;
		std::function<void()> mEnterFunction;
		std::function<void()> mExitFunction;
		std::function<void()> mInterruptFunction;
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
		std::function<void()> mEnterFunction;
		std::function<eAIUpdateResult(float)> mUpdateFunction;
		std::function<void()> mExitFunction;
		std::function<void()> mInterruptFunction;
	};

	constexpr const DynamicPointOptf& AIAction::GetPoint() const
	{
		return mPoint;
	}
}