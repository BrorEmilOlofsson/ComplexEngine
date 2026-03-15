#include "Game/Precompiled/GamePch.hpp"
#include "AIAction.hpp"

namespace CLX
{

	AIAction::AIAction(const AIActionParams& params)
		: mPoint(params.mPoint)
		, mUpdateFunction(params.mUpdateFunction)
		, mEnterFunction(params.mEnterFunction)
		, mExitFunction(params.mExitFunction)
		, mInterruptFunction(params.mInterruptFunction)
	{
	}

	eAIUpdateResult AIAction::Update(const float deltaTime)
	{
		return mUpdateFunction(deltaTime);
	}

	void AIAction::Enter()
	{
		if (mEnterFunction)
		{
			mEnterFunction();
		}
	}

	void AIAction::Exit()
	{
		if (mExitFunction)
		{
			mExitFunction();
		}
	}

	void AIAction::Interrupt()
	{
		if (mInterruptFunction)
		{
			mInterruptFunction();
		}
	}
}