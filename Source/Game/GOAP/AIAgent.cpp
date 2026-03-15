#include "Game/Precompiled/GamePch.hpp"
#include "AIAgent.hpp"
#include "Engine/Math/DynamicMath.hpp"

namespace CLX
{

	void AIAgent::Update(const float deltaTime)
	{
		if (mCurrentAction == nullptr)
		{
			MakeDecision();
		}
		if (mNewAction != nullptr && mNewAction != mCurrentAction)
		{
			if (mCurrentAction)
			{
				mCurrentAction->Interrupt();
			}
			mNewAction->Enter();
			mCurrentAction = mNewAction;
			mNewAction = nullptr;
		}
		const eAIUpdateResult updateResult = mCurrentAction->Update(deltaTime);
		if (updateResult == eAIUpdateResult::Finished)
		{
			mCurrentAction->Exit();
			mCurrentAction = nullptr;
		}
	}

	void AIAgent::MakeDecision()
	{
		AIAction* newAction = [this]()
			{
				AIAction* newAction = nullptr;
				float min = std::numeric_limits<float>::max();
				for (AIAction& action : mActions)
				{
					const float evaluation = DistanceSquared(action.GetPoint(), mPoint, 
						[](const auto& a) -> float { return a ? a->Value() : 0.f; },
						[](const auto& b) -> float { return b.Value(); });
					if (evaluation < min)
					{
						min = evaluation;
						newAction = &action;
					}
				}
				return newAction;
			}();

		if (newAction != mCurrentAction)
		{
			mNewAction = newAction;
		}
	}

	void AIAgent::UpdateAttributes()
	{
		for (size_t i = 0; i < mPoint.GetDimensionCount(); i++)
		{
			mPoint[i] = SaturatedFloat(mAttributes[i].GetValue());
		}
	}

	void AIAgent::AddAction(const AIActionParams& params)
	{
		mActions.push_back(AIAction(params));
	}

	void AIAgent::AddAttribute(std::function<float()> modifierFunction, const float initialValue)
	{
		mPoint.AddDimension(SaturatedFloat(initialValue));
		mAttributes.emplace_back(std::move(modifierFunction));
	}
}