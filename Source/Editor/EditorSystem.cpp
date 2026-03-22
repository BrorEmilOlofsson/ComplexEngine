#include "Editor/Precompiled/EditorPch.hpp"
#include "EditorSystem.hpp"
#include "Engine/ECS/ECS.hpp"
#include "Engine/ECSEngine/Components/EntityCompositionComponent.hpp"
#include "Engine/ECS/EntityComposition.hpp"
#include "Engine/ECSEngine/Components/TransformHierarchyComponent.hpp"
#include "Engine/ECSEngine/Utility/ECSTransformHierarchyUtility.hpp"

namespace CLX
{

	static void ReplaceHierarchy(ECS& targetECS, const EntityID targetEntity, const ECS& sourceECS, const EntityID sourceEntity)
	{
		targetECS.ReplaceEntity(targetEntity, sourceECS, sourceEntity);

		const auto targetChildren = GetEntityChildren(targetECS, targetEntity);
		const auto sourceChildren = GetEntityChildren(sourceECS, sourceEntity);
		if (targetChildren.size() != sourceChildren.size())
		{
			throw std::exception("Trying to replace entities but children are not the same size");
		}
		for (std::size_t i = 0; i < targetChildren.size(); i++)
		{
			ReplaceHierarchy(targetECS, targetChildren[i], sourceECS, sourceChildren[i]);
		}
	}

	void ReplaceHierarchy(ECS& targetECS, const EntityID entityID, const EntityComposition& entityComposition)
	{
		ReplaceHierarchy(targetECS, entityID, entityComposition.GetECS(), entityComposition.GetRootEntity());
	}

	static void UpdateEntityCompositionComponents(ECS& ecs)
	{
		ecs.ForEach([&ecs](const EntityID entityID, EntityCompositionComponent& entityCompositionComponent)
			{
				if (EntityCompositionAssetHandle& asset = entityCompositionComponent.asset)
				{
					if (!asset->HasChanged())
					{
						return;
					}

					ReplaceHierarchy(ecs, entityID, asset.Get());
					
				}
			});
	}

	void EditorSystem::EditorUpdate(ECS& ecs, const Blackboard& blackboard)
	{
		blackboard;
		UpdateEntityCompositionComponents(ecs);
	}
}