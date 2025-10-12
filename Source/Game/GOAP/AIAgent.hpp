#pragma once
#include <vector>
#include "AIAttribute.hpp"
#include "AIAction.hpp"
#include "AIDynamicGrid.hpp"

namespace Simple
{

	class AIAgent final
	{
	public:

		AIAgent() = default;
		void Update(const float deltaTime);
		void MakeDecision();
		void UpdateAttributes();

		void AddAttribute(std::function<float()> aModifierFunction, const float aInitialValue = 0.f);
		void AddAction(const AIActionParams& aParams);

	private:

		DynamicPoint<SaturatedFloat> mPoint;
		std::vector<AIAttribute> myAttributes;
		std::vector<AIAction> myActions;
		AIAction* myCurrentAction = nullptr;
		AIAction* myNewAction = nullptr;

		AIDynamicGrid<AIAction*> myGrid;
	};
}