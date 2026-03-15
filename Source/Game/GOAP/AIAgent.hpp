#pragma once
#include <vector>
#include "AIAttribute.hpp"
#include "AIAction.hpp"

namespace CLX
{

	class AIAgent final
	{
	public:

		AIAgent() = default;
		void Update(const float deltaTime);
		void MakeDecision();
		void UpdateAttributes();

		void AddAttribute(std::function<float()> modifierFunction, const float initialValue = 0.f);
		void AddAction(const AIActionParams& params);

	private:

		DynamicPoint<SaturatedFloat> mPoint;
		std::vector<AIAttribute> mAttributes;
		std::vector<AIAction> mActions;
		AIAction* mCurrentAction = nullptr;
		AIAction* mNewAction = nullptr;
	};
}