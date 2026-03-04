#include "Game/Precompiled/GamePch.hpp"
#include "AIAction.hpp"

namespace CLX
{

	AIAction::AIAction(const AIActionParams& aParams)
		: mPoint(aParams.mPoint)
		, myUpdateFunction(aParams.myUpdateFunction)
		, myEnterFunction(aParams.myEnterFunction)
		, myExitFunction(aParams.myExitFunction)
		, myInterruptFunction(aParams.myInterruptFunction)
	{
	}

	eAIUpdateResult AIAction::Update(const float deltaTime)
	{
		return myUpdateFunction(deltaTime);
	}

	void AIAction::Enter()
	{
		if (myEnterFunction)
		{
			myEnterFunction();
		}
	}

	void AIAction::Exit()
	{
		if (myExitFunction)
		{
			myExitFunction();
		}
	}

	void AIAction::Interrupt()
	{
		if (myInterruptFunction)
		{
			myInterruptFunction();
		}
	}
}