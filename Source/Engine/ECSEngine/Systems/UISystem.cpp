#include "Engine/Precompiled/EnginePch.hpp"
#include "UISystem.hpp"
#include "Engine/ECS/ECS.hpp"
#include "Engine/ECSEngine/Components/UICanvasComponent.hpp"

namespace Simple
{

	static void UpdateUI(ECS& ecs)
	{
		ecs.ForEach([](UICanvasComponent& uiComponent)
			{
				uiComponent.mCanvas.Update();
			});
	}

	void UISystem::Update(ECS& ecs, const float, const Blackboard&)
	{
		UpdateUI(ecs);
	}
}
