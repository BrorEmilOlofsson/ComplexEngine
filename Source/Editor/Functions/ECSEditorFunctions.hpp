#pragma once
#include <string>
#include <set>

#include "Engine/ECS/EntityID.hpp"
#include "Engine/Utility/Blackboard.hpp"
#include "Engine/Asset/AssetTypes/EntityCompositionAsset.hpp"
#include "Engine/ECSEngine/Utility/ECSTransformUtility.hpp"

namespace CLX
{
	class Camera;
	class ECS;
	class InputState;
	class EditorCommandTracker;

	EntityID CreateEntity(ECS& ecs, std::vector<EntityID>& rootEntities, const EntityID parentID, EditorCommandTracker& commandTracker);
	void DestroyEntity(ECS& ecs, const EntityID entityID, std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker);
	void SelectEntity(EntityID entityID, EntityID& selectedEntityID, EditorCommandTracker& commandTracker);

	void ReorderEntity(ECS& ecs, const EntityID entityID, std::vector<EntityID>& rootEntities, const std::size_t newIndex, EditorCommandTracker& commandTracker);
	void SetParentEntity(ECS& ecs, const EntityID parentID, const EntityID childID, std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker, ChildIndexSetting indexSetting);

	void ShowEntityAddButtons(ECS& ecs, EntityID& selectedEntityID, std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker, const std::string& imGuiTag, EntityID defaultParentID);
	void ShowEntityHierarchy(ECS& ecs, ECS& ecsBuffer, std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker, const std::string& imGuiTag,
		EntityID& selectedEntityID, const std::set<EntityID>& uneditableEntities, std::string& entitySearchBuffer);
	void ShowEntityHierarchyWithAddButtons(ECS& ecs, ECS& ecsBuffer, std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker,
		const std::string& imGuiTag, EntityID& selectedEntityID, EntityID defaultParentID, const std::set<EntityID>& uneditableEntities, std::string& entitySearchBuffer);
	
	void ShowEntityName(ECS& ecs, const EntityID selectedEntityID, const InputState& input, EditorCommandTracker& commandTracker);
	void ShowEntityComponents(ECS& ecs, EntityID entityID, bool& anyItemActiveLastFrame,
		ECS& ecsBuffer, EntityID& copyEntityID, const Blackboard& blackboard, EditorCommandTracker& commandTracker);
	void ShowEntityAddComponentButtons(ECS& ecs, EntityID entityID, const class DataTypeRegistry& dataTypeRegistry, EditorCommandTracker& commandTracker);
	void ShowEntityInspector(ECS& ecs, EntityID selectedEntityID, bool& anyItemActiveLastFrame,
		ECS& ecsBuffer, EntityID& copyEntityID, const Blackboard& blackboard, EditorCommandTracker& commandTracker);

	// Returns root entity of instantiated entity composition
	EntityID InstantiateEntityComposition(ECS& targetECS, const EntityCompositionAssetHandle& compositionAsset, std::vector<EntityID>& rootEntities, EditorCommandTracker& commandTracker);

	void TeleportCameraToEntity(const ECS& ecs, EntityID entityID, Camera& camera, bool changeRotation = true, const float offsetDistance = 5.f);
}