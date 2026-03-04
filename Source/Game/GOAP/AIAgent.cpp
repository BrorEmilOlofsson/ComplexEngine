#include "Game/Precompiled/GamePch.hpp"
#include "AIAgent.hpp"
#include "Engine/Math/DynamicMath.hpp"

namespace CLX
{

	void AIAgent::Update(const float deltaTime)
	{
		if (myCurrentAction == nullptr)
		{
			MakeDecision();
		}
		if (myNewAction != nullptr && myNewAction != myCurrentAction)
		{
			if (myCurrentAction)
			{
				myCurrentAction->Interrupt();
			}
			myNewAction->Enter();
			myCurrentAction = myNewAction;
			myNewAction = nullptr;
		}
		const eAIUpdateResult updateResult = myCurrentAction->Update(deltaTime);
		if (updateResult == eAIUpdateResult::Finished)
		{
			myCurrentAction->Exit();
			myCurrentAction = nullptr;
		}
	}

	void AIAgent::MakeDecision()
	{
		AIAction* newAction = [this]()
			{
				AIAction* newAction = nullptr;
				float min = std::numeric_limits<float>::max();
				for (AIAction& action : myActions)
				{
					const float evaluation = DistanceSquared(action.GetPoint(), mPoint, 
						[](const auto& a) -> float { return a ? a.value().Get() : 0.f; },
						[](const auto& b) -> float { return b.Get(); });
					if (evaluation < min)
					{
						min = evaluation;
						newAction = &action;
					}
				}
				return newAction;
			}();

		if (newAction != myCurrentAction)
		{
			myNewAction = newAction;
		}
	}

	void AIAgent::UpdateAttributes()
	{
		for (size_t i = 0; i < mPoint.GetDimensionCount(); i++)
		{
			mPoint[i] = myAttributes[i].GetValue();
		}
	}

	void AIAgent::AddAction(const AIActionParams& aParams)
	{
		myActions.push_back(AIAction(aParams));
	}

	void AIAgent::AddAttribute(std::function<float()> aModifierFunction, const float aInitialValue)
	{
		mPoint.AddDimension(aInitialValue);
		myAttributes.emplace_back(aModifierFunction);
	}
}