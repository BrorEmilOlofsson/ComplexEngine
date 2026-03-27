#include "Engine/Precompiled/EnginePch.hpp"
#include "RenderLightSystem.hpp"
#include "Engine/ECS/ECS.hpp"
#include "Engine/ECSEngine/Components/PointLightComponent.hpp"
#include "Engine/ECSEngine/Components/TransformComponent.hpp"
#include "Engine/Graphics/RenderState.hpp"
#include "Engine/ECSEngine/Utility/ECSTransformUtility.hpp"

namespace CLX
{

	static void ProcessPointLights(const ECS& ecs, RenderList& renderList)
	{
		ecs.ForEach([&ecs, &renderList](const EntityID entityID, const PointLightComponent& pointLightComponent)
			{
				PointLight p = pointLightComponent.pointLight;
				p.position = GetWorldTransform(ecs, entityID).GetPosition();
				renderList.AddPointLight(p);
			});
	}

	void RenderLightSystem::Render(const ECS& ecs, const Blackboard& blackboard)
	{
		ProcessPointLights(ecs, blackboard.Get<Key_CurrentRenderState>().GetRenderList());
	}
}